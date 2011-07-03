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

#ifndef NEPOMUK_DDMS_RESULT_PROXIES
#define NEPOMUK_DDMS_RESULT_PROXIES

#include "ddms_client_export.h"
#include "decision_proxy.h"

#include <QDBusPendingCallWatcher>
#include <QDBusPendingCall>
#include <QList>


namespace Nepomuk {
    class DecisionManagementClient;
    class DDMS_CLIENT_EXPORT IdProxy : public QDBusPendingCallWatcher
    {
        Q_OBJECT;

        protected:
            IdProxy(DecisionManagementClient * client, const QDBusPendingCall & call, QObject * parent = 0 );
            friend class DecisionManagementClient;
        protected Q_SLOTS:
            void onFinish( QDBusPendingCallWatcher * watcher );
        Q_SIGNALS:
           void finished(IdProxy *, int id, int error);
           void failed(IdProxy *);
        public:
           int id() const { return m_id; }
           int error() const { return m_error; }
           DecisionProxy toDecisionProxy() const;
           QDBusError dbusError() const { return this->QDBusPendingCall::error(); }

        private:
           int m_id;
           int m_error;
           DecisionManagementClient * m_client;
    };

    class DDMS_CLIENT_EXPORT IdListProxy : public QDBusPendingCallWatcher
    {
        Q_OBJECT;
        protected:
            IdListProxy(DecisionManagementClient * client, const QDBusPendingCall & call, QObject * parent = 0 );
            friend class DecisionManagementClient;
        protected Q_SLOTS:
            void onFinish( QDBusPendingCallWatcher * watcher );
        Q_SIGNALS:
           void finished(IdListProxy *,QList<int> list);
           void failed(IdListProxy*);
        public:
           QList<int> ids() const { return m_result; }
           QList<DecisionProxy> toDecisionProxies()const;

        private:
           QList<int> m_result;
           DecisionManagementClient * m_client;
    };
     
}


#endif
// vim:sw=4 ts=8 expandtab
