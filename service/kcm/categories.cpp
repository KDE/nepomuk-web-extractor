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
    update();
    foreach(const QString & dirName, KGlobal::dirs()->findDirs("config",CATEGORY_CONFIG_DIR))
    {
	kDebug() << "Watching dir "<< dirName;
	wc.addDir(dirName);
    }
    connect(&wc,SIGNAL(dirty(const QString &)),this,SLOT(update()));
}

void Categories::update()
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
    kDebug() << "Looking at: " << CATEGORY_CONFIG_DIR;
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

    m_categories = cats;

    emit categoriesChanged();

    //m_init = true;
}

const QSet<QString> & Categories::categories()
{
    return self()->m_categories;
}

void Categories::EmitCatChanged()
{
    emit categoriesChanged();
}

void Categories::addCategory(const QString & name)
{
    if ( self()->m_categories.contains(name))
	return;

    self()->m_categories << name;
    self()->EmitCatChanged();
}

Categories *  Categories::m_self = new Categories();
