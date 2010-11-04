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

#include <Nepomuk/Query/Query>

class Nepomuk::CategoriesPool::Private
{
public:
    void reloadCategories();
    Category loadCategory( const QString& name, WebExCategoryConfig* config ) const;

    QHash<QString, Category> m_categories;

    CategoriesPool* q;
};


void Nepomuk::CategoriesPool::Private::reloadCategories()
{
    m_categories.clear();

    kDebug() << "Looking at: " << CATEGORY_CONFIG_DIR;
    const QStringList categoryFiles = KGlobal::dirs()->findAllResources("config",CATEGORY_CONFIG_DIR"/*rc");
    if ( categoryFiles.isEmpty() ) {
	kDebug() << "No category detected";
    }

    foreach( const QString & cat, categoryFiles) {
	QFileInfo info(cat);
	QString name = info.fileName();

        // FIXME: use the rc file path instead of the weirdly constructed name
	name.remove(name.size() - 2,2);
	if (!name.isEmpty()) {
	    kDebug() << "Load category " << name;
            WebExCategoryConfig cfg(name);
	    m_categories.insert( name, loadCategory(name, &cfg) );
	}
    }

    emit q->categoriesChanged();
}

Category Nepomuk::CategoriesPool::Private::loadCategory( const QString& name, Nepomuk::WebExCategoryConfig* config ) const
{
    Category cat;

    // get the basic category settings
    cat.setName(name);
    cat.setQuery( Nepomuk::Query::Query::fromString(config->queryText()));
    cat.setQueryDescription(config->description());
    cat.setInterval(config->interval());
    cat.setMaxResSimult(config->maxResSimult());
    cat.setPluginSelectionType(config->pluginSelectType() == 0 ? Category::Stepwise : Category::All);
    cat.setPluginSelectionStep(config->pluginSelectStep());
    cat.setUCrit(config->uCrit());

    // get the plugin configurations
    cat.setPlugins(config->plugins().values());

    return cat;
}


Nepomuk::CategoriesPool::CategoriesPool()
    : QObject(),
      d(new Private)
{
    d->q = this;

    d->reloadCategories();
    foreach(const QString & dirName, KGlobal::dirs()->findDirs("config",CATEGORY_CONFIG_DIR))
    {
	kDebug() << "Watching dir "<< dirName;
	KDirWatch::self()->addDir(dirName);
    }
    connect(KDirWatch::self(),SIGNAL(dirty(const QString &)),this,SLOT(reloadCategories()));
}

Nepomuk::CategoriesPool::~CategoriesPool()
{
    delete d;
}

QList<Category> Nepomuk::CategoriesPool::categories() const
{
    return d->m_categories.values();
}

void Nepomuk::CategoriesPool::addCategory(const QString & name)
{
//    if ( self()->m_categories.contains(name))
//	return;

//    self()->m_categories << name;
//    self()->EmitCatChanged();
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
        d->m_categories[cat.name()] = cat;
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
    d->reloadCategories();
}

void Nepomuk::CategoriesPool::saveCategories()
{
#warning IMPLEMENTME: saveCategories()
}

bool Nepomuk::CategoriesPool::removeCategory(const QString &name)
{
    if(d->m_categories.contains(name)) {
        d->m_categories.remove(name);
        return true;
    }
    else {
        kDebug() << "Could not find category with name" << name;
        return false;
    }
}

#include "categoriespool.moc"
