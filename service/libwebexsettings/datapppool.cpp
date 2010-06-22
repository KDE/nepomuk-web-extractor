#include "datapppool.h"
#include "datappconfig.h"
#include "settings_config.h"
#include <QDir> 
#include <QFileInfo> 
#include <KDebug> 
#include <KService> 
#include <KServiceTypeTrader> 
#include <KGlobal> 
#include <kstandarddirs.h>

Nepomuk::DataPPPool::DataPPPool()
{
    init();
}

void Nepomuk::DataPPPool::init()
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
    QStringList list = KGlobal::dirs()->findAllResources("config",PLUGIN_CONFIG_DIR"/*rc");
    foreach( const QString & plg, list)
    {
	QFileInfo info(plg);
	QString filename = info.fileName();
	QString name = filename;
	
	name.remove(name.size() - 2,2);
	if (!name.isEmpty())
	    m_plugins.push_back(name);


	// Open it's config to read file
	DataPPConfig * dppcfg = new DataPPConfig(name);
	// Take categories 
	foreach( const QString & categoryName, dppcfg->categories())
	{
	    m_categoryPlugins[categoryName].insert(name);
	}

	// Take source plugin
	m_dataPPSources[name] = dppcfg->plugin();

	// Remove config object
	delete dppcfg;
	
    }

    //m_init = true;
}

QStringList Nepomuk::DataPPPool::plugins()
{
    return self()->m_plugins;
}

QSet< QString >  Nepomuk::DataPPPool::categoryDataPPs(const QString & categoryName) 
{
    return self()->m_categoryPlugins[categoryName];
}

#if 0
void Nepomuk::DataPPPool::addDataPP(const QString & name, const QString & sourcePlugin)
{
    if ( self()->m_plugins.contains(name) )
	return;

    DataPPConfig * dppcfg = new DataPPConfig(name);

    // If DataPP already exists
    if (dppcfg->plugin().size()) {
	// Clear it config file
    self()->m_plugins << 
}
QString Nepomuk::DataPPPool::dataPPSource(const QString & name)
{
    if (!self()->m_dataPPSources.contains(name)) {
	kDebug() << "No such DataPP: "<<name ;
	return QString();
    }
    else {
	return self()->m_dataPPSources[name];
    }
}

KSharedConfigPtr Nepomuk::DataPPPool::dataPPConfig(const QString & name)
{
    return KSharedConfig::openConfig()
}
#endif

Nepomuk::DataPPPool * Nepomuk::DataPPPool::self()
{
    static DataPPPool *  m_self = new DataPPPool();
    return m_self;
}


QDebug Nepomuk::operator<<( QDebug dbg,  const DataPPPool & pool)
{
    dbg << " Pool of all DataPP installed in system/user "<<'\n';
    foreach( const QString & plg, pool.m_plugins )
    {
	dbg << plg << /*" source: " << pool.m_dataPPSources[plg]<<*/'\n';
    }

    return dbg;
}
