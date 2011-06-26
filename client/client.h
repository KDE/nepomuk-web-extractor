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

#include <QtCore/QObject>
#include <QtCore/QString>
#include <QDBusConnection>
#include <QDBusPendingCallWatcher>

namespace Nepomuk {
    class IdProxy;
    class IdListProxy;
    
    class DecisionManagementClient : public QObject
    {
        Q_OBJECT;
        public:
            DecisionManagementClient( const QString & service, const QString & path, const QDBusConnection & , QObject * parent = 0); 
            ~DecisionManagementClient();
            // declaration only
            DecisionManagementClient( const DecisionManagementClient & );
            void operator=( const DecisionManagementClient & );

            DecisionManagementClient * instance();
            /*! \brief Request list of Decisions matching given uri
             *
             * This function return a IdListProxy object - this objects
             * inherits 
             */
            IdListProxy * getDecisions( const QString &  uri);

            IdProxy * addDecision(const QString &decision, const QStringList &uriList );
            QDBusPendingCallWatcher * removeDecision(int id);
            /*! \brief Return true if object creation was successfull
             *
             * Method is simply proxy to QDBusAbstractInterface::isValid and
             * has the same limitations
             * \see QDBusAbstractInterface
             */
            bool isValid() const;
        private:
            class Private;
            Private * d;
    };
}

#endif
// vim:sw=4 ts=8 expandtab
