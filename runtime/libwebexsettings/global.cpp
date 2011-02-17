/*
   Copyright (C) 2010 by Serebriyskiy Artem <v.for.vandal@gmail.com>

   This library is free software; you can redistribute it and/or modify
   it under the terms of the GNU Library General Public License as published by
   the Free Software Foundation; either version 3, or (at your option)
   any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#include "global.h"
#include <KServiceTypeTrader>
#include <KService>
#include "settings_config.h"
#include <KDebug>


QString Nepomuk::GlobalSettings::pluginServiceType()
{
    static QString _t = QString(WE_PLUGIN_SERVICE_TYPE);
    return _t;
}

QString Nepomuk::GlobalSettings::pluginQueryByName(const QString & name)
{
    return pluginQueryTemplate().arg(name);
}

QString Nepomuk::GlobalSettings::kcmQueryByName(const QString & name)
{
    static QString _t = QString("( ( [X-KDE-ParentApp] == 'webextractorconfig' ) and (["WE_PLUGIN_NAME_KEY"] == '%1') ) ");
    return _t.arg(name);
}

QString Nepomuk::GlobalSettings::pluginQueryTemplate()
{
    static QString _t = QString("(["WE_PLUGIN_NAME_KEY"] == '%1')");
    return _t;
}

Nepomuk::WebExtractorPlugin * Nepomuk::GlobalSettings::plugin( const QString & name)
{
    // FIXME Protect m_plugins() with lock
    //FIXME Replace contains() with find()
	// If plugin already loaded
	if (!m_plugins().contains(name)) {
	    // Load plugin

	    KService::List offers = KServiceTypeTrader::self()->query(pluginServiceType(),pluginQueryByName(name));
	    if (offers.begin() == offers.end() ) {
		//TODO May be it is necessary to put NULL to m_plugins to remeber that 
		//this plugin failed to load
		kError() << "Plugin "<<name<< " was not found. All it's DppExecutive will be ignored";
		return 0;
	    }


	    QString error;
	    KService::Ptr service = *(offers.begin());
	    KPluginFactory *factory = KPluginLoader(service->library()).factory();
	    if (!factory) {
		//KMessageBox::error(0, i18n("<html><p>KPluginFactory could not load the plugin:<br/><i>%1</i></p></html>",
		  //                         service->library()));
		kError(5001) << "KPluginFactory could not load the plugin:" << service->library();
		return 0;
	    }

	    WebExtractorPlugin *plugin = factory->create<WebExtractorPlugin>();

	    if (plugin) {
	       kDebug() << "Load plugin:" << service->name();
	       m_plugins().insert(name,plugin);
	    } else {
	       kDebug() << "Some error when loading plugin";
	       return 0;
	    }

	    // clear
	    delete factory;
	}

	return m_plugins()[name];
	
}

Nepomuk::WebExtractorPluginKCM * Nepomuk::GlobalSettings::kcm( const QString & name)
{
    // FIXME Protect m_plugins() with lock
    //FIXME Replace contains() with find()
	// If plugin already loaded
	if (!m_plugins().contains(name)) {
	    // Load plugin

	    KService::List offers = KServiceTypeTrader::self()->query("KCModule",kcmQueryByName(name));
	    if (offers.begin() == offers.end() ) {
		// This mean that this plugin doesn't provide KCM.
		// Write NULL to avoid further attempts
		m_kcms().insert(name,0);
		return 0;
	    }


	    QString error;
	    KService::Ptr service = *(offers.begin());
	    KPluginFactory *factory = KPluginLoader(service->library()).factory();
	    if (!factory) {
		//KMessageBox::error(0, i18n("<html><p>KPluginFactory could not load the plugin:<br/><i>%1</i></p></html>",
		  //                         service->library()));
		kError(5001) << "KPluginFactory could not load the plugin:" << service->library();
		return 0;
	    }

	    WebExtractorPluginKCM *kcm = factory->create<WebExtractorPluginKCM>();

	    if (kcm) {
	       kDebug() << "Load kcm:" << service->name();
	       m_kcms().insert(name,kcm);
	    } else {
	       kDebug() << "Some error when loading kcm";
	       return 0;
	    }

	    // clear
	    delete factory;
	}

	return m_kcms()[name];
	
}

QHash< QString, Nepomuk::WebExtractorPlugin*> & Nepomuk::GlobalSettings::m_plugins()
{
    static QHash< QString, WebExtractorPlugin*> m_p;
    return m_p; 
}


QHash< QString, Nepomuk::WebExtractorPluginKCM*> & Nepomuk::GlobalSettings::m_kcms()
{
    static QHash< QString, WebExtractorPluginKCM*> m_k;
    return m_k; 
}

int Nepomuk::GlobalSettings::pluginCount()
{
    return m_plugins().size();
}
