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
#include "webextractor_settings.h"
#include <QtCore/QString>
#include <KDebug>
#include <QtGlobal>

Nepomuk::WebExtractorConfig::WebExtractorConfig():
    WebExConfigBase()
{
    QStringList cats = WebExConfigBase::categories();
    foreach( QString cat, cats)
    {
	if (!m_categories.contains(cat)) {
	    kDebug() << "Loading category "<<cat;
	    this->m_categories[cat] = new WebExCategory(KSharedConfig::openConfig(QString("webextractor/categories/") + cat +"rc"));
	}
    }
}

int Nepomuk::WebExtractorConfig::maxPluginsLaunched( const QString & categoryName)
{

    int s =  max_plugins_launched_per_category();
    if ( s)
	return s;
    else {
	WebExCategory * c = m_categories[categoryName];
	Q_CHECK_PTR(c);
	if (c->pluginSelectType() == WebExCategory::EnumPluginSelectType::all)
	    return 0;
	else {
	    return c->pluginSelectStep();
	}
    }

}

int Nepomuk::WebExtractorConfig::maxResSimult( const QString & categoryName)
{

    int s =  maxResSimultPerCategory();
    if ( s)
	return s;
    else {
	WebExCategory * c = m_categories[categoryName];
	Q_CHECK_PTR(c);
	return c->maxResSimult();
    }

}

QDebug Nepomuk::operator<<( QDebug dbg,  const WebExCategory & cat)
{
    dbg<<"Description: "<<cat.description();
    dbg<<"Query: "<<cat.query();
    dbg<<"Update interval: "<<cat.interval();
    return dbg;
}

QDebug Nepomuk::operator<<( QDebug dbg,  const WebExtractorConfig & conf)
{
    //QStringList cats = conf->categories();
    if (conf.m_categories.size() > 0) {
	dbg<<conf.m_categories.size()<<" Categories:";
	//foreach( QString cat, cats)
	foreach( WebExCategory* cat, conf.m_categories)
	{
	    dbg<<*cat;
	    //dbg<<"wtf";
	    //dbg<<cat->query();
	}
    }
    else {
	dbg << "Config has no category enabled";
    }
    return dbg;
}

