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
#include "webexcatconfig.h"
#include <QDir> 
#include <QFileInfo> 
#include <KDebug> 
#include <KService> 
#include <KServiceTypeTrader> 
#include <KGlobal> 
#include <kstandarddirs.h>
#include <KDirWatch>
#include <QHash>
#include <KSharedConfig>

#include <Nepomuk/Query/Query>

class Nepomuk::CategoriesPool::Private
{
public:
    QHash<QString, Category> m_categories;
    bool m_autoUpdate;

    CategoriesPool* q;
};


Nepomuk::CategoriesPool::CategoriesPool()
    : QObject(),
      d(new Private)
{
    d->q = this;
    d->m_autoUpdate = false;
    reloadCategories();
}

Nepomuk::CategoriesPool::~CategoriesPool()
{
    delete d;
}

QList<Category> Nepomuk::CategoriesPool::categories() const
{
    return d->m_categories.values();
}


K_GLOBAL_STATIC( Nepomuk::CategoriesPool, s_pool )

Nepomuk::CategoriesPool* Nepomuk::CategoriesPool::self()
{
   return s_pool;
}

Category Nepomuk::CategoriesPool::category(const QString &name) const
{
    if(d->m_categories.contains(name))
        return d->m_categories[name];
    else
        return Category();
}

bool Nepomuk::CategoriesPool::addCategory(const Category &cat)
{
    if(cat.isValid()) {
        if(d->m_categories[cat.name()].isGlobal()) {
            kDebug() << "Cannot overwrite global categories.";
            return false;
        }
        else {
            d->m_categories[cat.name()] = cat;
            emit categoriesChanged();
            return true;
        }
    }
    else {
        kDebug() << "Cannot save invalid Category";
        return false;
    }
}

void Nepomuk::CategoriesPool::reloadCategories()
{
    d->m_categories.clear();

    kDebug() << "Looking at: " << CATEGORY_CONFIG_DIR;
    const QStringList categoryFiles = KGlobal::dirs()->findAllResources("config", CATEGORY_CONFIG_DIR"/*rc");
    if ( categoryFiles.isEmpty() ) {
	kDebug() << "No category detected";
    }
    Q_FOREACH(const QString& catFile, categoryFiles) {
        Category cat = Category::load(KSharedConfig::openConfig(catFile));
        if(cat.isValid()) {
            kDebug() << cat.name() << "read only?" << cat.isGlobal();
            d->m_categories.insert(cat.name(), cat);
        }
        else {
            kDebug() << "Invalid category at" << catFile;
        }
    }
}

void Nepomuk::CategoriesPool::saveCategories()
{
    // 1. delete all locally existing categories
    QDir localCatDir = KStandardDirs::locateLocal("config", QLatin1String(CATEGORY_CONFIG_DIR));
    Q_FOREACH(const QString& oldCatFile, localCatDir.entryList(QStringList(QLatin1String("*rc")), QDir::Files)) {
        localCatDir.remove(oldCatFile);
    }

    // 2. save all categories in the pool
    Q_FOREACH(const Category& cat, d->m_categories) {
        if(!cat.isGlobal()) {
            KSharedConfig::Ptr config = KSharedConfig::openConfig(QString::fromLatin1(CATEGORY_CONFIG_DIR"%1rc").arg(cat.name().toLower().replace(QChar(' '), QChar('_'))));
            cat.save(config);
        }
    }
}

bool Nepomuk::CategoriesPool::removeCategory(const QString &name)
{
    QHash<QString, Category>::iterator it = d->m_categories.find(name);
    if(it != d->m_categories.end()) {
        if(it.value().isGlobal()) {
            kdDebug() << "Cannot remove global categories";
            return false;
        }
        else {
            d->m_categories.erase(it);
            return true;
        }
    }
    else {
        kDebug() << "Could not find category with name" << name;
        return false;
    }
}

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

#include "categoriespool.moc"
