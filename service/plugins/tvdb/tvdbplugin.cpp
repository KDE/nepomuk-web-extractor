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

#include "tvdbplugin.h"
#include "tvdbplugin_config.h"
#include "tvdbdatapp.h"

#include <KPluginFactory>
#include <KDebug>

#include <tvdb/series.h>

Q_DECLARE_METATYPE(QList<Tvdb::Series>)

Nepomuk::TvdbPlugin::TvdbPlugin(QObject* parent, const QList<QVariant>&):
    WebExtractorPlugin(parent)
{
    // apparently DataPP and DataPPReply can live in different threads.
    qRegisterMetaType<QList<Tvdb::Series> >();
}


float Nepomuk::TvdbPlugin::version()
{
    return (TVDB_PLUGIN_VERSION);
}


Nepomuk::WebExtractor::DataPP* Nepomuk::TvdbPlugin::getDataPP(KSharedConfigPtr configFile)
{
    return new TvdbDataPP(version());
}


double Nepomuk::TvdbPlugin::calculateRankTheDumbWay( const QString& queryString, const QString& name )
{
    return double( name.length() - queryString.length() ) / double( name.length() );
}

K_PLUGIN_FACTORY(TvdbPluginFactory, registerPlugin< Nepomuk::TvdbPlugin >();)
K_EXPORT_PLUGIN(TvdbPluginFactory("webextvdbplugin"))
