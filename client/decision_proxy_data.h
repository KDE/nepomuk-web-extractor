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

#ifndef NEPOMUK_DDMS_DECISION_PROXY_DATA_H
#define NEPOMUK_DDMS_DECISION_PROXY_DATA_H

#include <QtCore/QString>
#include <QtCore/QUrl>
#include <QtCore/QList>

namespace Nepomuk {
    class DecisionManagementClient;
    class DecisionProxyData
    {
        public:
            DecisionProxyData(DecisionManagementClient * client, int id, bool checkExist);
            // Declaration only
            DecisionProxyData(const DecisionProxyData &);
            ~DecisionProxyData();

            QString description;
            QList<QUrl> uriList;
            bool valid;
        private:
            bool m_metadataLoaded;
            DecisionManagementClient * m_client;
            int m_id;

        public:
            void loadMetadata();
        private:
            void initialCheckExist();
    };
}
#endif

