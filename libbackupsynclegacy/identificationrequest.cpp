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
#include <Nepomuk/Vocabulary/NIE>
#include <Nepomuk/Vocabulary/NFO>
#include <Soprano/Vocabulary/NAO>
#include "backupsync.h"

#include <QtCore/QDir>
#include <QtCore/QSet>

#include <Soprano/Statement>
#include <Soprano/Vocabulary/RDF>
#include <Soprano/Vocabulary/RDFS>
#include <Soprano/Model>
#include <Soprano/QueryResultIterator>
#include <Soprano/Node>

#include <Nepomuk/ResourceManager>
#include <Nepomuk/Resource>
#include <Nepomuk/Variant>

#include <KDebug>


Nepomuk::Sync::IdentificationRequest::IdentificationRequest(QObject* parent)
    : QObject( parent ),
      d( new Nepomuk::Sync::IdentificationRequest::Private(this) )
{
}


Nepomuk::Sync::IdentificationRequest::IdentificationRequest(const Nepomuk::Sync::SyncFile& sf, QObject* parent)
    : QObject(parent),
      d( new Nepomuk::Sync::IdentificationRequest::Private(this) )
{
    d->m_identificationSet = sf.identificationSet();
    d->m_masterChangeLog = sf.changeLog();
    
    d->m_id = Private::NextId;
    Private::NextId++;

    initModel(ResourceManager::instance());
}


Nepomuk::Sync::IdentificationRequest::IdentificationRequest(const Nepomuk::Sync::SyncFile& sf, Soprano::Model* model, QObject* parent)
    : QObject( parent ),
      d( new Nepomuk::Sync::IdentificationRequest::Private(this) )
{
    d->m_identificationSet = sf.identificationSet();
    d->m_masterChangeLog = sf.changeLog();
    
    d->m_id = Private::NextId;
    Private::NextId++;

    initModel(model);
}


Nepomuk::Sync::IdentificationRequest::IdentificationRequest(const Nepomuk::Sync::ChangeLog& log, Soprano::Model* fromModel, Soprano::Model* toModel, QObject* parent)
    : QObject( parent ),
      d( new Nepomuk::Sync::IdentificationRequest::Private(this) )
{
    d->m_masterChangeLog = log;
    d->m_identificationSet = IdentificationSet::fromChangeLog( log, fromModel );

    d->m_id = Private::NextId;
    Private::NextId++;
    
    initModel(toModel);
}


Nepomuk::Sync::IdentificationRequest::IdentificationRequest(const Nepomuk::Sync::ChangeLog& log, const Nepomuk::Sync::IdentificationSet& identSet, Soprano::Model* toModel, QObject* parent)
    : QObject(parent),
      d( new Nepomuk::Sync::IdentificationRequest::Private(this) )
{
    kDebug() << "ChangeLog: ";// << log;
    foreach( const ChangeLogRecord &rec, log.toList() )
        kDebug() << rec.toString();
    
    d->m_masterChangeLog = log;
    d->m_identificationSet = identSet;
    
    d->m_id = Private::NextId;
    Private::NextId++;
    
    initModel(toModel);
}

void Nepomuk::Sync::IdentificationRequest::initModel( Soprano::Model * model )
{
    Q_ASSERT(model);
    d->m_model = model;
     // Create new or use default manager
    if( model != ResourceManager::instance()->mainModel() )
        d->m_resourceManger = ResourceManager::createManagerForModel( model );
    else
        d->m_resourceManger = ResourceManager::instance();
}

void Nepomuk::Sync::IdentificationRequest::initModel( Nepomuk::ResourceManager * manager )
{
    Q_ASSERT(manager);
    d->m_model = manager->mainModel();
    d->m_resourceManger = manager;
}

void Nepomuk::Sync::IdentificationRequest::load()
{
    if( !d->m_resourceHash.isEmpty() )
        return;

    d->m_resourceHash = d->convertToResourceHash( d->m_identificationSet.toList() );
    // The m_identificationSet isn't required any more.
    d->m_identificationSet.clear();

    // Adds all the resources to the notIdentified set
    d->m_notIdentified += d->m_resourceHash.keys().toSet();
}


Nepomuk::Sync::ChangeLog Nepomuk::Sync::IdentificationRequest::convert()
{
    QList<ChangeLogRecord> masterLogRecords = d->m_masterChangeLog.toList();
    kDebug() << "masterLogRecords : " << masterLogRecords.size();

    QList<ChangeLogRecord> identifiedRecords;
    QMutableListIterator<ChangeLogRecord> it( masterLogRecords );

    while( it.hasNext() ) {
        ChangeLogRecord r = it.next();

        // Identify Subject
        QUrl subUri = r.st().subject().uri();
        if( subUri.scheme() == QLatin1String("nepomuk") ) {
            QHash<QUrl, QUrl>::const_iterator iter = d->m_hash.constFind( subUri );
            if( iter != d->m_hash.constEnd() ) {
                r.setSubject( iter.value() );
            }
            else {
                continue;
            }
        }

        // Identify object
        if( r.st().object().isResource() ) {
            QUrl objUri = r.st().object().uri();
            if( objUri.scheme() == QLatin1String("nepomuk") ) {
                QHash<QUrl, QUrl>::const_iterator iter = d->m_hash.constFind( objUri );
                if( iter != d->m_hash.constEnd() ) {
                    r.setObject( iter.value() );
                }
                else {
                    continue;
                }
            }
        }

        identifiedRecords.push_back( r );

        // Remove the statement from the masterchangerecords
        it.remove();
    }

    // Update the master change log
    d->m_masterChangeLog = ChangeLog( masterLogRecords );
    
    return ChangeLog( identifiedRecords );
}


