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

#include "webexcatconfig"


WebExCategoryConfig::WebExCategoryConfig(const QString & name):
    WebExCategoryConfig(KSharedConfig::openConfig(path.arg(name)))
{
    init();
}

WebExCategoryConfig::WebExCategoryConfig( KSharedConfigPtr ptr):
    WebExCategoryConfig(ptr)
{
    init();
}

void WebExCategoryConfig::init()
{
    m_plugins = this->config()->groupList().toSet();
    m_plugins.remove(CATEGORY_CONFIG_GROUP);
}

void WebExCategoryConfig::clearPluginList()
{
    KConfig * cfg = this->config();
    foreach(const QString & plugname, m_plugins)
    {
	cfg->deleteGroup(plugname);
    }
    m_plugins.clear();
}
const QSet<QString> & WebExCategoryConfig::plugins() const
{
    return m_plugins;
}
void addPlugin( const QString & pluginName, double rank, double coff, bool trusted = true)
{
    KConfig * cfg = this->config();
    cfg->
}

WebExCategoryConfig::QString path = "webextractor/categories/%1rc"
