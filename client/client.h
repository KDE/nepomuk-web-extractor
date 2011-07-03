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

#ifndef NEPOMUK_DDMS_CLIENT_H
#define NEPOMUK_DDMS_CLIENT_H

#include "ddms_client_export.h"

#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QSharedPointer>
#include <QDBusConnection>
#include <QDBusPendingCallWatcher>

namespace Nepomuk {
    class IdProxy;
    class IdListProxy;
    class DecisionProxy;
    class DecisionProxyData;
    
    class DDMS_CLIENT_EXPORT DecisionManagementClient : public QObject
    {
        Q_OBJECT;
        public:
            DecisionManagementClient( const QString & service, const QString & path, const QDBusConnection & , QObject * parent = 0); 
            ~DecisionManagementClient();
            // declaration only
            DecisionManagementClient( const DecisionManagementClient & );
            void operator=( const DecisionManagementClient & );

            static DecisionManagementClient * instance();
            /*! \brief Request list of Decisions matching given uri
             *
             * This function return a IdListProxy object - this objects
             * inherits 
             */
            IdListProxy * getDecisions( const QUrl &  uri);

            IdProxy * addDecision(const QString &decision, const QStringList &uriList );
            QDBusPendingCallWatcher * removeDecision(int id);
            /*! \brief Return true if object creation was successfull
             *
             * Method is simply proxy to QDBusAbstractInterface::isValid and
             * has the same limitations
             * \see QDBusAbstractInterface
             */
            bool isValid() const;

            bool existsDecision(int id) const;
        private:
            friend class DecisionProxy;
            friend class DecisionProxyData;

            QSharedPointer<DecisionProxyData> getDecisionData(int id, bool checkExist);
            DecisionMetadata getDecisionMetadata(int id, int & error);
            class Private;
            Private * d;
    };
}

#endif
// vim:sw=4 ts=8 expandtab
