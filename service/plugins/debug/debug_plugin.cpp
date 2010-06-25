/*
   Copyright (C) 2010 by Serebriyskiy Artem <v.for.vandal at gmail.com>

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

#include "debug_plugin.h"
#include "debug_plugin_config.h"
#include "simple_debug_reply.h"
#include <webextractor/simplenetworkdatapp.h>
#include <webextractor/simplenetworkreplyfactory.h>
#include <KPluginFactory>

K_PLUGIN_FACTORY(DebugPluginFactory, registerPlugin< Nepomuk::DebugPlugin >();)
K_EXPORT_PLUGIN(DebugPluginFactory("webexdebugplugin"))

Nepomuk::DebugPlugin::DebugPlugin(QObject* parent, const QList<QVariant>&):
    WebExtractorPlugin(parent)
{
    ;
}


Nepomuk::WebExtractor::DataPP* Nepomuk::DebugPlugin::getDataPP(KSharedConfigPtr configFile)
{
    static WebExtractor::SimpleNetworkDataPP * m_dataPP = new Nepomuk::WebExtractor::SimpleNetworkDataPP(
        Nepomuk::DebugPlugin::version(),
        "http://www.example.org/",
        new Nepomuk::WebExtractor::SimpleReplyFactoryTemplate<SimpleDebugReply>()
    );
    return m_dataPP;
}

QString Nepomuk::DebugPlugin::version()
{
    static QString v = QString(DEBUG_PLUGIN_VERSION);
    return v;
}

