#include "debug_plugin.h"
#include "debug_plugin_config.h"
#include "simple_debug_reply.h"
#include <webextractor/simple_reply_factory.h>
#include <KPluginFactory>

K_PLUGIN_FACTORY( DebugPluginFactory, registerPlugin< Nepomuk::DebugPlugin >(); ) 
	  K_EXPORT_PLUGIN( DebugPluginFactory("webexdebugplugin") )
	    
Nepomuk::DebugPlugin::DebugPlugin(QObject* parent, const QList<QVariant>&):
    WebExtractorPlugin(parent)
{;}


Nepomuk::WebExtractor::DataPP* Nepomuk::DebugPlugin::getDataPP( KSharedConfigPtr configFile)
{
    return m_dataPP;
}

QString Nepomuk::DebugPlugin::version() 
{
    static QString v = QString(DEBUG_PLUGIN_VERSION);
    return v;
}

Nepomuk::WebExtractor::SimpleDataPP * Nepomuk::DebugPlugin::m_dataPP = new Nepomuk::WebExtractor::SimpleDataPP(
	Nepomuk::DebugPlugin::version(),
	"http://www.example.org/",
	new Nepomuk::WebExtractor::SimpleReplyFactoryTemplate<SimpleDebugReply>()
	);
