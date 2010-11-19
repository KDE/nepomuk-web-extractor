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

#include "identificationset.h"
#include "changelog.h"
#include "changelogrecord.h"
#include "backupsync.h"

#include <QtCore/QList>
#include <QtCore/QFile>
#include <QtCore/QRegExp>
#include <QtCore/QHash>
#include <QtCore/QMultiHash>
#include <QtCore/QHashIterator>
#include <QtCore/QMutableHashIterator>
#include <QtCore/QSet>
#include <QtCore/QMutableSetIterator>

#include <Soprano/Model>
#include <Soprano/QueryResultIterator>
#include <Soprano/Vocabulary/RDF>
#include <Soprano/Vocabulary/RDFS>
#include <Soprano/Serializer>
#include <Soprano/Parser>
#include <Soprano/Util/SimpleStatementIterator>
#include <Soprano/PluginManager>

#include <Nepomuk/ResourceManager>

#include <KDebug>

namespace {
    class IdentificationSetGenerator {
    public :
        IdentificationSetGenerator( const QSet<QUrl>& uniqueUris, Soprano::Model * m , const QSet<QUrl> & ignoreList = QSet<QUrl>());

        Soprano::Model * model;
        QSet<QUrl> done;
        QSet<QUrl> notDone;

        QList<Soprano::Statement> statements;

        Soprano::QueryResultIterator performQuery( const QStringList& uris );
        void iterate();
        QList<Soprano::Statement> generate();

        static const int maxIterationSize = 50;
    };

    IdentificationSetGenerator::IdentificationSetGenerator(const QSet<QUrl>& uniqueUris, Soprano::Model* m, const QSet<QUrl> & ignoreList)
    {
        notDone = uniqueUris - ignoreList;
        model = m;
        done = ignoreList;

    }

    Soprano::QueryResultIterator IdentificationSetGenerator::performQuery(const QStringList& uris)
    {
        QString query = QString::fromLatin1("select distinct ?r ?p ?o where { ?r ?p ?o. "
                                            "{ ?p %1 %2 .} "
                                            "UNION { ?p %1 %3. } "
                                            " FILTER( ?r in ( %4 ) ) . } ")
                        .arg(Soprano::Node::resourceToN3(Soprano::Vocabulary::RDFS::subPropertyOf()),
                             Soprano::Node::resourceToN3(Nepomuk::Vocabulary::backupsync::identifyingProperty()),
                             Soprano::Node::resourceToN3(Soprano::Vocabulary::RDF::type()),
                             uris.join(", "));


        return model->executeQuery(query, Soprano::Query::QueryLanguageSparql);
    }

    void IdentificationSetGenerator::iterate()
    {
        QStringList uris;

        QMutableSetIterator<QUrl> iter( notDone );
        while( iter.hasNext() ) {
            const QUrl & uri = iter.next();
            iter.remove();

            done.insert( uri );
            uris.append( Soprano::Node::resourceToN3( uri ) );

            if( uris.size() == maxIterationSize )
                break;
        }

        Soprano::QueryResultIterator it = performQuery( uris );
        while( it.next() ) {
            Soprano::Node sub = it["r"];
            Soprano::Node pred = it["p"];
            Soprano::Node obj = it["o"];

            statements.push_back( Soprano::Statement( sub, pred, obj ) );

            // If the object is also a nepomuk uri, it too needs to be identified.
            const QUrl & objUri = obj.uri();
            if( objUri.toString().startsWith("nepomuk:/res/") ) {
                if( !done.contains( objUri ) ) {
                    notDone.insert( objUri );
                }
            }
        }
    }

    QList<Soprano::Statement> IdentificationSetGenerator::generate()
    {
        done.clear();

        while( !notDone.isEmpty() ) {
            iterate();
        }
        return statements;
    }
}


class Nepomuk::Sync::IdentificationSet::Private : public QSharedData {
public:
    QList<Soprano::Statement> m_statements;
};


Nepomuk::Sync::IdentificationSet::IdentificationSet()
    : d( new Nepomuk::Sync::IdentificationSet::Private )
{
}


Nepomuk::Sync::IdentificationSet::IdentificationSet(const Nepomuk::Sync::IdentificationSet& rhs)
    : d( rhs.d )
{
}


Nepomuk::Sync::IdentificationSet& Nepomuk::Sync::IdentificationSet::operator=(const Nepomuk::Sync::IdentificationSet& rhs)
{
    d = rhs.d;
    return *this;
}


Nepomuk::Sync::IdentificationSet::~IdentificationSet()
{
}


Nepomuk::Sync::IdentificationSet Nepomuk::Sync::IdentificationSet::fromUrl(const QUrl& url)
{
    QFile file( url.toLocalFile() );
    if( !file.open( QIODevice::ReadOnly | QIODevice::Text ) ) {
        kWarning() << "The file " << url << " failed to load";
        return IdentificationSet();
    }

    QTextStream in( &file );
    return fromTextStream( in );
}


