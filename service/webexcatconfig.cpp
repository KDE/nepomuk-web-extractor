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

#include "settings_config.h"
#include "webexcatconfig.h"


Nepomuk::WebExCategoryConfig::WebExCategoryConfig(const QString & name):
    WebExCategory(KSharedConfig::openConfig(path.arg(name)))
{
    init();
}

Nepomuk::WebExCategoryConfig::WebExCategoryConfig( KSharedConfigPtr ptr):
    WebExCategory(ptr)
{
    init();
}

void Nepomuk::WebExCategoryConfig::init()
{
    QSet<QString> gset = this->config()->groupList().toSet();
    gset.remove(CATEGORY_CONFIG_GROUP);
    foreach( const QString & pluginName, gset)
    {
	KConfigGroup grp = this->config()->group(pluginName);
	DataPPDescr dppd(pluginName);
	dppd.rank = grp.readEntry("rank", 1.0);
	dppd.coff = grp.readEntry("coff", 1.0);
	dppd.trusted = grp.readEntry("trusted",true);
	m_plugins.insert(pluginName,dppd); 
	m_pluginsNames.insert(pluginName);
    }
}

void Nepomuk::WebExCategoryConfig::clearPluginList()
{
    KConfig * cfg = this->config();
    foreach(const DataPPDescr & dppd, m_plugins)
    {
	const QString & pluginName = dppd.name;
	cfg->deleteGroup(pluginName);
    }
    m_plugins.clear();
}
const QMap<QString, DataPPDescr> & Nepomuk::WebExCategoryConfig::plugins() const
{
    return m_plugins;
}

const QSet<QString> & Nepomuk::WebExCategoryConfig::pluginsNames() const
{
    return m_pluginsNames;
}

void Nepomuk::WebExCategoryConfig::addPlugin( const QString & pluginName, double rank, double coff, bool trusted )
{
    DataPPDescr d(pluginName);
    d.rank = rank;
    d.coff = coff;
    d.trusted = trusted;
    addPlugin(d);
}

void Nepomuk::WebExCategoryConfig::addPlugin( const DataPPDescr & dppd)
{
	KConfigGroup grp = this->config()->group(dppd.name);
	grp.writeEntry("rank", dppd.rank);
	grp.writeEntry("coff", dppd.coff);
	grp.writeEntry("trusted", dppd.trusted);
	m_plugins.insert(dppd.name,dppd); 
	m_pluginsNames.insert(dppd.name);
}
QDebug Nepomuk::operator<<( QDebug dbg,  const WebExCategory & cat)
{
    dbg<<"Description: "<<cat.description();
    dbg<<"Query: "<<cat.query();
    dbg<<"Update interval: "<<cat.interval();
    return dbg;
}

QString Nepomuk::WebExCategoryConfig::path = "webextractor/categories/%1""rc";
