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

#include "tvdbplugin_config.h"

#include "tvdbdatappreply.h"

#include "tvdbdatapp.h"

#include <webextractor/decisionfactory.h>
#include <Nepomuk/Resource>
#include <KDebug>

Nepomuk::TvdbReply::TvdbReply(TvdbDataPP* parent,
                              const WebExtractor::DecisionFactory* factory,
                              const Nepomuk::Resource& res,
                              const QString& name, int season, int episode ):
    SimpleDataPPReply(parent, factory, res),
    m_name( name ),
    m_season( season ),
    m_episode( episode )
{
    connect( parent->seriesCache(), SIGNAL( requestDone( int, QList<Tvdb::Series> ) ),
             SLOT( slotRequestDone( requestDone( int, QList<Tvdb::Series> ) ) );
    m_seriesCacheId = parent->seriesCache()->querySeries( name );
}

bool Nepomuk::TvdbReply::isValid() const
{
    kError() <<  "ATTENTION: Not implemented";
}

void Nepomuk::TvdbReply::abort()
{
    kError() <<  "ATTENTION: Not implemented";
}


void Nepomuk::TvdbReply::slotRequestDone( int id, const QList<Tvdb::Series>& results )
{
    if ( id == m_seriesCacheId ) {
        Q_FOREACH( const Tvdb::Series& series, results ) {
            Decision d = newDecision();



            addDecision( d );
        }

    }
}
