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

#include <QDBusPendingCallWatcher>
#include <QDBusPendingCall>
#include <QList>

namespace Nepomuk {
    class DecisionManagementClient;
    class IdProxy : public QDBusPendingCallWatcher
    {
        Q_OBJECT;

        protected:
            IdProxy( const QDBusPendingCall & call, QObject * parent = 0 );
            friend class DecisionManagementClient;
        protected Q_SLOTS:
            void onFinish( QDBusPendingCallWatcher * watcher );
        Q_SIGNALS:
           void finished(IdProxy *, int id, int error);
           void failed(IdProxy *);
        public:
           int id() const { return m_id; }
           int error() const { return m_error; }
           QDBusError dbusError() const { return this->QDBusPendingCall::error(); }
        private:
           int m_id;
           int m_error;
    };

    class IdListProxy : public QDBusPendingCallWatcher
    {
        Q_OBJECT;
        protected:
            IdListProxy( const QDBusPendingCall & call, QObject * parent = 0 );
            friend class DecisionManagementClient;
        protected Q_SLOTS:
            void onFinish( QDBusPendingCallWatcher * watcher );
        Q_SIGNALS:
           void finished(IdListProxy *,QList<int> list);
           void failed(IdListProxy*);
        public:
           QList<int> ids() const { return m_result; }

        private:
           QList<int> m_result;
    };
     
}


#endif
// vim:sw=4 ts=8 expandtab
