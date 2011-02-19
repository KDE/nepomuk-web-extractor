/*
   Copyright (C) 2010 by Serebriyskiy Artem <v.for.vandal at gmail.com>

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

#include "datapppool.h"
#include "webextractor_kcm.h"
#include "settings_config.h"
#include <QDir>
#include <QFileInfo>
#include <KDebug>
#include <KService>
#include <KServiceTypeTrader>
#include <KGlobal>
#include <QtDebug>
#include <QQueue>
#include <kstandarddirs.h>

using namespace Nepomuk;

class TreeItem
{
    public:
	TreeItem(const QString & name);
	~TreeItem();
	// Add datapp only if this datpp doesn't exist
	TreeItem *  addDataPP(const QString & systemName, const QString & displayName);
	// Add category only if this category doesn't exist
	// Return pointer to category
	TreeItem * addCategory(const QString & name);
	TreeItem * child(int row);
	TreeItem * categoryItem(const QString & name);
	TreeItem * datappItem(const QString & name);
	QList< TreeItem* > allDataPP() const;
	QSet< QString > allDataPPNames() const;
	TreeItem * parent();
	int childsCount();
	int row() const;
	bool isCategory() const;
	QString displayName() const;
	QString sysName() const;
    DataPP * datapp() const;
	void print(int displacments, QDebug & stream);

    private:
	// This is the system name. It can be used
	// for DataPPConfig and so on
	QString m_sysname;
	// Name of datapp or name of category.
	// This is human readable name and it may be used
	// for displaying
	QString  m_name;
	QString source;
	// This is the pointer to KCM. It is not unique -
	// another DataPP can have the same pointer
	// This pointer is load on-demand.
	// If there is no KCM for plugin, then pointer
	// is equal to NULL.
	QSharedPointer<Nepomuk::WebExtractorPluginKCM>  kcm;
	// If KCM was loaded to the kcm member, then loaded
	// is equal to true. It is necessary to distinguish between
	// cases when KCM was not loaded and there is no KCM.
	bool loaded;
	QList< TreeItem*> childs;
	QHash< QString, TreeItem *> m_datapps;
	QHash< QString, TreeItem *> categories;

	bool m_category;
    mutable DataPP * m_datapp;
    
	TreeItem * parentItem;

};
TreeItem::TreeItem(const QString & name):
    m_sysname(name),
    m_name(name),
    m_category(true),
    m_datapp(0),
    parentItem(0)
{
    ;
}

TreeItem::~TreeItem()
{
    foreach(TreeItem * item, childs) {
        delete item;
    }
}

TreeItem *  TreeItem::addDataPP(const QString & systemName, const QString & displayName)
{
    if(m_datapps.contains(systemName))
        return m_datapps[systemName];

    TreeItem * item = new TreeItem(systemName);
    item->m_category = false;
    this->childs << item;
    item->parentItem = this;
    item->m_sysname = systemName;
    item->m_name = displayName;
    m_datapps[systemName] = item;
    return item;
}

TreeItem *  TreeItem::addCategory(const QString & name)
{
    if(categories. contains(name))
        return categories[name];

    TreeItem * item = new TreeItem(name);
    item->m_category = true;
    this->childs << item;
    item->parentItem = this;
    item->m_sysname = QString();
    categories[name] = item;
    return item;
}

TreeItem *  TreeItem::categoryItem(const QString & name)
{
    if(categories. contains(name))
        return categories[name];
    return 0;
}


TreeItem *  TreeItem::datappItem(const QString & name)
{
    if(m_datapps.contains(name))
        return m_datapps[name];
    return 0;
}

DataPP * TreeItem::datapp() const
{
    if (m_datapp == 0) {
        m_datapp = DataPPPool::dataPPById(m_sysname);
    }
    return m_datapp;
}

QList< TreeItem* > TreeItem::allDataPP() const
{
    if(isCategory())
        return m_datapps.values();
    return QList< TreeItem*>();
}


QSet< QString > TreeItem::allDataPPNames() const
{
    if(isCategory())
        return m_datapps.keys().toSet();
    return QSet<QString>();
}


TreeItem* TreeItem::child(int row)
{
    if(row > childs.size())
        return 0;
    return childs[row];
}

TreeItem* TreeItem::parent()
{
    return parentItem;
}

int TreeItem::row() const
{
    if(parentItem)
        return parentItem->childs.indexOf(const_cast<TreeItem*>(this));

    return 0;
}

int TreeItem::childsCount()
{
    return childs.size();
}


bool TreeItem::isCategory() const
{
    return childs.size();
}

QString TreeItem::displayName() const
{
    return m_name;
}

QString TreeItem::sysName() const
{
    return m_sysname;
}

void TreeItem::print(int displacments, QDebug & stream)
{
    for(int i = 0; i < displacments; i++)
        stream << ' ';
    if(m_category) {
        stream << m_name << '|' << '+' << '\n';
    } else {
        stream << m_name << '\n';
    }
    // Print DataPP first
    foreach(TreeItem * item , m_datapps) {
        item->print(displacments + 4, stream);
    }
    foreach(TreeItem * item , categories) {
        item->print(displacments + 4, stream);
    }
}

Nepomuk::DataPPPool::DataPPPool(QObject * parent):
    QAbstractItemModel(parent)
{
    m_categoryPlugins = 0;
    update();
    // Ignore watching directory because it breaks config idealogy
    foreach(const QString & dirName, KGlobal::dirs()->findDirs("config", PLUGIN_CONFIG_DIR)) {
        kDebug() << "Watching dir " << dirName;
        //wc.addDir(dirName);
    }

    this->setSupportedDragActions(Qt::CopyAction);
    //connect(&wc,SIGNAL(dirty(const QString &)),this,SLOT(update()));
}

void Nepomuk::DataPPPool::update()
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
    delete m_categoryPlugins;
    m_categoryPlugins = new TreeItem("DataPP");
    QStringList list = KGlobal::dirs()->findAllResources("config", PLUGIN_CONFIG_DIR"/*.datapp");
    foreach(const QString & plg, list) {
        QFileInfo info(plg);
        QString filename = info.fileName();
        QString name = filename;

        name.remove(name.size() - 6, 6);
        if(!name.isEmpty()) {
            // Put name of the plugin into the list of all plugins
            m_availableDataPP.push_back(name);
        }


        // Open it's config to read file
        DataPP* dppcfg = new DataPP(name);
        // If it is valid, add it to the list of the valid plugins
        if ( dppcfg->isValid() ) {
            m_validDataPP.push_back(name);
        }
        // Take categories
        if(dppcfg->categories().isEmpty())   // DataPP is uncategorized
            m_categoryPlugins->addDataPP(name, dppcfg->displayName());
        else {
            foreach(const QString & categoryName, dppcfg->categories()) {
                // Split category into subcategories
                QStringList catpath = categoryName.split("/", QString::SkipEmptyParts);
                TreeItem * item = m_categoryPlugins;
                foreach(const QString & cat, catpath) {
                    item = item->addCategory(cat);
                }

                item->addDataPP(name,dppcfg->displayName());
            }
        }

        // Add source to the list of sources
        m_dataPPSources[name] = dppcfg->source();
        // Add display name source to the list of names
        m_displayNames[name] = dppcfg->displayName();
        // Remove config object
        delete dppcfg;

    }

    //m_init = true;
}

QStringList Nepomuk::DataPPPool::availableDataPP()
{
    return self()->m_availableDataPP;
}

QStringList Nepomuk::DataPPPool::validDataPP()
{
    return self()->m_validDataPP;
}

QSet< QString >  Nepomuk::DataPPPool::categoryDataPPs(const QString & categoryName)
{
    //return self()->m_categoryPlugins[categoryName];
    if(!categoryName.isEmpty()) {
        // Parse category
        QStringList catpath = categoryName.split('/');
        TreeItem * item = self()->m_categoryPlugins;
        foreach(const QString & cat, catpath) {
            item = item->categoryItem(cat);
            if(!item) {  // No such category
                kDebug() << "No such category: " << cat << "Full path: " << categoryName;
                return QSet<QString> ();
            }
        }
        return item->allDataPPNames();
    }
    else {
#warning IMPLEMENTME
    }

    return QSet<QString>();
}

#if 0
void Nepomuk::DataPPPool::addDataPP(const QString & name, const QString & sourcePlugin)
{
    if(self()->m_availableDataPP.contains(name))
        return;

    DataPPConfig * dppcfg = new DataPPConfig(name);

    // If DataPP already exists
    if(dppcfg->plugin().size()) {
        // Clear it config file
        self()->m_availableDataPP <<
                      }

    KSharedConfigPtr Nepomuk::DataPPPool::dataPPConfig(const QString & name) {
        return KSharedConfig::openConfig()
           }
       }
#endif

QString Nepomuk::DataPPPool::sourceById(const QString & id)
{
    QHash<QString,QString>::const_iterator fit = self()->m_dataPPSources.find(id);
    if(fit == self()->m_dataPPSources.end()) {
        kDebug() << "No such DataPP: " << id ;
        return QString();
    } else {
        return fit.value();
    }
}

QString Nepomuk::DataPPPool::displayNameById( const QString & id)
{
    QHash<QString,QString>::const_iterator fit = self()->m_displayNames.find(id);
    if(fit == self()->m_displayNames.end()) {
        kDebug() << "No such DataPP: " << id ;
        return QString();
    } else {
        return fit.value();
    }
}

DataPP * Nepomuk::DataPPPool::dataPPById(const QString & id)
{
    QHash< QString, DataPP*>::const_iterator fit = self()->m_dataPPHash.find(id);
    bool s = ( fit == self()->m_dataPPHash.end() );
    if ( s ) {
        DataPP * dpp = new DataPP(id);
        self()->m_dataPPHash[id] = dpp;
        return dpp;
    }
    return fit.value();
}

Nepomuk::DataPPPool * Nepomuk::DataPPPool::self()
{
    static DataPPPool *  m_self = new DataPPPool();
    return m_self;
}

int Nepomuk::DataPPPool::categoryCount()
{
    return self()->m_categoryPlugins->childsCount();
}

int Nepomuk::DataPPPool::availableDataPPCount()
{
    return self()->m_availableDataPP.size();
}

// Non-static methods of the class
// These are methods that realize  model functionality
//
QModelIndex Nepomuk::DataPPPool::index(int row, int column, const QModelIndex & parent) const
{
    //kDebug() << "Index " << row << ' ' << column << ' ' << parent;
    if(!hasIndex(row, column, parent))
        return QModelIndex();

    TreeItem * parentItem;
    if(parent.isValid())
        parentItem = static_cast<TreeItem*>(parent.internalPointer());
    else
        parentItem = m_categoryPlugins;

    TreeItem * childItem = parentItem->child(row);
    if(childItem)
        return createIndex(row, column, childItem);
    else
        return QModelIndex();
}

QModelIndex Nepomuk::DataPPPool::parent(const QModelIndex & index) const
{
    if(!index.isValid())
        return QModelIndex();

    TreeItem *childItem = static_cast<TreeItem*>(index.internalPointer());
    Q_ASSERT(childItem);
    TreeItem *parentItem = childItem->parent();
    Q_ASSERT(parentItem);

    if(parentItem == m_categoryPlugins)
        return QModelIndex();

    return createIndex(parentItem->row(), 0, parentItem);
}


int Nepomuk::DataPPPool::rowCount(const QModelIndex & parent) const
{
    //kDebug() << "Row  count. parent: "<<parent;
    if(!parent.isValid()) {
        //kDebug() << m_categoryPlugins->childsCount();
        return m_categoryPlugins->childsCount();
    }

    if(parent.column() > 0) {
        //kDebug() << 0;
        return 0;
    }

    //kDebug() << static_cast<TreeItem*>(parent.internalPointer())->childsCount();
    return static_cast<TreeItem*>(parent.internalPointer())->childsCount();
}

int Nepomuk::DataPPPool::columnCount(const QModelIndex & parent) const
{
    //kDebug() << "Column  count. parent: "<<parent;
    // If parent root item return 1
    if(!parent.isValid())
        return 1;

    TreeItem * item = static_cast<TreeItem*>(parent.internalPointer());
    Q_ASSERT(item);
    // If parent is category then return 1 if it has any child
    if(item->isCategory() && item->childsCount())
        return 1;
    else {
        // If it is DataPP or Category without any DataPP assigned
        return 0;
    }

}

QVariant Nepomuk::DataPPPool::data(const QModelIndex & index, int role) const
{
    if(!index.isValid())
        return QVariant();

    TreeItem * item = static_cast<TreeItem*>(index.internalPointer());
    Q_ASSERT(item);
    switch(role) {
    case Qt::DisplayRole : {
        //kDebug() << item->name();
        return item->displayName();
    }
    case DataPPPool::TypeRole : {
        return !item->isCategory() ;
    }
    case DataPPPool::SourceRole : {
        return DataPPPool::sourceById(item->sysName());
    }
    case DataPPPool::IdRole : {
	  return item->sysName();
    }
    case DataPPPool::DataPPRole : {
      return QVariant::fromValue(item->datapp());
    }
    }
    return QVariant();
}

Qt::ItemFlags Nepomuk::DataPPPool::flags(const QModelIndex & index) const
{
    return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsDragEnabled;
}

QVariant Nepomuk::DataPPPool::headerData(int section, Qt::Orientation orientation,
        int role) const
{
    if(orientation == Qt::Horizontal)
        return m_categoryPlugins->sysName();

    return QVariant();

}

QModelIndexList Nepomuk::DataPPPool::match(
    const QModelIndex & start,
    int role,
    const QVariant & value,
    int hits,
    Qt::MatchFlags flags) const
{
    // FIXME Enable respeting flags
    TreeItem * startItem;
    QModelIndexList answer;
    QQueue<QModelIndex> stored;
    stored << start;
    //kDebug() << "Initial Stored: " << stored;



    while((stored.size() > 0) and((answer.size() < hits) or(hits == -1))) {
        //kDebug() << "Stored: " << stored;
        QModelIndex current = stored.dequeue();
        if(current.isValid()) {
            //kDebug() << "Check: " << data(current, Qt::DisplayRole);
            if(data(current, role) == value) {
                answer << current;
                continue;
            }
        }
        for(int i = 0; i < rowCount(current); i++)
            for(int j = 0; j < columnCount(current); j++) {
                //kDebug() << "i,j" << i <<','<<j;
                QModelIndex ch = index(i, j, current);
                if(ch.isValid())
                    stored.enqueue(ch);
            }
    }

    return answer;
}

QStringList Nepomuk::DataPPPool::mimeTypes() const
{
    QStringList types;
    types << "application/vnd.text.list";
    return types;
}

QMimeData * Nepomuk::DataPPPool::mimeData(const QModelIndexList & indexes) const
{
    QMimeData *_mimeData = new QMimeData();
    //QByteArray encodedData;
    //QDataStream stream(&encodedData, QIODevice::WriteOnly);

    foreach(QModelIndex index, indexes) {
        if(index.isValid()) {
            if(data(index, DataPPRole).toBool()) {
                QString text = data(index, NameRole).toString();
                kDebug() << "Create mimetype for index: " << index << "Name: " <<
                text;
                _mimeData->setText(text);
                //stream << text;
            }
        }
    }

    //_mimeData->setData("text/plain", encodedData);
    kDebug() << "Mime text: " << _mimeData->text();
    return _mimeData;
}


QDebug Nepomuk::operator<<(QDebug dbg,  const DataPPPool & pool)
{
    dbg << " Pool of all DataPP installed in system/user " << '\n';
    foreach(const QString & plg, pool.m_availableDataPP) {
        dbg << plg << /*" source: " << pool.m_dataPPSources[plg]<<*/'\n';
    }
    if(pool.m_categoryPlugins) {
        dbg << " DataPP,per-category " << '\n';
        pool.m_categoryPlugins->print(0, dbg);
    }


    return dbg;
}
