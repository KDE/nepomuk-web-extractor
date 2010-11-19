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
#include "seriescache.h"
#include "tvdbplugin.h"

#include "tvseries.h"
#include "tvshow.h"

#include <webextractor/decisionfactory.h>
#include <webextractor/decision.h>

#include <Nepomuk/Resource>
#include <KDebug>
#include <KLocale>


Nepomuk::TvdbReply::TvdbReply( TvdbDataPP* parent,
                               const WebExtractor::DecisionFactory* factory,
                               const Nepomuk::Resource& res,
                               const QString& name, int season, int episode )
    : SimpleDataPPReply(parent, factory, res),
      m_name( name ),
      m_season( season ),
      m_episode( episode )
{
    connect( parent->seriesCache(), SIGNAL( requestDone( int, QList<Tvdb::Series> ) ),
             SLOT(slotRequestDone(int, QList<Tvdb::Series>)) );
    m_seriesCacheId = parent->seriesCache()->querySeries( name );
}

bool Nepomuk::TvdbReply::isValid() const
{
    return true;
}

void Nepomuk::TvdbReply::abort()
{
    kError() <<  "ATTENTION: Not implemented";
}


void Nepomuk::TvdbReply::slotRequestDone( int id, const QList<Tvdb::Series>& results )
{
    if ( id == m_seriesCacheId ) {
        kDebug();
        Q_FOREACH( const Tvdb::Series& series, results ) {
            // 1. make sure this series actually has an episode that matches our values
            if ( series.numSeasons() < m_season ||
                 series[m_season].numEpisodes() < m_episode ) {
                kDebug() << "Ignoring" << series.name() << "since it does not contain the requested episode";
                continue;
            }

            kDebug() << "Creating decision for" << series.name();

            // 2. create the new decision
            WebExtractor::Decision d = newDecision();
            WebExtractor::PropertiesGroup g = d.newGroup();

            // 3. create the series resource
            TVSeries tvSeries( QUrl(), g.manager() );
            tvSeries.setLabel( series.name() );
            tvSeries.setDescription( series.overview() );

            // 4. add the data to the file resource: the TVShow type and the rest
            TVShow proxyRes( d.proxyUrl(resource()), g.manager() );
            proxyRes.addType( QUrl( TVShow::resourceTypeUri() ) );
            proxyRes.setSeries( tvSeries );
            tvSeries.addEpisode( proxyRes );
            proxyRes.setEpisodeNumber( m_episode );
            proxyRes.setSeason( m_season );
            proxyRes.setSynopsis( series[m_season][m_episode].overview() );

            // 5. calculate the probability of the match the dumb way
            d.setRank( TvdbPlugin::calculateRankTheDumbWay( m_name, series.name() ) );

            // 6. Set a user readable desciption
            d.setDescription(i18nc("@info %1 is a file name, %2 and %3 are numbers, %4 is the name of a TV show",
                                   "%1 is episode %2 from season %3 of %4",
                                   resource().genericLabel(),
                                   m_episode,
                                   m_season,
                                   series.name()));

            // 7. add the decision to the pool of applicable ones
            addDecision( d );
        }

        // we are done
        finish();
    }
}