int Nepomuk::Sync::IdentificationRequest::id() const
{
    return d->m_id;
}


bool Nepomuk::Sync::IdentificationRequest::done() const
{
    return d->m_notIdentified.isEmpty();
}


void Nepomuk::Sync::IdentificationRequest::provideResourceUri(const QUrl& oldUri, const QUrl& newUri)
{
    d->m_hash[oldUri] = newUri;
    d->m_notIdentified.remove( oldUri );
}


namespace {

    QString stripFileName( QString & url ) {
        kDebug() << url;
        int lastIndex = url.lastIndexOf('/') + 1; // the +1 is because we want to keep the trailing /
        return url.remove( lastIndex, url.size() );
    }
}


void Nepomuk::Sync::IdentificationRequest::provideNieUrl( const QUrl & resourceUri, const QUrl & newNieUrl )
{
    const QUrl nieUrlProp = Nepomuk::Vocabulary::NIE::url();

    Sync::SimpleResource & res = d->m_resourceHash[ resourceUri ];
    QUrl oldNieUrl = res.nieUrl();

    //
    // Modify resourceUri's nie:url
    //
    res.remove( nieUrlProp );
    res.insert( nieUrlProp, Soprano::Node( newNieUrl ) );

    // Remove from list. Insert later 
    d->m_notIdentified.remove( resourceUri );
    
    //
    // Modify other non identified resources with similar nie:urls
    //
    QString oldString = oldNieUrl.toString();
    QString newString = newNieUrl.toString();

    if( !res.isFolder() ) {
        oldString = stripFileName( oldString );
        newString = stripFileName( newString );

        kDebug() << oldString;
        kDebug() << newString;
    }
    else {
        //TODO: Use KUrl. Folder name should end with /
    }

    foreach( const QUrl & uri, d->m_notIdentified ) {
        // Ignore If already identified
        if( d->m_hash.contains( uri ) )
            continue;

        Sync::SimpleResource& simpleRes = d->m_resourceHash[ uri ];
        // Check if it has a nie:url   
        QString nieUrl = simpleRes.nieUrl().toString();
        if( nieUrl.isEmpty() )
            return;

        // Modify the existing nie:url
        if( nieUrl.startsWith(oldString) ) {
            nieUrl.replace( oldString, newString );

            simpleRes.remove( nieUrlProp );
            simpleRes.insert( nieUrlProp, Soprano::Node( QUrl(nieUrl) ) );
        }
    }

    d->m_notIdentified.insert( resourceUri );
}


void Nepomuk::Sync::IdentificationRequest::identifyAll()
{
    int totalSize = d->m_notIdentified.size();
    int numDone = 0;
    kDebug() << totalSize;

    foreach( const QUrl & uri, d->m_notIdentified ) {
        const Sync::SimpleResource & res = d->m_resourceHash[ uri ];

        // If already identified
        if( d->m_hash.contains( uri ) ) {
            kDebug() << d->m_hash;
            continue;
        }

        d->m_beingIdentified.clear();

        if( d->identify( uri ) ) {
            kDebug() << "Emitting identified";
            emit identified( d->m_id, uri, d->m_hash.value(uri) );
            emit identified( uri, d->m_hash.value(uri) );
            d->m_notIdentified.remove( uri );
        }
        else {
            kDebug() << "Emiting not identified for " << res.uri();
            emit notIdentified( d->m_id, res.toStatementList() );
            emit notIdentified( uri );
        }
        
        numDone++;
        emit completed( static_cast<float>( numDone ) / totalSize );
    }

    // Just in case
    //d->m_notIdentified -= d->m_hash.keys().toSet();
}



bool Nepomuk::Sync::IdentificationRequest::ignore(const QUrl& uri, bool ignoreSub)
{
    if( d->m_hash.contains( uri ) )
        return false;

    const QUrl nieUrlProp = Nepomuk::Vocabulary::NIE::url();

    Sync::SimpleResource & res = d->m_resourceHash[ uri ];
    QList<Soprano::Node> nieUrlNodes = res.values( nieUrlProp );
    if( nieUrlNodes.size() != 1 )
        return false;

    KUrl mainNieUrl = nieUrlNodes.first().uri();

    // Remove the resource
    d->m_resourceHash.remove( uri );

    if( ignoreSub || res.isFolder() )
        return true;

    foreach( const QUrl & uri, d->m_notIdentified ) {
        Sync::SimpleResource res = d->m_resourceHash[ uri ];

        // If already identified
        if( d->m_hash.contains(uri) )
            continue;

        // Check if it has a nie:url
        QList<Soprano::Node> nieUrls = res.values( nieUrlProp );
        if( nieUrls.empty() )
            continue;

        QString nieUrl = nieUrls.first().uri().toString();

        // Check if the nie url contains the mainNieUrl
        if( nieUrl.contains( mainNieUrl.toLocalFile( KUrl::AddTrailingSlash ) ) ) {
            d->m_resourceHash.remove( uri );
        }
    }
    return true;
}


QUrl Nepomuk::Sync::IdentificationRequest::mappedUri( const QUrl & resourceUri ) const
{
    QHash<QUrl, QUrl>::const_iterator fit =  d->m_hash.find( resourceUri );
    if( fit != d->m_hash.end() )
        return fit.value();
    else
        return QUrl();
}


QHash<QUrl,QUrl> Nepomuk::Sync::IdentificationRequest::mappings() const
{
    return d->m_hash;
}

QSet<QUrl> Nepomuk::Sync::IdentificationRequest::unidentified() const
{
    return d->m_notIdentified;
}
