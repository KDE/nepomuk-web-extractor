/*
   Copyright (C) 2010 by Artem Serebriyskiy <v.for.vandal@gmail.com>

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

#include "testplugin.h"

#include "testplugin_config.h"
#include "testdatapp.h"

#include <KPluginFactory>
#include <KDebug>


K_PLUGIN_FACTORY(TestPluginFactory, registerPlugin< Nepomuk::TestPlugin >();)
K_EXPORT_PLUGIN(TestPluginFactory("webextestplugin"))

Nepomuk::TestPlugin::TestPlugin(QObject* parent, const QList<QVariant>&):
    WebExtractorPlugin(parent)
{
    ;
}

int Nepomuk::TestPlugin::version()
{
    return (TEST_PLUGIN_INTERNAL_VERSION);
}

Nepomuk::WebExtractor::DataPP * Nepomuk::TestPlugin::getExecutive(const QSharedPointer<KConfigBase> & configFile)
{
    return new TestDataPP(version());
}


