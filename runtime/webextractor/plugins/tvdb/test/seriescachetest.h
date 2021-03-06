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

#ifndef _SERIES_CACHE_TEST_H_
#define _SERIES_CACHE_TEST_H_

#include <QtCore/QObject>
#include <QtCore/QStringList>
#include <QtCore/QHash>

#include <tvdb/series.h>

class SeriesCache;

class SeriesCacheTest : public QObject
{
    Q_OBJECT

public:
    SeriesCacheTest( QObject* parent = 0 );
    ~SeriesCacheTest();

    void performQueries( const QStringList& queries );

private Q_SLOTS:
    void slotRequestDone( int id, const QList<Tvdb::Series>& series );

private:
    QHash<int, QString> m_queries;
    SeriesCache* m_seriesCache;
};

#endif
