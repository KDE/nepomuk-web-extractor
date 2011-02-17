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

#include "datappconfig.h"
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
    class WEBEXSETTINGS_EXPORT DppExecutivePool: public QAbstractItemModel
    {
            Q_OBJECT;
        public:
            /*! \brief Return list of all DppExecutive in the system
             * This function will return set of id of all available DppExecutive in the system.
             * It will return both valid and invalid DppExecutive.
             */
            static QStringList availableDppExecutive();

            /*! \brief Return list of all DppExecutive in the system
             * This function will return set of id of all valid DppExecutive in the system.
             */
            static QStringList validDppExecutive();

            /*! \brief  Return id of all datapp that belong to category
             */
            static QSet< QString >  categoryDppExecutives(const QString & categoryName) ;

            // Return number of main categories
            static int categoryCount() ;

            static DppExecutivePool * self();

            enum Roles {
                /*! \brief Boolean role. Store type of this item - DppExecutive or Category
                 */
                DppExecutiveRole = Qt::UserRole + 1,
                /*! \brief Store source plugin for DppExecutive or NULL string for category
                 */
                SourceRole = Qt::UserRole + 2,

                /*! \brief Store the system name of the DppExecutive
                 * This is the name that can be used in DppExecutiveConfig and
                 * other system functions
                 */
                IdRole,

                /*! \brief Store name of datapp/category
                 */
                NameRole = Qt::DisplayRole,
            };
            static QString  dataPPSource(const QString &);
            static QString displayNameById(const QString & id);
            //static KSharedConfigPtr dataPPConfig(const QString & name);
            friend QDebug operator<<(QDebug dbg,  const DppExecutivePool & cat);
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
        private Q_SLOTS:
            void update();
        private:
            DppExecutivePool(QObject * parent = 0);
            // List of all available DppExecutive in system
            // TODO Use QSet<QString> instead of QStringList
            QStringList m_availableDppExecutive;
            QStringList m_validDppExecutive;

            // Hash CategoryName->List of all available datapp in system that belongs to
            // this category(DppExecutive category, not WebEXtractor Categories)
            //
            //QHash< QString, QSet<QString> > m_categoryPlugins;
            TreeItem * m_categoryPlugins;

            // Cache the source plugin for datapp
            QHash< QString, QString > m_dataPPSources;

            // Cache the display name for datapp
            QHash< QString, QString > m_displayNames;

            // Watch for directories where datapp config files are stored
            KDirWatch wc;

            static DppExecutivePool * m_self;
    };
    QDebug operator<<(QDebug dbg,  const DppExecutivePool & cat);
}

#endif
