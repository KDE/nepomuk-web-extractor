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


#include "client.h"
#include "respond_proxies.h"
#include "decisionmanagementserviceproxy.h"
#include <QDBusPendingCallWatcher>

using namespace Nepomuk;

class Nepomuk::DecisionManagementClient::Private
{
    public:
	OrgKdeNepomukDecisionManagementInterface * interface;
	Private():
	    interface(0)
	{;}
};

Nepomuk::DecisionManagementClient::DecisionManagementClient(
	const QString & service, const QString & path, 
	const QDBusConnection & connection,
	QObject * parent):
    QObject(parent)
{
    d = new Private();
    d->interface = new OrgKdeNepomukDecisionManagementInterface(
	    service,path,connection
	    );
}

Nepomuk::DecisionManagementClient::~DecisionManagementClient()
{
    delete d->interface;
    delete d;
}

bool Nepomuk::DecisionManagementClient::isValid() const 
{
    return d->interface->isValid();
}

Nepomuk::DecisionManagementClient*
Nepomuk::DecisionManagementClient::instance()
{
    static DecisionManagementClient * self = new DecisionManagementClient(
	    "org.kde.nepomuk.services.""nepomukddmservice",
	    "/nepomukddmservice",
	    QDBusConnection::sessionBus()
	    );
    return self;
}
IdProxy *
Nepomuk::DecisionManagementClient::addDecision(const QString &decision, const QStringList &uriList )
{
    return new IdProxy(
	    d->interface->addDecision(decision,uriList),
	    this
	    );
}

IdListProxy * 
Nepomuk::DecisionManagementClient::getDecisions(const QString &uri)
{
    return new IdListProxy( 
	    d->interface->getDecisions(uri),
	    this
	    );
}

QDBusPendingCallWatcher*
Nepomuk::DecisionManagementClient::removeDecision(int id)
{
    return new QDBusPendingCallWatcher(
	    d->interface->removeDecision(id),
	    this
	    );
}
// vim:sw=4 ts=8 expandtab
