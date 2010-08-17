/*
   Copyright (C) 2010 by Sebastian Trueg <trueg@kde.org>

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#include "seriescache.h"

#include <tvdb/client.h>
#include <tvdb/series.h>

#include <KDebug>

SeriesCache::SeriesCache( QObject* parent )
    : QObject( parent ),
      m_requestIdCnt( 0 ),
      m_requestInProcess( false )
{
    m_tvdbClient = new Tvdb::Client( parent );
    connect( m_tvdbClient, SIGNAL(finished(Tvdb::Series)),
             SLOT(slotFinished(Tvdb::Series)) );
    connect( m_tvdbClient, SIGNAL(multipleResultsFound(QList<Tvdb::Series>)),
             SLOT(slotMultipleResultFound(QList<Tvdb::Series>)) );
}


SeriesCache::~SeriesCache()
{
}


int SeriesCache::querySeries( const QString& query )
{
    m_requests.append( Request( query, ++m_requestIdCnt ) );
    QMetaObject::invokeMethod( this, "nextRequest", Qt::QueuedConnection );
    return m_requestIdCnt;
}


void SeriesCache::nextRequest()
{
    if ( !m_requestInProcess &&
         !m_requests.isEmpty() ) {
        m_currentRequest = m_requests.dequeue();
        if ( m_series.contains( m_currentRequest.query ) ) {
            kDebug() << "Reusing from cache:" << m_currentRequest.query;
            emit requestDone( m_currentRequest.id, m_series[m_currentRequest.query] );
            QMetaObject::invokeMethod( this, "nextRequest", Qt::QueuedConnection );
        }
        else {
            kDebug() << "Starting query:" << m_currentRequest.query;
            m_requestInProcess = true;
            m_tvdbClient->getSeriesByName( m_currentRequest.query );
        }
    }
}


void SeriesCache::nextSubRequest()
{
    if ( !m_requestInProcess ) {
        if ( !m_currentRequest.subRequests.isEmpty() ) {
            m_requestInProcess = true;
            m_tvdbClient->getSeriesById( m_currentRequest.subRequests.dequeue() );
        }
    }
}


void SeriesCache::slotFinished( const Tvdb::Series& series )
{
    kDebug() << m_currentRequest.query;

    // reset state
    m_requestInProcess = false;

    // build the cache value
    if ( series.isValid() )
        m_series[m_currentRequest.query].append( series );

    // in case there is a sub request left handle that
    if ( m_currentRequest.subRequests.isEmpty() ) {
        emit requestDone( m_currentRequest.id, m_series[m_currentRequest.query] );
        QMetaObject::invokeMethod( this, "nextRequest", Qt::QueuedConnection );
    }
    else {
        QMetaObject::invokeMethod( this, "nextSubRequest", Qt::QueuedConnection );
    }
}


void SeriesCache::slotMultipleResultFound( const QList<Tvdb::Series>& results )
{
    kDebug() << m_currentRequest.query;

    // reset state
    m_requestInProcess = false;

    // start a sub-request for each usable result
    Q_FOREACH( const Tvdb::Series& s, results ) {
        m_currentRequest.subRequests.enqueue( s.id() );
    }
    nextSubRequest();
}

#include "seriescache.moc"
