#include "plugins.h"
#include "kcm_config.h"
#include <QDir> 
#include <QFileInfo> 
#include <KDebug> 
#include <KService> 
#include <KServiceTypeTrader> 
#include <KGlobal> 
#include <kstandarddirs.h>

Plugins::Plugins()
{
    init();
}

void Plugins::init()
{
    /*
    KService::List services;
    KServiceTypeTrader* trader = KServiceTypeTrader::self();
     
    services = trader->query("WebExtractor/DataPP");
    foreach (KService::Ptr service, services) {
	    kDebug() << "read datapp" << service->name();
    }
    */
    /*
    QDir myDir(CONFIG_DIR);
    kDebug() << "Open config dir: "<<CONFIG_DIR;
    QStringList filters;
    filters.push_back(QString("*rc"));
    QStringList list = myDir.entryList(filters, QDir::Files);
    */
    QStringList list = KGlobal::dirs()->findAllResources("config","webextractor/plugins/*rc");
    foreach( const QString & cat, list)
    {
	QFileInfo info(cat);
	QString name = info.fileName();
	
	name.remove(name.size() - 2,2);
	if (!name.isEmpty())
	    m_plugins.push_back(name);
    }

    //m_init = true;
}

const QStringList & Plugins::plugins()
{
    return self()->m_plugins;
}


Plugins *  Plugins::m_self = new Plugins();
