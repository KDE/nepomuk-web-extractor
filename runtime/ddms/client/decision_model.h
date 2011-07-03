/*
   Copyright (C) 2011 by Serebriyskiy Artem <v.for.vandal at gmail.com>

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

#ifndef NEPOMUK_DECISION_PROXY_MODEL_H
#define NEPOMUK_DECISION_PROXY_MODEL_H

#include <QtCore/QAbstractListModel>
#include <QtCore/QVariant>
#include <QtCore/QSharedDataPointer>

namespace Nepomuk {
    class IdListProxy;
    class DecisionManagementClient;
    class DecisionModel  : public QAbstractListModel
    {
        public:
            enum Roles { Description, Proxy };
            DecisionModel(DecisionManagementClient * client);
            ~DecisionModel();

            //void setUriList(const QList<QUrl> &);
            DecisionModel(const DecisionModel & );
            DecisionModel & operator=( const DecisionModel & );
            int rowCount(const QModelIndex &) const;
            QVariant data( const QModelIndex &, int ) const;
        public Q_SLOTS:
            void setUri(const QUrl & uri );
        private Q_SLOTS:
            void onLoaded(IdListProxy * , QList<int> );
            void onFailed(IdListProxy*);
        private:
            class Private;
            QSharedDataPointer<Private> d;
    };
};
#endif

