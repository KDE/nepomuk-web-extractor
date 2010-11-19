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

#include "categoriespool.h"
#include "category.h"
#include "settings_config.h"
#include <QDir> 
#include <QFileInfo> 
#include <KDebug> 
#include <KService> 
#include <KServiceTypeTrader> 
#include <KGlobal> 
#include <kstandarddirs.h>
#include <KDirWatch>
#include <QMap>
#include <KSharedConfig>

#include <Nepomuk/Query/Query>


class Nepomuk::CategoriesPool::Private
{
public:
    void _k_categoryChanged(Category*);

    QMap<QString, Category*> m_categories;
    bool m_autoUpdate;

    CategoriesPool* q;
};


void Nepomuk::CategoriesPool::Private::_k_categoryChanged(Category* cat)
{
    emit q->categoriesChanged();
}

Nepomuk::CategoriesPool::CategoriesPool()
    : QObject(),
      d(new Private)
{
    kDebug();
    d->q = this;
    d->m_autoUpdate = false;
    reloadCategories();
}

Nepomuk::CategoriesPool::~CategoriesPool()
{
    kDebug();
    delete d;
}

QList<Category*> Nepomuk::CategoriesPool::categories() const
{
    return d->m_categories.values();
}


K_GLOBAL_STATIC( Nepomuk::CategoriesPool, s_pool )

Nepomuk::CategoriesPool* Nepomuk::CategoriesPool::self()
{
   return s_pool;
}

Category* Nepomuk::CategoriesPool::categoryById(const QString &id) const
{
    if(d->m_categories.contains(id))
        return d->m_categories[id];
    else
        return 0;
}

bool Nepomuk::CategoriesPool::addCategory(Category *cat)
{
    if(cat->isValid()) {
        connect(cat, SIGNAL(changed(Category*)),
                this, SLOT(_k_categoryChanged(Category*)));
        d->m_categories[cat->identifer()] = cat;
        emit categoriesChanged();
        return true;
    }
    else {
        kDebug() << "Cannot save invalid Category";
        return false;
    }
}

void Nepomuk::CategoriesPool::reloadCategories()
{
    kDebug();
    // Idea for better config scheme:
    // - We have one .cat file for each category. In there the category settings are defined
    // - We have one .datapp file for each plugin/cat relation, containing the DataPPDescr settings and the plugin KCM settings
    // - There is a list of default global categories which can be used by plugin devs to have default configs for their plugins
    // - Plugin devels can install new categories
    // - The user can create new categories and relations
    // - categories can be disabled
    // - global Categories can be locally overwritten, ie. changed
    // - plugin/cat relations can be disabled
    // - global plugin/cat relations cannot be deleted but disabled
    //
    // Technical issues
    // - While we can use findAllResources to get all categories we need to use locate() on each of the files to let the config system merge global and local settings
    // - Naming scheme of the config files? The only real problem is renaming global stuff
    //

    qDeleteAll(d->m_categories);
    d->m_categories.clear();

    // load the categories
    // ==============================
    kDebug() << "Looking at: " << CATEGORY_CONFIG_DIR;
    // we are using the relative paths to allow local settings overriding the global ones
    QStringList categoryFiles;
    KGlobal::dirs()->findAllResources("config", CATEGORY_CONFIG_DIR"/*.cat", KStandardDirs::NoDuplicates, categoryFiles);
    if ( categoryFiles.isEmpty() ) {
	kDebug() << "No category detected";
    }
    Q_FOREACH(const QString& relativePath, categoryFiles) {
        const QString catConfigFile = KStandardDirs::locate("config", relativePath);
        if(QFile::exists(catConfigFile)) {
            KSharedConfig::Ptr config = KSharedConfig::openConfig(catConfigFile);
            // TODO: a local installation may be read-write, too
            Category* cat = new Category(config->accessMode() == KConfig::ReadOnly, this);
            cat->load(config->group("Category"));
            if(cat->isValid()) {
                kDebug() << cat->identifer() << "read only?" << cat->isGlobal();
                addCategory(cat);
            }
            else {
                delete cat;
                kDebug() << "Invalid category at" << catConfigFile;
            }
        }
    }

    // now load all datapps
    // ==============================
    QStringList datappFiles;
    KGlobal::dirs()->findAllResources("config", PLUGIN_CONFIG_DIR"/*.datapp", KStandardDirs::NoDuplicates, datappFiles);
    if ( datappFiles.isEmpty() ) {
	kDebug() << "No datapps detected";
    }
    Q_FOREACH(const QString& relativePath, datappFiles) {
        const QString datappConfigFile = KStandardDirs::locate("config", relativePath);
        if(QFile::exists(datappConfigFile)) {
            KSharedConfig::Ptr config = KSharedConfig::openConfig(datappConfigFile);
            KConfigGroup datappGroup = config->group("DataPP");
            DataPPDescr datapp = DataPPDescr::load(datappGroup, this);
            if( datapp.isValid() ) {
                datapp.category()->addPlugin(datapp);
            }
            else {
                kDebug() << "Could not match datapp" << datapp.identifier() << "to any category";
            }
        }
    }
}