Nepomuk::Sync::IdentificationSet Nepomuk::Sync::IdentificationSet::fromTextStream(QTextStream& ts)
{
    //
    // Parse all the statements
    //
    const Soprano::Parser * parser = Soprano::PluginManager::instance()->discoverParserForSerialization( Soprano::SerializationNQuads );

    if( !parser ) {
        kDebug() << "The required parser could not be loaded.";
        return IdentificationSet();
    }

    Soprano::StatementIterator iter = parser->parseStream( ts, QUrl(), Soprano::SerializationNQuads );

    IdentificationSet identSet;
    identSet.d->m_statements = iter.allElements();
    return identSet;
}

namespace {

    //
    // Seperate all the unique URIs of scheme "nepomuk" from the subject and object in all the statements.
    //
    // vHanda: Maybe we should separate the graphs as well. Identification isn't meant for graphs.
    QSet<QUrl> getUniqueUris( const QList<Nepomuk::Sync::ChangeLogRecord> records ) {
        QSet<QUrl> uniqueUris;
        foreach( const Nepomuk::Sync::ChangeLogRecord & r, records ) {
            QUrl sub = r.st().subject().uri();
            uniqueUris.insert( sub );

            // If the Object is a resource, then it has to be identified as well.
            const Soprano::Node obj = r.st().object();
            if( obj.isResource() ) {
                QUrl uri = obj.uri();
                if( uri.scheme() == QLatin1String("nepomuk") )
                    uniqueUris.insert( uri );
            }
        }
        return uniqueUris;
    }
}

Nepomuk::Sync::IdentificationSet Nepomuk::Sync::IdentificationSet::fromChangeLog(const Nepomuk::Sync::ChangeLog& log, Soprano::Model* model, const QSet<QUrl> & ignoreList)
{
    QSet<QUrl> uniqueUris = getUniqueUris( log.toList() );

    IdentificationSetGenerator ifg( uniqueUris, model, ignoreList );
    IdentificationSet is;
    is.d->m_statements = ifg.generate();
    return is;
}

Nepomuk::Sync::IdentificationSet Nepomuk::Sync::IdentificationSet::fromResource(const QUrl & resourceUrl, Soprano::Model* model, const QSet<QUrl> &  ignoreList)
{
    QSet<QUrl> uniqueUris;
    uniqueUris.insert(resourceUrl);

    IdentificationSetGenerator ifg(uniqueUris, model, ignoreList);
    IdentificationSet is;
    is.d->m_statements = ifg.generate();
    return is;
}


Nepomuk::Sync::IdentificationSet Nepomuk::Sync::IdentificationSet::fromResourceList(const QList<QUrl> resList, Soprano::Model* model)
{
    IdentificationSetGenerator ifg( resList.toSet(), model );
    IdentificationSet is;
    is.d->m_statements = ifg.generate();
    return is;
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


Nepomuk::Sync::IdentificationSet Nepomuk::Sync::IdentificationSet::fromOnlyChangeLog(const Nepomuk::Sync::ChangeLog& log )
{
    Soprano::Model * model = ResourceManager::instance()->mainModel();
    IdentificationSet is;

    foreach( const ChangeLogRecord record, log.toList() ) {
        QUrl propUri = record.st().predicate().uri();
        if( isIdentifyingProperty( propUri, model ) )
            is.d->m_statements << record.st();
    }
    return is;
}


bool Nepomuk::Sync::IdentificationSet::save( const QUrl& output ) const
{
    QFile file( output.path() );
    if( !file.open( QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate ) ) {
        kWarning() << "File could not be opened : " << output.path();
        return false;
    }

    QTextStream out( &file );
    return save( out );
}


//TODO: We could probably use some kind of error return codes
bool Nepomuk::Sync::IdentificationSet::save( QTextStream& out ) const
{
    if( d->m_statements.isEmpty() )
        return false;

    //
    // Serialize the statements and output them
    //
    const Soprano::Serializer * serializer = Soprano::PluginManager::instance()->discoverSerializerForSerialization( Soprano::SerializationNQuads );

    if( !serializer ) {
        kWarning() << "Could not find the required serializer";
        return false;
    }

    if( d->m_statements.empty() ) {
        kWarning() << "No statements to Serialize";
        return false;
    }

    Soprano::Util::SimpleStatementIterator it( d->m_statements );
    if( !serializer->serialize( it, out, Soprano::SerializationNQuads ) ) {
        kWarning() << "Serialization Failed";
        return false;
    }

    return true;
}


bool Nepomuk::Sync::IdentificationSet::contains(const QUrl & url) const
{
    foreach( const Soprano::Statement & st, d->m_statements)
    {
        if ( st.subject() == url  or st.object() == url or st.predicate() == url )
            return true;
    }
    return false;
}

QList< Soprano::Statement > Nepomuk::Sync::IdentificationSet::toList() const
{
    return d->m_statements;
}


void Nepomuk::Sync::IdentificationSet::clear()
{
    d->m_statements.clear();
}

Nepomuk::Sync::IdentificationSet& Nepomuk::Sync::IdentificationSet::operator+=(const Nepomuk::Sync::IdentificationSet& rhs)
{
    d->m_statements << rhs.d->m_statements;
    return *this;
}

void Nepomuk::Sync::IdentificationSet::mergeWith(const IdentificationSet & rhs)
{
    operator+=(rhs);
}

