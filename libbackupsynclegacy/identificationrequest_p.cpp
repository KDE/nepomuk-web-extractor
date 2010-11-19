/*
    This file is part of the Nepomuk KDE project.
    Copyright (C) 2010  Vishesh Handa <handa.vish@gmail.com>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/

#include "identificationrequest.h"
#include "identificationrequest_p.h"
#include "changelog.h"
#include "changelogrecord.h"
#include "syncfile.h"
#include "identificationset.h"
#include "nie.h"
#include "nfo.h"
#include "backupsync.h"

#include <QtCore/QDir>
#include <QtCore/QSet>

#include <Soprano/Statement>
#include <Soprano/Vocabulary/RDF>
#include <Soprano/Vocabulary/RDFS>
#include <Soprano/Model>
#include <Soprano/QueryResultIterator>
#include <Soprano/StatementIterator>
#include <Soprano/NodeIterator>
#include <Soprano/Node>

#include <Nepomuk/ResourceManager>
#include <Nepomuk/Resource>
#include <Nepomuk/Variant>

#include <KDebug>

int Nepomuk::Sync::IdentificationRequest::Private::NextId = 0;

Nepomuk::Sync::IdentificationRequest::Private::Private( IdentificationRequest * parent )
    : q( parent ),
      m_model(0),
      m_resourceManger(0),
      m_id(-1),
      m_minScore( 0.60 )
{;}

//static
Nepomuk::Sync::ResourceHash Nepomuk::Sync::IdentificationRequest::Private::convertToResourceHash( const QList< Soprano::Statement >& allStatements )
{
    //
    // Convert into multi hash for easier look up
    //
    QMultiHash<QUrl, Soprano::Statement> stHash;
    stHash.reserve( allStatements.size() );
    foreach( const Soprano::Statement & st, allStatements ) {
        QUrl uri = st.subject().uri();
        stHash.insert( uri, st );
    }

    //
    // Convert them into a better format --> ResourceStruct
    // and translate the home url
    //
    const QList<QUrl> & uniqueUris = stHash.uniqueKeys();

    ResourceHash resources;
    resources.reserve( uniqueUris.size() );

    foreach( const QUrl & resUri, uniqueUris ) {
        SimpleResource res;
        res.setUri( resUri );

        QList<Soprano::Statement> statements = stHash.values( resUri );
        foreach( const Soprano::Statement & st, statements ) {
            QUrl pred = st.predicate().uri();
            Soprano::Node obj = st.object();

            if( !res.contains( pred, obj ) )
                res.insert( pred, obj );
        }

        resources.insert( res.uri(), res );
    }

    return resources;
}


namespace {

    //TODO: Use Nepomuk::Type::Property
    bool isIdentifyingProperty( QUrl prop, Soprano::Model * model ) {
        QString query = QString::fromLatin1( "ask { %1 %2 %3 }" )
        .arg( Soprano::Node::resourceToN3( prop ) )
        .arg( Soprano::Node::resourceToN3(Soprano::Vocabulary::RDFS::subPropertyOf()) )
        .arg( Soprano::Node::resourceToN3(Nepomuk::Vocabulary::backupsync::identifyingProperty()) );
        return model->executeQuery( query, Soprano::Query::QueryLanguageSparql ).boolValue();
    }
}


void Nepomuk::Sync::IdentificationRequest::Private::loadChangeLogHash()
{
    // This function should only be called once, as it is quite an expensive affair
    if( !m_changeLogHash.isEmpty() )
        return;

    QList<ChangeLogRecord> records = m_masterChangeLog.toList();

    // Convert to set to remove duplicate statements
    QSet<Soprano::Statement> statementSet;
    foreach( const ChangeLogRecord & r, records ) {
        statementSet.insert( r.st() );
    }

    //
    // Convert to list and remove all non-identifying properties
    //
    QList<Soprano::Statement> list = statementSet.toList();

    QMutableListIterator<Soprano::Statement> it( list );
    while( it.hasNext() ) {
        const Soprano::Statement & st = it.next();

        if( !isIdentifyingProperty( st.predicate().uri(), m_model ) ) {
            it.remove();
        }
    }

    m_changeLogHash = convertToResourceHash( list );
}



bool Nepomuk::Sync::IdentificationRequest::Private::identify( const QUrl& uri )
{
    if( simpleIdentify( uri ) )
        return true;

    if( existsIdentify( uri ) )
        return true;

    if( changeLogIdentify( uri ) )
        return true;

    if( addIdentify( uri ) )
        return true;

    return false;
}


namespace {

    //
    // Removes the old home directory and replaces it with the current one
    // TODO: Make it OS independent
    //
    QUrl translateHomeUri( const QUrl & uri ) {
        QString uriString = uri.toString();

        QRegExp regEx("^file://(/home/[^/]*)(/.*)$");
        if( regEx.exactMatch( uriString ) ) {
            QString newUriString = "file://" + QDir::homePath() + regEx.cap(2);

            uriString.replace( regEx, newUriString );
            return QUrl( newUriString );
        }
        return uri;
    }


    QString toSparql( const QUrl & prop, Soprano::Node obj ) {
        // Translate the home url
        if( obj.isResource() && obj.uri().scheme() == QLatin1String("file") )
            obj = Soprano::Node( translateHomeUri( obj.uri() ) );

        const QString pred = Soprano::Node( prop ).toN3();
        return QString(" ?r " + pred + " " + obj.toN3() + ". ");
    }
}


//FIXME: The scoring is currently wrong as the non-identified objects are not taken into count.
QString Nepomuk::Sync::IdentificationRequest::Private::constructIdentificationQuery( const Nepomuk::Sync::SimpleResource& res )
{
    const QString prefix = "select ?r count(?r) as ?cnt where { ";
    const QString postfix = " } ORDER BY DESC(?cnt) LIMIT 1";

    QString query = prefix;

    QList<QUrl> properties = res.uniqueKeys();
    foreach( const QUrl & propUri, properties ) {

        QList<Soprano::Node> objList = res.values( propUri );
        foreach( const Soprano::Node& n, objList ) {
            QString q;
            // Handle nepomuk resources
            if( n.isResource() && n.uri().scheme() == QLatin1String("nepomuk") ) {
                const QUrl & uri = n.uri();
                if( !queryIdentify( uri ) ) {
                    continue;
                }

                Soprano::Node node( m_hash.value( uri ) );
                q += toSparql( propUri, node );
            }
            else {
                q += toSparql( propUri, n );
            }

            if( propUri == Soprano::Vocabulary::RDF::type() )
                query += " { " + q + " } ";
            else
                query += " OPTIONAL { " + q + " } ";
        }
    }

    query += postfix;
    return query;
}


bool Nepomuk::Sync::IdentificationRequest::Private::queryIdentify(const QUrl& oldUri)
{
    if( m_beingIdentified.contains( oldUri ) )
        return false;
    bool result = identify( oldUri );

    if( result )
        m_notIdentified.remove( oldUri );

    return result;
}


QUrl Nepomuk::Sync::IdentificationRequest::Private::findMatch(const Nepomuk::Sync::SimpleResource& simpleRes)
{
    //TODO: Try doing this in one query

    //
    // Get resources with the same type
    //
    QString query = QString::fromLatin1("select distinct ?r where {");
    QList<Soprano::Node> types = simpleRes.property( Soprano::Vocabulary::RDF::type() );
    foreach( const Soprano::Node & type, types ) {
        query += QString::fromLatin1(" ?r %1 %2. ")
                 .arg( Soprano::Node::resourceToN3(Soprano::Vocabulary::RDF::type()),
                       type.toN3() );
    }
    query += " }";

    //
    // Insert them in resourceCount with count = 0
    //
    Soprano::QueryResultIterator it = m_model->executeQuery( query, Soprano::Query::QueryLanguageSparql );
    QHash<QUrl, int> resourceCount;
    while( it.next() ) {
        resourceCount.insert( it[0].uri(), 0 );
    }

    //
    // Get all the other properties, and increase resourceCount accordingly.
    // Find the maxCount
    //
    int maxCount = 0;
    QList<QUrl> properties = simpleRes.uniqueKeys();
    foreach( const QUrl & propUri, properties ) {

        Soprano::Statement statement( Soprano::Node(), propUri, Soprano::Node(), Soprano::Node() );

        QList<Soprano::Node> objList = simpleRes.values( propUri );
        foreach( const Soprano::Node& n, objList ) {
            if( n.isResource() && n.uri().scheme() == QLatin1String("nepomuk") ) {
                if( !queryIdentify( n.uri() ) ) {
                    continue;
                }
            }

            statement.setObject( n );

            Soprano::NodeIterator iter = m_model->listStatements( statement ).iterateSubjects();
            while( iter.next() ) {
                QHash< QUrl, int >::iterator it = resourceCount.find( iter.current().uri() );
                if( it != resourceCount.end() ) {
                    it.value()++;
                }
            }
            maxCount++;
        }
    }

    if( resourceCount.isEmpty() )
        return QUrl();

    //
    // Find the resource with the max count
    //
    QHash< QUrl, int >::const_iterator maxIter = resourceCount.begin();
    QHash< QUrl, int >::const_iterator iter = resourceCount.constBegin();

    while( iter != resourceCount.constEnd() ) {
        if( iter.value() > maxIter.value() )
            maxIter = iter;
        iter++;
    }

    float score = ((float)maxIter.value())/ maxCount;
    if( score < m_minScore ) {
        return QUrl();
    }

    //
    // Check for resources which have the same score as maxIter
    //
    QList<QUrl> duplicates;
    iter = resourceCount.constBegin();
    while( iter != resourceCount.constEnd() ) {
        if( iter.value() == maxIter.value() )
            duplicates.append( iter.key() );
        iter++;
    }

    if( duplicates.size() > 1 ) {
        kDebug() << "WE GOT A PROBLEM!!";
        kDebug() << "More than one resource with the exact same score found";
        kDebug() << "NOT IDENTIFYING IT! Do it manually!";

        emit q->duplicateMatch( simpleRes.uri(), duplicates, score );
        return QUrl();
    }

    return maxIter.key();
}


bool Nepomuk::Sync::IdentificationRequest::Private::simpleIdentify(const QUrl& oldUri)
{
    if( m_hash.contains( oldUri ) )
        return true;

    const SimpleResource & res = m_resourceHash[ oldUri ];
    QUrl resourceUri = findMatch( res );

    if( resourceUri.isEmpty() )
        return false;

    m_hash[ oldUri ] = resourceUri;

    kDebug() << oldUri << " ---> " << resourceUri;
    outputDebugInfo( oldUri );
    return true;
}


QUrl Nepomuk::Sync::IdentificationRequest::Private::createNewResource(const Sync::SimpleResource & simpleRes)
{
    kDebug();
    Nepomuk::Resource res( m_resourceManger );

    if( simpleRes.isFileDataObject() ) {
        res = Nepomuk::Resource( simpleRes.nieUrl(), QUrl(), m_resourceManger );
        if( res.exists() ) {
            // If the resource already exists. We should not create it. This is to avoid the bug where
            // a different file with the same nie:url exists. If it was the same file, simpleIdentify
            // should have found it. If it hasn't, well tough luck. No other option but to manually
            // identify ( unless changeLogIdentify finds it )
            return QUrl();
        }
    }

    const QList<QUrl> & keys = simpleRes.uniqueKeys();
    foreach( const QUrl & prop, keys ) {
        //kDebug() << "Prop " << prop;

        const QList<Soprano::Node> nodeList = simpleRes.values( prop );
        res.setProperty( prop, Nepomuk::Variant::fromNodeList( nodeList ) );
    }
    return res.resourceUri();
}


bool Nepomuk::Sync::IdentificationRequest::Private::existsIdentify( const QUrl& oldUri )
{
    if( m_hash.contains( oldUri ) )
        return true;

    const Sync::SimpleResource & res = m_resourceHash[ oldUri ];

    const QUrl & nieProp = Nepomuk::Vocabulary::NIE::url();
    QList<Soprano::Node> nieUrls = res.values( nieProp );

    foreach( const Soprano::Node& nieUrl, nieUrls ) {
        if( QFile::exists( nieUrl.uri().toLocalFile() ) ) {
            //It exists. Create a new resource.

            QUrl newUri = createNewResource( res );
            if( newUri.isEmpty() ) {
                return false;
            }
            m_hash[ oldUri ] = newUri;

            kDebug() << oldUri << " ---> " << newUri;
            outputDebugInfo( oldUri );
            return true;
        }
    }

    return false;
}


bool Nepomuk::Sync::IdentificationRequest::Private::addIdentify(const QUrl& oldUri)
{
    if( m_hash.contains( oldUri ) )
        return true;

    const Sync::SimpleResource & res = m_resourceHash[ oldUri ];
    if( res.isFileDataObject() )
        return false;

    // Not a FileDataObject, just add it for now
    QUrl newUri = createNewResource( res );
    if( newUri.isEmpty() ) {
        return false;
    }
    m_hash[ oldUri ] = newUri;

    kDebug() << oldUri << " ---> " << newUri;
    outputDebugInfo( oldUri );
    return true;
}


bool Nepomuk::Sync::IdentificationRequest::Private::changeLogIdentify( const QUrl& oldUri )
{
    if( m_hash.contains( oldUri ) )
        return true;

    loadChangeLogHash();

    const Sync::SimpleResource & res = m_changeLogHash[ oldUri ];
    QUrl resourceUri = findMatch( res );

    if( resourceUri.isEmpty() )
        return false;

    m_hash[ oldUri ] = resourceUri;

    kDebug() << oldUri << " ---> " << resourceUri;
    outputDebugInfo( oldUri );
    return true;
}

namespace {

    QString humanReadsable( const QUrl & url ) {
        QString str = url.toString();
        if( str.contains("semanticdesktop") || str.contains("http://www.w3.org/") ) {
            QRegExp regex("^http://www.*\\d*/\\d*/\\d*/(.*)$");
            if( regex.exactMatch( str ) ) {
                QString part = regex.cap( 1 );
                part.replace( "#", ":" );
                return part;
            }
        }
        return str;
    }
}

void Nepomuk::Sync::IdentificationRequest::Private::outputDebugInfo(const QUrl& uri) const
{
    Q_UNUSED( uri );
    /*
    const Sync::SimpleResource & res = m_resourceHash.value( uri );

    kDebug() << res.uri;
    const QList<QUrl> properties = res.propHash.uniqueKeys();
    foreach( const QUrl & prop, properties ) {
        kDebug() << humanReadsable( prop );

        QList< Soprano::Node > objects = res.propHash.values( prop );
        foreach( const Soprano::Node & n, objects ) {
            if( n.isResource() )
                qDebug() << humanReadsable( n.uri() );
        }
    }*/
}
