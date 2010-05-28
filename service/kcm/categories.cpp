#include "categories.h"
#include "settings_config.h"
#include <QDir> 
#include <QFileInfo> 
#include <KDebug> 
#include <KService> 
#include <KServiceTypeTrader> 
#include <KGlobal> 
#include <kstandarddirs.h>

Categories::Categories()
{
    init();
}

void Categories::init()
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
    kDebug() << "Looking at: "CATEGORY_CONFIG_DIR;
    QStringList list = KGlobal::dirs()->findAllResources("config",CATEGORY_CONFIG_DIR"/*rc");
    if ( list.isEmpty() ) {
	kDebug() << "No category detected";
	return;
    }
    QSet < QString > cats;
    foreach( const QString & cat, list)
    {
	QFileInfo info(cat);
	QString name = info.fileName();
	
	name.remove(name.size() - 2,2);
	if (!name.isEmpty()) {
	    cats.insert(name);
	    kDebug() << "Load category " << name;
	}
    }

    m_categories = cats.toList();

    //m_init = true;
}

const QStringList & Categories::categories()
{
    return self()->m_categories;
}


Categories *  Categories::m_self = new Categories();
