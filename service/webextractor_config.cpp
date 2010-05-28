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
#include "webextractor_config.h"
#include <QtCore/QString>
#include <KDebug>
#include <QtGlobal>

namespace NW = Nepomuk::WebExtractor;
Nepomuk::WebExtractorConfig::WebExtractorConfig():
    WebExConfigBase()
{
    update();
}

Nepomuk::WebExtractorConfig::~WebExtractorConfig()
{
    foreach(WebExCategory * ctg, m_categories)
    {
	if (ctg)
	    delete ctg;
    }
    m_categories.clear();
    foreach(WebExtractorPlugin * plg, m_plugins)
    {
	if (plg)
	    delete plg;
    }
    m_plugins.clear();
}


void Nepomuk::WebExtractorConfig::clear()
{


    /*
    foreach(WebExCategory * ctg, m_categories)
    {
	if (ctg)
	    delete ctg;
    }
    m_categories.clear();
    */
}

QStringList Nepomuk::WebExtractorConfig::categories() const
{
    return m_categories.keys();
}

void Nepomuk::WebExtractorConfig::update()
{

    clear();

    QStringList cats = WebExConfigBase::categories();
    foreach( const QString &  cat, cats)
    {
	// If it was not loaded before
	// If it was not loaded before
	if (!m_categories.contains(cat)) {
	    kDebug() << "Loading category "<<cat;
	    WebExCategoryConfig * ctg = new WebExCategoryConfig(cat);
	    this->m_categories[cat] = ctg;
	}
    }
}

Nepomuk::WebExCategoryConfig * Nepomuk::WebExtractorConfig::categoryConfig(const QString & categoryName) const
{
    if (m_categories.contains(categoryName))
	return m_categories[categoryName];
    else {
	kDebug() << "No such category "<<categoryName;
	return 0;
    }
}


QDebug Nepomuk::operator<<( QDebug dbg,  const WebExtractorConfig & conf)
{
    QStringList cats = conf.categories();
    if (conf.m_categories.size() > 0) {
	dbg<<conf.m_categories.size()<<" Categories:";
    }
    else {
	dbg << "Config has no category enabled";
    }
    return dbg;
}