void Nepomuk::CategoriesPool::saveCategories()
{
    // 1. delete all locally existing categories and plugins
    QDir localCatDir = KStandardDirs::locateLocal("config", QLatin1String(CATEGORY_CONFIG_DIR));
    Q_FOREACH(const QString& oldCatFile, localCatDir.entryList(QStringList(QLatin1String("*.cat")), QDir::Files)) {
        localCatDir.remove(oldCatFile);
    }
    Q_FOREACH(const QString& oldDataPPFile, localCatDir.entryList(QStringList(QLatin1String("*.datapp")), QDir::Files)) {
        localCatDir.remove(oldDataPPFile);
    }

    // 2. save all categories in the pool
    Q_FOREACH(Category* cat, d->m_categories) {
        KSharedConfig::Ptr config = KSharedConfig::openConfig(QString::fromLatin1(CATEGORY_CONFIG_DIR"%1.cat").arg(cat->identifer()));
        KConfigGroup catGroup = config->group("Category");
        cat->save(catGroup);

        // 3. Save all the DataPPs
        Q_FOREACH(const DataPPDescr& datapp, cat->plugins()) {
            const QString datappName = cat->identifer() + QLatin1String("_") + datapp.identifier();
            KSharedConfig::Ptr config = KSharedConfig::openConfig(QString::fromLatin1(PLUGIN_CONFIG_DIR"%1.datapp").arg(datappName));
            KConfigGroup datappGroup = config->group("DataPP");
            datapp.save(datappGroup);

            // TODO: load the plugin-specific configuration and provide API to change it
        }
    }
}

//bool Nepomuk::CategoriesPool::removeCategory(const QString &name)
//{
//    QMap<QString, Category*>::iterator it = d->m_categories.find(name);
//    if(it != d->m_categories.end()) {
//        if(it.value()->isGlobal()) {
//            kdDebug() << "Cannot remove global categories";
//            return false;
//        }
//        else {
//            d->m_categories.erase(it);
//            return true;
//        }
//    }
//    else {
//        kDebug() << "Could not find category with name" << name;
//        return false;
//    }
//}

void Nepomuk::CategoriesPool::setAutoUpdate(bool autoUpdate)
{
    if(autoUpdate != d->m_autoUpdate) {
        if(autoUpdate) {
            foreach(const QString & dirName, KGlobal::dirs()->findDirs("config", CATEGORY_CONFIG_DIR)) {
                kDebug() << "Watching dir "<< dirName;
                KDirWatch::self()->addDir(dirName);
            }
            connect(KDirWatch::self(), SIGNAL(dirty(const QString &)), this, SLOT(reloadCategories()));
        }
        else {
            KDirWatch::self()->disconnect(this);
        }
    }
}

KService::List Nepomuk::CategoriesPool::availablePlugins() const
{
    return KServiceTypeTrader::self()->query(QLatin1String("Nepomuk/WebExtractorPlugin"));
}

KService::Ptr Nepomuk::CategoriesPool::pluginByName(const QString &name) const
{
    KService::List offers = KServiceTypeTrader::self()->query(QLatin1String("Nepomuk/WebExtractorPlugin"));
    Q_FOREACH(KService::Ptr service, offers) {
        if(service->name() == name) {
            return service;
        }
    }
    return KService::Ptr();
}

#include "categoriespool.moc"
