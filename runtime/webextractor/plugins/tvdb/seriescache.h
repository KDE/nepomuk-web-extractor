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

#ifndef _SERIES_CACHE_H_
#define _SERIES_CACHE_H_

#include <QtCore/QObject>
#include <QtCore/QHash>
#include <QtCore/QQueue>
#include <QtCore/QPair>
#include <QtCore/QString>

#include <tvdb/series.h>

namespace Tvdb {
    class Client;
}

class SeriesCache : public QObject
{
    Q_OBJECT

public:
    SeriesCache( QObject* parent = 0 );
    ~SeriesCache();

    int querySeries( const QString& query );

Q_SIGNALS:
    /**
     * Emitted for each request issued with querySeries().
     */
    void requestDone( int id, const QList<Tvdb::Series>& series );

private Q_SLOTS:
    void nextRequest();
    void nextSubRequest();
    void slotFinished( const Tvdb::Series& series );
    void slotMultipleResultFound( const QList<Tvdb::Series>& results );

private:
    struct Request {
        Request()
            : seriesId( -1 ),
              id( -1 ) {
        }
        Request( const QString& q,
                 int id_ )
            : query( q ),
              seriesId( -1 ),
              id( id_ ) {
        }
        QString query;
        int seriesId;
        int id;
        QQueue<int> subRequests;
    };
    int m_requestIdCnt;
    QQueue<Request> m_requests;
    QHash<QString, QList<Tvdb::Series> > m_series;
    Tvdb::Client* m_tvdbClient;
    bool m_requestInProcess;
    Request m_currentRequest;
};

#endif
