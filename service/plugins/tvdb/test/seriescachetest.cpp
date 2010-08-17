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

#include "seriescachetest.h"
#include "seriescache.h"

#include <QtCore/QCoreApplication>

#include <KDebug>
#include <KComponentData>


SeriesCacheTest::SeriesCacheTest( QObject* parent )
    : QObject( parent )
{
    m_seriesCache = new SeriesCache( this );
    connect( m_seriesCache, SIGNAL(requestDone(int, QList<Tvdb::Series>)),
             SLOT(slotRequestDone(int, QList<Tvdb::Series>)) );
}


SeriesCacheTest::~SeriesCacheTest()
{
}


void SeriesCacheTest::performQueries( const QStringList& queries )
{
    Q_FOREACH( const QString& query, queries ) {
        int id = m_seriesCache->querySeries( query );
        m_queries.insert( id, query );
    }
}


void SeriesCacheTest::slotRequestDone( int id, const QList<Tvdb::Series>& series )
{
    kDebug() << id << series;
    m_queries.remove( id );
    if ( m_queries.isEmpty() )
        QCoreApplication::exit( 0 );
}


int main( int argc, char *argv[] )
{
    QCoreApplication app( argc, argv );
    KComponentData comp( "seriescachetest" );

    SeriesCacheTest sct;
    sct.performQueries( app.arguments().mid( 1 ) );

    return app.exec();
}

#include "seriescachetest.moc"
