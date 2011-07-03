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

#ifndef NEPOMUK_DDMS_DECISION_PROXY_H
#define NEPOMUK_DDMS_DECISION_PROXY_H

#include "ddms_client_export.h"

#include <QtCore/QSharedPointer>
#include <QtCore/QString>
#include <QtCore/QMetaType>


namespace Nepomuk {
    class DecisionManagementClient;
    class DecisionProxyData;
    class DDMS_CLIENT_EXPORT DecisionProxy
    {
        public:
            DecisionProxy(DecisionManagementClient * client, int id);
            DecisionProxy();
            ~DecisionProxy();
            DecisionProxy(const DecisionProxy & );
            DecisionProxy & operator=(const DecisionProxy & );
            /*! \brief Return true if this proxy is valid
             * Valid proxies are proxies that were created with client and id 
             * parameters and server reported that such id exists.
             * Default constructor returns invalid proxies
             */
            bool isValid() const;
            QString description() const;
        private:
            QSharedPointer<DecisionProxyData> d;
    };
}

Q_DECLARE_METATYPE(Nepomuk::DecisionProxy)
#endif
