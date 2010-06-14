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
#include <KService>
#include <KServiceTypeTrader>
#include <KPluginFactory>
#include <KPluginLoader>
#include <QtGlobal>
#include <webextractor/debug_datapp.h>
#include <webextractor/datapp.h>
#include <webextractor/datappwrapper.h>
#include <webextractor/global.h>
#include <webextractor/parameters.h>
#include "datapppool.h"

namespace NW = Nepomuk::WebExtractor;
Nepomuk::WebExtractorSettings::WebExtractorSettings():
    WebExtractorConfig()
{
    update();
}

Nepomuk::WebExtractorSettings::~WebExtractorSettings()
{
    clear();
}

void Nepomuk::WebExtractorSettings::clear()
{
    // Free all dataPP
    foreach( NW::DataPP * pp, m_datapp)
    {
	delete pp;
    }
    m_datapp.clear();

    // Free all DataPPWrappers
    foreach ( NW::ExtractParametersPtr ptr, m_parameters)
    {
	const NW::DataPPKeeper &  plugins = ptr->plugins();
	foreach( NW::DataPPWrapper * dppw, plugins)
	{
	    delete dppw;
	}
    }
    m_parameters.clear();

    m_plugins.clear();

}

void Nepomuk::WebExtractorSettings::update()
{

    clear();

    WebExtractorConfig::update();

    //

    QStringList cats = WebExConfigBase::categories();
    foreach( const QString &  cat, cats)
    {
	WebExCategoryConfig * cfg = m_categories[cat];

	NW::ExtractParameters * p  = new NW::ExtractParameters;


	foreach( const DataPPDescr & dppdescr, cfg->plugins()) 
	{
	    /*Load plugin with this name and parse it config*/
	    NW::DataPP * dpp = 0 ;
	    NW::DataPPWrapper * dppw = 0 ;
	    double rank = dppdescr.rank;
	    double coff = dppdescr.coff;
	    const QString & dataPPName = dppdescr.name;

	    // If plugin(datapp) with this name exist then skip loading
	    if (m_datapp.contains(dataPPName) ) {
		dpp = m_datapp[dataPPName];
	    }
	    else {
		if (dataPPName == "debug" ) {
		    //This is predefined plugin
		    dpp = new NW::DebugDataPP();
		    m_datapp.insert("debug",dpp);
		}
		else {
		    DataPPConfig * dppcfg = new DataPPConfig(dataPPName);
		    QString sourceName = dppcfg->plugin();
		    if (sourceName.isEmpty()) {
			kError() << "DataPP "<<dataPPName<<" has no source plugin set.Ignoring";
			continue;
		    }

		    
		    // Get source plugin name from dataPP settings

		    // If plugin already loaded
		    if (!m_plugins.contains(sourceName)) {
			// Load plugin

			KService::List offers = KServiceTypeTrader::self()->query(pluginServiceType(),queryByName(sourceName));
			if (offers.begin() == offers.end() ) {
			    kError() << "Plugin "<<sourceName<< " was not found. All it's DataPP will be ignored";
			    continue;
			}


			QString error;
			KService::Ptr service = *(offers.begin());
			KPluginFactory *factory = KPluginLoader(service->library()).factory();
			if (!factory) {
			    //KMessageBox::error(0, i18n("<html><p>KPluginFactory could not load the plugin:<br/><i>%1</i></p></html>",
			      //                         service->library()));
			    kError(5001) << "KPluginFactory could not load the plugin:" << service->library();
			    continue;
			}

			WebExtractorPlugin *plugin = factory->create<WebExtractorPlugin>(this);

			if (plugin) {
			   kDebug() << "Load plugin:" << service->name();
			   m_plugins.insert(sourceName,plugin);
			} else {
			   kDebug() << "Some error when loading plugin";
			   continue;
			}

			// clear
			delete factory;
			delete dppcfg;

		    // Save it to m_datapp;
		    // m_datapp.insert(dpp,
		    }

		    // Create new DataPP
		    dpp = m_plugins[sourceName]->getDataPP(DataPPConfig::config(dataPPName));
		    if (dpp) {
			kDebug() << "Loaded DataPP "<<dataPPName << "version: "<<dpp->pluginVersion();
			m_datapp.insert(dataPPName,dpp);
		    }
		    else {
			kError() << "Error while generating DataPP "<<dataPPName;
		    }
		}
	    }

	    // Add DataPP to category parameters 
	    if (dpp) {
		dppw = new NW::DataPPWrapper(dpp,dataPPName, rank,coff);
		p->addDataPP(dppw);
		//m_datappwrappers.insert(pluginName,dppw);
	    }

	}

	Q_CHECK_PTR(cfg);
	p->setUCrit(cfg->uCrit());
	p->setACrit(cfg->aCrit());
	p->setPluginSelectStep(cfg->pluginSelectStep());
	NW::WE::LaunchPolitics pol;
	switch ( cfg->pluginSelectType() )
	{
	    case (WebExCategory::EnumPluginSelectType::stepwise) : {pol = NW::WE::StepWise; break;} 
	    case (WebExCategory::EnumPluginSelectType::all) : {pol = NW::WE::All; break;} 
	}
	p->setLaunchPolitics(pol);
	p->setMergePolitics(NW::WE::Highest);
	this->m_parameters.insert(cat,NW::ExtractParametersPtr(p));
    }
}

