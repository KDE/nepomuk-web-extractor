/*
   Copyright (C) 2010 by Serebriyskiy Artem <v.for.vandal at gmail.com>

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#ifndef _webexsettings_datapppool_h_
#define _webexsettings_datapppool_h_

#include "datappconfig.h"
#include "webexsettings_export.h"
#include <QStringList>
#include <QAbstractItemModel>
#include <QModelIndex>
#include <KDirWatch>
#include <QSet>
#include <QCache>

// FIXME Mode defenition of treeitem to the source file instead of header
namespace
{
    class TreeItem
    {
        public:
            TreeItem(const QString & name);
            ~TreeItem();
            // Add datapp only if this datpp doesn't exist
            TreeItem *  addDataPP(const QString & name);
            // Add category only if this category doesn't exist
            // Return pointer to category
            TreeItem * addCategory(const QString & name);
            TreeItem * child(int row);
            TreeItem * category(const QString & name);
            TreeItem * datapp(const QString & name);
            QList< TreeItem* > allDataPP() const;
            QSet< QString > allDataPPNames() const;
            TreeItem * parent();
            int childsCount();
            int row() const;
            bool isCategory() const;
            QString name() const;
            void print(int displacments, QDebug & stream);

        private:
            // Name of datapp or name of category
            QString  m_name;
            QString source;
            QList< TreeItem*> childs;
            QHash< QString, TreeItem *> m_datapps;
            QHash< QString, TreeItem *> categories;

            bool m_category;
            TreeItem * parentItem;

    };
}

namespace Nepomuk
{
    class WEBEXSETTINGS_EXPORT DataPPPool: public QAbstractItemModel
    {
            Q_OBJECT;
        public:
            static QStringList plugins();

            // Return all datapp that belong to category
            static QSet< QString >  categoryDataPPs(const QString & categoryName) ;

            // Return number of main categories
            static int categoryCount() ;

            static DataPPPool * self();

            enum Roles {
                /*! \brief Boolean role. Store type of this item - DataPP or Category
                 */
                DataPPRole = Qt::UserRole + 1,
                /*! \brief Store source plugin for DataPP or NULL string for category
                 */
                SourceRole
            };
            static QString  dataPPSource(const QString &);
            //static KSharedConfigPtr dataPPConfig(const QString & name);
            friend QDebug operator<<(QDebug dbg,  const DataPPPool & cat);
        public:
            int rowCount(const QModelIndex &parent = QModelIndex()) const;
            int columnCount(const QModelIndex &parent = QModelIndex()) const;
            //Qt::ItemFlags flags(const QModelIndex & index) const;
            QVariant data(const QModelIndex &index, int role) const;
            QVariant headerData(int section, Qt::Orientation orientation,
                                int role = Qt::DisplayRole) const;
            QModelIndex index(int row, int column, const QModelIndex &parent) const;
            QModelIndex parent(const QModelIndex &index) const;

        Q_SIGNALS:
            void dataPPChanged();
        private Q_SLOTS:
            void update();
        private:
            DataPPPool(QObject * parent = 0);
            // List of all available DataPP in system
            // TODO Use QSet<QString> instead of QStringList
            QStringList m_plugins;

            // Hash CategoryName->List of all available datapp in system that belongs to
            // this category(DataPP category, not WebEXtractor Categories)
            //
            //QHash< QString, QSet<QString> > m_categoryPlugins;
            TreeItem * m_categoryPlugins;

            // Cache the source plugin for datapp
            QHash< QString, QString > m_dataPPSources;

            // Watch for directories where datapp config files are stored
            KDirWatch wc;

            static DataPPPool * m_self;
    };
    QDebug operator<<(QDebug dbg,  const DataPPPool & cat);
}

#endif
