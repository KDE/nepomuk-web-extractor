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
    WebExCategory(KSharedConfig::openConfig(path.arg(name))),
    m_name(name)
{
    init();
}

Nepomuk::WebExCategoryConfig::WebExCategoryConfig(KSharedConfigPtr ptr):
    WebExCategory(ptr),
    m_name(ptr->name())
{
    init();
}

void Nepomuk::WebExCategoryConfig::init()
{
    readPluginsList();
}

void Nepomuk::WebExCategoryConfig::readPluginsList()
{
    KConfigGroup pluginsGroup = this->config()->group(CATEGORY_PLUGINS_CONFIG_GROUP);
    QSet<QString> gset = pluginsGroup.groupList().toSet();
    //gset.remove(CATEGORY_CONFIG_GROUP);
    foreach(const QString & pluginName, gset) {
        KConfigGroup grp = pluginsGroup.group(pluginName);
        DataPPDescr dppd(pluginName);
        dppd.rank = grp.readEntry("rank", 1.0);
        dppd.coff = grp.readEntry("coff", 1.0);
        dppd.trusted = grp.readEntry("trusted", true);
        m_plugins.insert(pluginName, dppd);
        m_pluginsNames.insert(pluginName);
    }
}

void Nepomuk::WebExCategoryConfig::addPlugin(const DataPPDescr & dppd)
{
    KConfigGroup pluginsGroup = this->config()->group(CATEGORY_PLUGINS_CONFIG_GROUP);
    KConfigGroup grp = pluginsGroup.group(dppd.name);
    grp.writeEntry("rank", dppd.rank);
    grp.writeEntry("coff", dppd.coff);
    grp.writeEntry("trusted", dppd.trusted);
    m_plugins.insert(dppd.name, dppd);
    m_pluginsNames.insert(dppd.name);
}


void Nepomuk::WebExCategoryConfig::clearPluginList()
{
    KConfig * cfg = this->config();
    KConfigGroup pluginsGroup = cfg->group(CATEGORY_PLUGINS_CONFIG_GROUP);
    foreach(const DataPPDescr & dppd, m_plugins) {
        const QString & pluginName = dppd.name;
        pluginsGroup.deleteGroup(pluginName);
    }
    m_plugins.clear();
}

QMap<QString, DataPPDescr>  Nepomuk::WebExCategoryConfig::plugins() const
{
    return m_plugins;
}

QSet<QString> Nepomuk::WebExCategoryConfig::pluginsNames() const
{
    return m_pluginsNames;
}

void Nepomuk::WebExCategoryConfig::addPlugin(const QString & pluginName, double rank, double coff, bool trusted)
{
    DataPPDescr d(pluginName);
    d.rank = rank;
    d.coff = coff;
    d.trusted = trusted;
    addPlugin(d);
}

bool Nepomuk::WebExCategoryConfig::isOptimizedForNepomuk() const
{
#warning Implement this after support for per-categori examined info backend selection\
will be introduced
    return false;
}

QDebug Nepomuk::operator<<(QDebug dbg,  const WebExCategoryConfig & cat)
{
    dbg << "Category name(filename)" << cat.m_name << '\n';
    dbg << "Description: " << cat.description() << '\n';
    dbg << "Query: " << cat.queryText() << '\n';
    dbg << "Update interval: " << cat.interval() << '\n';
    dbg << "Plugins:(" << cat.m_pluginsNames.size() << ")" << '\n';
    foreach(const QString & plg, cat.m_pluginsNames) {
        const DataPPDescr descr = cat.m_plugins[plg];
        dbg << plg << ((descr.trusted) ? " [trusted]" : " [untrusted]") << " rank: " << descr.rank << " scale coff " << descr.coff << '\n';
    }
    return dbg;
}

QString Nepomuk::WebExCategoryConfig::path = CATEGORY_CONFIG_DIR"%1""rc";
