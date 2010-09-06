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
#include "settings_config.h"
#include <QDir> 
#include <QFileInfo> 
#include <KDebug> 
#include <KService> 
#include <KServiceTypeTrader> 
#include <KGlobal> 
#include <kstandarddirs.h>

Nepomuk::CategoriesPool::CategoriesPool()
{
    update();
    foreach(const QString & dirName, KGlobal::dirs()->findDirs("config",CATEGORY_CONFIG_DIR))
    {
	kDebug() << "Watching dir "<< dirName;
	wc.addDir(dirName);
    }
    connect(&wc,SIGNAL(dirty(const QString &)),this,SLOT(update()));
}

void Nepomuk::CategoriesPool::update()
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

QSet<QString> Nepomuk::CategoriesPool::categories()
{
    return self()->m_categories;
}

void Nepomuk::CategoriesPool::EmitCatChanged()
{
    emit categoriesChanged();
}

void Nepomuk::CategoriesPool::addCategory(const QString & name)
{
    if ( self()->m_categories.contains(name))
	return;

    self()->m_categories << name;
    self()->EmitCatChanged();
}

Nepomuk::CategoriesPool* Nepomuk::CategoriesPool::self()
{
   static Nepomuk::CategoriesPool * m_self = new Nepomuk::CategoriesPool(); 
    return m_self; 
}

