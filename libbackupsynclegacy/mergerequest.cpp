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

#include "mergerequest.h"
#include "changelog.h"
#include "changelogrecord.h"
#include <nepomuk/nie.h>
#include <nepomuk/nfo.h>
#include "backupsync.h"
#include "identificationrequest.h"
#include "graphwrapper_p.h"

#include <algorithm>

#include <QtCore/QMultiHash>
#include <QtCore/QHashIterator>
#include <QtCore/QThread>

#include <Soprano/Node>
#include <Soprano/Vocabulary/NAO>
#include <Soprano/Vocabulary/NRL>
#include <Soprano/Vocabulary/RDF>
#include <Soprano/Statement>
#include <Soprano/Model>
#include <Soprano/QueryResultIterator>
#include <Soprano/StatementIterator>
#include <Soprano/NodeIterator>

#include <Nepomuk/Resource>
#include <Nepomuk/Variant>
#include <Nepomuk/Types/Property>
#include <Nepomuk/ResourceManager>

#include <KDebug>

class Nepomuk::Sync::MergeRequest::Private
{
public:
    Soprano::Model * model;
    ChangeLog changeLog;
    GraphWrapper graphWrapper;
};


Nepomuk::Sync::MergeRequest::MergeRequest(const Nepomuk::Sync::ChangeLog& log, Soprano::Model * model)
    : d( new Nepomuk::Sync::MergeRequest::Private )
{
    d->model =  model;
    d->changeLog = log;

    ResourceManager * rm = ResourceManager::instance();
    if( model != ResourceManager::instance()->mainModel() )
        rm = ResourceManager::createManagerForModel( model );

    d->graphWrapper.setManager( rm );
}


Nepomuk::Sync::MergeRequest::MergeRequest()
    : d( new Nepomuk::Sync::MergeRequest::Private )
{
}


Nepomuk::Sync::MergeRequest::~MergeRequest()
{
}


void Nepomuk::Sync::MergeRequest::merge()
{
    kDebug();

    d->changeLog.sort();

    kDebug() << "The changeLog : ";
    foreach( const ChangeLogRecord & rec, d->changeLog.toList() )
        kDebug() << rec.toString();
    
    foreach( const ChangeLogRecord & rec, d->changeLog.toList() ) {
        if( rec.added() ) {
            Soprano::Statement st( rec.st() );

            // Avoid duplicates
            if( !d->model->containsAnyStatement( st.subject(), st.predicate(), st.object() ) ) {
                st.setContext( d->graphWrapper.currentGraph() );
                d->model->addStatement( st );
            }
        }
        else {
            d->model->removeAllStatements( rec.st().subject(), rec.st().predicate(), rec.st().context() );
        }

    }
}

#include "mergerequest.moc"
