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

#ifndef _webexsettings_datapppool_h_
#define _webexsettings_datapppool_h_

#include "datapp.h"
#include "webexsettings_export.h"
#include <QStringList>
#include <QAbstractItemModel>
#include <QModelIndex>
#include <KDirWatch>
#include <QSet>
#include <QCache>
#include <QMimeData>

class TreeItem;

namespace Nepomuk
{
    class WEBEXSETTINGS_EXPORT DataPPPool: public QAbstractItemModel
    {
            Q_OBJECT;
        public:
            /*! \brief Return list of all DataPP in the system
             * This function will return set of id of all available DataPP in the system.
             * It will return both valid and invalid DataPP.
             */
            static QStringList availableDataPP();

            /*! \brief Return list of all DataPP in the system
             * This function will return set of id of all valid DataPP in the system.
             */
            static QStringList validDataPP();

            /*! \brief  Return id of all datapp that belong to category
             */
            static QSet< QString >  categoryDataPPs(const QString & categoryName) ;

            /*! \brief Return pointer to the DataPP object
             * Parentship of the pointer is not transfered - you should not
             * delete it
             */
            static DataPP * dataPPById(const QString & id);

            // Return number of main categories
            static int categoryCount() ;
            static int availableDataPPCount();

            static DataPPPool * self();

            enum Roles {
                /*! \brief Boolean role. Store type of this item - DataPP or Category
                 * True is DataPP, False is Category
                 */
                TypeRole = Qt::UserRole + 1,
                /*! \brief Store source plugin for DataPP or NULL string for category
                 */
                SourceRole = Qt::UserRole + 2,

                /*! \brief Store the system name of the DataPP
                 * This is the system name that can be used in DataPPConfig and
                 * other system functions
                 */
                IdRole,

                /*! \brief Return pointer to the DataPP
                 * If it is first request of the DataPP, then new DataPP object
                 * will be created. So it is not constant-time operation
                 */
                DataPPRole,

                /*! \brief Store name of datapp/category
                 */
                NameRole = Qt::DisplayRole,
            };
            static QString  sourceById(const QString &);
            static QString displayNameById(const QString & id);
            //static KSharedConfigPtr dataPPConfig(const QString & name);
            friend QDebug operator<<(QDebug dbg,  const DataPPPool & cat);
        public:
            int rowCount(const QModelIndex &parent = QModelIndex()) const;
            int columnCount(const QModelIndex &parent = QModelIndex()) const;
            Qt::ItemFlags flags(const QModelIndex & index) const;
            QVariant data(const QModelIndex &index, int role) const;
            QVariant headerData(int section, Qt::Orientation orientation,
                                int role = Qt::DisplayRole) const;
            QModelIndex index(int row, int column, const QModelIndex &parent) const;
            QModelIndex parent(const QModelIndex &index) const;
            QModelIndexList match(
                const QModelIndex & start,
                int role,
                const QVariant & value,
                int hits = 1,
                Qt::MatchFlags flags = Qt::MatchFlags(Qt::MatchStartsWith | Qt::MatchWrap)) const;

            QMimeData * mimeData(const QModelIndexList & indexes) const ;
            QStringList  mimeTypes() const ;
        Q_SIGNALS:
            void dataPPChanged();
        public Q_SLOTS:
            void update();
        private:
            DataPPPool(QObject * parent = 0);
            // List of all available DataPP in system
            // TODO Use QSet<QString> instead of QStringList
            QStringList m_availableDataPP;
            QStringList m_validDataPP;

            // Hash CategoryName->List of all available datapp in system that belongs to
            // this category(DataPP category, not WebEXtractor Categories)
            //
            //QHash< QString, QSet<QString> > m_categoryPlugins;
            TreeItem * m_categoryPlugins;

            // Cache the source plugin for datapp
            QHash< QString, QString > m_dataPPSources;

            // Cache the display name for datapp
            QHash< QString, QString > m_displayNames;

            // Hash of DataPP*
            QHash< QString, DataPP*> m_dataPPHash;

            // Watch for directories where datapp config files are stored
            KDirWatch wc;

            static DataPPPool * m_self;
    };
    QDebug WEBEXSETTINGS_EXPORT operator<<(QDebug dbg,  const DataPPPool & cat);
}

#endif
