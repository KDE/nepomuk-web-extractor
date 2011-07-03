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

#include "respond_proxies.h"
#include "protocol_types.h"
#include "client.h"

#include <QDBusPendingReply>

using namespace Nepomuk;

IdProxy::IdProxy(DecisionManagementClient * client, const QDBusPendingCall & call, QObject * parent ):
	QDBusPendingCallWatcher(call,parent),
	m_id(-1),
	m_error(2),
        m_client(client)
{
    Q_ASSERT(m_client);
    connect(
	    this, SIGNAL(finished(QDBusPendingCallWatcher *)),
	    this,SLOT(onFinish(QDBusPendingCallWatcher * ))
	   );
}

void IdProxy::onFinish( QDBusPendingCallWatcher * watcher )
{
    QDBusPendingReply<int,int> reply = *watcher;
    if (reply.isError() )
	emit failed(this);
    else {
	m_id = reply.argumentAt<0>();
	m_error = reply.argumentAt<1>();
	emit finished(this,m_id,m_error);
    }
}

DecisionProxy IdProxy::toDecisionProxy() const
{
    if (!isFinished())
        return DecisionProxy();
    else if ( m_error != Error::NoError )
       return DecisionProxy();
    else  
        return DecisionProxy(m_client,m_id);
}

IdListProxy::IdListProxy(DecisionManagementClient * client, const QDBusPendingCall & call, QObject * parent ):
	QDBusPendingCallWatcher(call,parent),
        m_client(client)
{
    Q_ASSERT(m_client);
    connect(
	    this, SIGNAL(finished(QDBusPendingCallWatcher *)),
	    this,SLOT(onFinish(QDBusPendingCallWatcher * ))
	   );
}

void IdListProxy::onFinish( QDBusPendingCallWatcher * watcher )
{

    QDBusPendingReply<IdList> reply = *watcher;
    if (reply.isError() )
	emit failed(this);
    else {
	m_result = reply.value(); 
	emit finished(this,m_result);
    }
}

QList<DecisionProxy>
IdListProxy::toDecisionProxies() const
{
    QList<DecisionProxy> answer;
    if (!isFinished())
        return answer;
    else {
        foreach( int id, m_result )
        {
            answer << DecisionProxy(m_client,id);
        }
    }

    return answer;
}