int Nepomuk::WebExtractorSettings::maxPluginsLaunched( const QString & categoryName)
{

    int s =  max_plugins_launched_per_category();
	WebExCategoryConfig * c = m_categories[categoryName];
	Q_CHECK_PTR(c);
	if (c->pluginSelectType() == WebExCategoryConfig::EnumPluginSelectType::all)
	    if ( s)
		return s;
	    else
		return 0;
	else {
	    int s2 = c->pluginSelectStep();
	    return qMin(s,s2);
	}

}

int Nepomuk::WebExtractorSettings::maxResSimult( const QString & categoryName)
{

    int s =  maxResSimultPerCategory();
    WebExCategoryConfig * c = m_categories[categoryName];
    Q_CHECK_PTR(c);
    int s2 = c->maxResSimult();
    return qMin(s,s2);

}

int Nepomuk::WebExtractorSettings::interval( const QString & categoryName)
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
    WebExCategoryConfig * c = m_categories[categoryName];
    Q_CHECK_PTR(c);
    return c->interval();

}

NW::ExtractParametersPtr  Nepomuk::WebExtractorSettings::extractParameters(const QString categoryName) const
{
    //return NW::ExtractParametersPtr(m_parameters[categoryName].data());
    if (!m_parameters.contains(categoryName)) {
	kDebug() << "No such category";
    }
    return m_parameters[categoryName];
    
}

#if 0
DataPPKeeper & Nepomuk::WebExtractorSettings::datapp( const QString categoryName)
{
    /*
    WebExCategory * c = m_categories[categoryName];
    Q_CHECK_PTR(c);
    return c->datapp();
    */
}
#endif

QString Nepomuk::WebExtractorSettings::query( const QString categoryName)
{
    WebExCategoryConfig * c = m_categories[categoryName];
    Q_CHECK_PTR(c);
    return c->query();
}

QString Nepomuk::WebExtractorSettings::queryPrefix( const QString categoryName)
{
    WebExCategoryConfig * c = m_categories[categoryName];
    Q_CHECK_PTR(c);
    return c->queryPrefix();
}


QDebug Nepomuk::operator<<( QDebug dbg,  const WebExtractorSettings & conf)
{
    dbg << static_cast<const WebExtractorConfig &>(conf);
    
#if 0
    QStringList cats = conf.categories();
    if (cats.size() > 0) {
	dbg<<cats.size()<<" Categories:";
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
#endif
    dbg << "Total plugins loaded: "<<conf.m_plugins.size() << '\n';
    dbg << "Total DataPP loaded: "<<conf.m_datapp.size() << '\n';
    if (conf.categories().size() > 0) {
	dbg << "Parameters per category" << '\n';
	for(
		QHash< QString, WebExtractor::ExtractParametersPtr >::const_iterator it = conf.m_parameters.begin();
		it != conf.m_parameters.end();
		it++
	   )
	{
	    dbg << "Category "<< it.key() << '\n';
	    dbg << *(it.value()) << '\n';
	}
    }
    return dbg;
}

