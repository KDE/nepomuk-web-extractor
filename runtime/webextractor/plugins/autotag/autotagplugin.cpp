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

#include "autotagplugin.h"
#include "autotagexecutive.h"

#include "autotagplugin_config.h"

#include <KPluginFactory>
#include <KSharedConfig>
#include <KConfigGroup>
#include <KDebug>


K_PLUGIN_FACTORY(AutotagPluginFactory, registerPlugin< Nepomuk::AutotagPlugin >();)
K_EXPORT_PLUGIN(AutotagPluginFactory("webexautotagplugin"))

Nepomuk::AutotagPlugin::AutotagPlugin(QObject* parent, const QList<QVariant>&):
    WebExtractorPlugin(parent)
{
    ;
}

int Nepomuk::AutotagPlugin::version()
{
    return (AUTOTAG_PLUGIN_INTERNAL_VERSION);
}

Nepomuk::WebExtractor::Executive * Nepomuk::AutotagPlugin::getExecutive(const KConfigGroup & configFile)
{
    // Parse config
    KConfigGroup grp = configFile.group(AUTOTAG_CONFIG_GROUP);

    QString regexpString = grp.readEntry(AUTOTAG_CONFIG_REGEXP_KEY, QString());

    QString tag = grp.readEntry(AUTOTAG_CONFIG_TAG_KEY, QString());

    kDebug() << "Read config file. Tag: " << tag << " Regexp: " << regexpString;

    QRegExp regexp(regexpString);

    return new AutotagExecutive(version(), regexp, tag);
}


