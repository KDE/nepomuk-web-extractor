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
		kError() << "Plugin "<<name<< " was not found. All it's DataPP will be ignored";
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


QHash< QString, Nepomuk::WebExtractorPlugin*> & Nepomuk::GlobalSettings::m_plugins()
{
    static QHash< QString, WebExtractorPlugin*> m_p;
    return m_p; 
}


int Nepomuk::GlobalSettings::pluginCount()
{
    return m_plugins().size();
}