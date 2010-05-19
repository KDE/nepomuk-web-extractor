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
#include <webextractor/debug_datapp.h>
#include <webextractor/global.h>
#include <webextractor/parameters.h>

namespace NW = Nepomuk::WebExtractor;
Nepomuk::WebExtractorConfig::WebExtractorConfig():
    WebExConfigBase()
{
    update();
}
void Nepomuk::WebExtractorConfig::update()
{
    m_parameters.clear();
    foreach(WebExCategory * ctg, m_categories)
    {
	if (ctg)
	    delete ctg;
    }
    m_categories.clear();

    QStringList cats = WebExConfigBase::categories();
    foreach( const QString &  cat, cats)
    {
	// If it was not loaded before
	if (!m_categories.contains(cat)) {
	    kDebug() << "Loading category "<<cat;
	    WebExCategory * ctg = new WebExCategory(KSharedConfig::openConfig(QString("webextractor/categories/") + cat +"rc"));
	    // Get config and parse it
	    KConfig * catconfig = ctg->config();
	    QStringList groups = catconfig->groupList();
	    NW::ExtractParameters * p  = new NW::ExtractParameters;
	    for( 
		    QStringList::iterator it = groups.begin();
		    it != groups.end();
		    it++
	       )
	    {
		if (*it == "category") {
		    groups.erase(it);
		    break;
		}

	    }


	    foreach( const QString & pluginName, groups) 
	    {
		/*Load plugin with this name and parse it config*/
		NW::DataPP * dpp = 0 ;
		double rank;
		KConfigGroup pluginConfigGroup = catconfig->group(pluginName);


		if (pluginName == "debug" ) {
		    //This is predefined plugin
		    dpp = new NW::DebugDataPP();

		    rank = pluginConfigGroup.readEntry("coff",1.0);
		}
		else {
		    kDebug() << "Not realized yet";

		    // Save it to m_datapp;
		    // m_datapp.insert(dpp,
		    continue;
		}
		if (dpp)
		    p->addDataPP(dpp,rank);

	    }

	    Q_CHECK_PTR(ctg);
	    p->setUCrit(ctg->uCrit());
	    p->setACrit(ctg->aCrit());
	    p->setPluginSelectStep(ctg->pluginSelectStep());
	    NW::WE::LaunchPolitics pol;
	    switch ( ctg->pluginSelectType() )
	    {
		case (WebExCategory::EnumPluginSelectType::stepwise) : {pol = NW::WE::StepWise; break;} 
		case (WebExCategory::EnumPluginSelectType::all) : {pol = NW::WE::All; break;} 
	    }
	    p->setLaunchPolitics(pol);
	    p->setMergePolitics(NW::WE::Highest);
	    this->m_parameters.insert(cat,NW::ExtractParametersPtr(p));
	    this->m_categories[cat] = ctg;
	}
    }
}

int Nepomuk::WebExtractorConfig::maxPluginsLaunched( const QString & categoryName)
{

    int s =  max_plugins_launched_per_category();
	WebExCategory * c = m_categories[categoryName];
	Q_CHECK_PTR(c);
	if (c->pluginSelectType() == WebExCategory::EnumPluginSelectType::all)
	    if ( s)
		return s;
	    else
		return 0;
	else {
	    int s2 = c->pluginSelectStep();
	    return qMin(s,s2);
	}

}

int Nepomuk::WebExtractorConfig::maxResSimult( const QString & categoryName)
{

    int s =  maxResSimultPerCategory();
    WebExCategory * c = m_categories[categoryName];
    Q_CHECK_PTR(c);
    int s2 = c->maxResSimult();
    return qMin(s,s2);

}

int Nepomuk::WebExtractorConfig::interval( const QString & categoryName)
{

    /*
    int s =  maxResSimultPerCategory();
    if ( s)
	return s;
    else {
	WebExCategory * c = m_categories[categoryName];
	Q_CHECK_PTR(c);
	return c->maxResSimult();
    }*/
    WebExCategory * c = m_categories[categoryName];
    Q_CHECK_PTR(c);
    return c->interval();

}
QStringList Nepomuk::WebExtractorConfig::categories() const
{
    return m_categories.keys();
}

NW::ExtractParametersPtr  Nepomuk::WebExtractorConfig::extractParameters(const QString categoryName) const
{
    //return NW::ExtractParametersPtr(m_parameters[categoryName].data());
    if (!m_parameters.contains(categoryName)) {
	kDebug() << "No such category";
    }
    return m_parameters[categoryName];
    
}

#if 0
DataPPKeeper & Nepomuk::WebExtractorConfig::datapp( const QString categoryName)
{
    /*
    WebExCategory * c = m_categories[categoryName];
    Q_CHECK_PTR(c);
    return c->datapp();
    */
}
#endif

QString Nepomuk::WebExtractorConfig::query( const QString categoryName)
{
    WebExCategory * c = m_categories[categoryName];
    Q_CHECK_PTR(c);
    return c->query();
}

QString Nepomuk::WebExtractorConfig::queryPrefix( const QString categoryName)
{
    WebExCategory * c = m_categories[categoryName];
    Q_CHECK_PTR(c);
    return c->queryPrefix();
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
    QStringList cats = conf.categories();
    if (conf.m_categories.size() > 0) {
	dbg<<conf.m_categories.size()<<" Categories:";
	foreach( const QString & cat, cats)
	//foreach( WebExCategory* cat, conf.m_categories)
	{
	    dbg<<*(conf.m_parameters[cat]);
	    //dbg<<"wtf";
	    //dbg<<cat->query();
	}
    }
    else {
	dbg << "Config has no category enabled";
    }
    return dbg;
}

