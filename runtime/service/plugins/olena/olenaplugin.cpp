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

#include "olenaplugin.h"
#include "olenaplugin_config.h"
#include "olenadatapp.h"

#include <KPluginFactory>
#include <KDebug>


K_PLUGIN_FACTORY(OlenaPluginFactory, registerPlugin< Nepomuk::OlenaPlugin >();)
K_EXPORT_PLUGIN(OlenaPluginFactory("webexolenaplugin"))

Nepomuk::OlenaPlugin::OlenaPlugin(QObject* parent, const QList<QVariant>&):
    WebExtractorPlugin(parent)
{
}

int Nepomuk::OlenaPlugin::version()
{
    return (OLENA_PLUGIN_INTERNAL_VERSION);
}

Nepomuk::WebExtractor::DataPP * Nepomuk::OlenaPlugin::getDataPP(const QSharedPointer<KConfigBase> &  configFile)
{
    Q_UNUSED(configFile);
    return new OlenaDataPP(version());
}


