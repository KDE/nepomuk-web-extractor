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
#include "tvdbexecutive.h"

#include <KPluginFactory>
#include <KDebug>

#include <tvdb/series.h>

Q_DECLARE_METATYPE(QList<Tvdb::Series>)

Nepomuk::TvdbPlugin::TvdbPlugin(QObject* parent, const QList<QVariant>&)
    : WebExtractorPlugin(parent)
{
    // apparently Executive and ExecutiveReply can live in different threads.
    qRegisterMetaType<QList<Tvdb::Series> >();
}


int Nepomuk::TvdbPlugin::version()
{
    return (TVDB_PLUGIN_INTERNAL_VERSION);
}


Nepomuk::WebExtractor::Executive* Nepomuk::TvdbPlugin::getExecutive(const KConfigGroup & configFile)
{
    return new TvdbExecutive(version());
}


double Nepomuk::TvdbPlugin::calculateRankTheDumbWay(const QString& queryString, const QString& name)
{
    return 1.0 - double(name.length() - queryString.length()) / double(name.length());
}

K_PLUGIN_FACTORY(TvdbPluginFactory, registerPlugin< Nepomuk::TvdbPlugin >();)
K_EXPORT_PLUGIN(TvdbPluginFactory("webextvdbplugin"))
