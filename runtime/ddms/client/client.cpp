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
#include "protocol_types.h"
#include "decision_proxy.h"
#include "decision_proxy_data.h"
#include "decisionmanagementserviceproxy.h"

#include <QDBusPendingCallWatcher>
#include <decision/decision.h>
#include <decision/decisionmetadata.h>

using namespace Nepomuk;

class Nepomuk::DecisionManagementClient::Private
{
    public:
	OrgKdeNepomukDecisionManagementInterface * interface;
        QMap<int, QSharedPointer<DecisionProxyData> > decisionsData;
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
    /* Init necessary QT metatypes */
    IdAndError::registerMetaType();
    IdList::registerMetaType();
    Decision::DecisionMetadata::registerMetaType();
    MetadataAndError::registerMetaType();
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
Nepomuk::DecisionManagementClient::addDecision(const Decision::Decision &decision)
{
    QByteArray result;
    qDebug() << decision;
    QDataStream dstream(&result, QIODevice::WriteOnly );
    dstream << decision;
    
    return new IdProxy(
	    this,
	    d->interface->addDecision(result),
            this
	    );
}

IdListProxy * 
Nepomuk::DecisionManagementClient::getDecisions(const QUrl &uri)
{
    return new IdListProxy( 
	    this,
	    d->interface->getDecisions(uri.toString()),
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

bool Nepomuk::DecisionManagementClient::existsDecision(int id) const
{
    QDBusPendingReply<bool> reply =  d->interface->existsDecision(id); 
    if ( reply.isError() )
        return false;
    else 
        return reply.value();
}

QSharedPointer<DecisionProxyData> 
Nepomuk::DecisionManagementClient::getDecisionData(int id, bool checkExist)
{
    QMap<int,QSharedPointer<DecisionProxyData> >::const_iterator fit = 
        d->decisionsData.find(id);

    if ( fit == d->decisionsData.end() ) {
        // Generate new DecisionProxyData
        QSharedPointer<DecisionProxyData> ptr(
                new DecisionProxyData(this,id,checkExist)
                );
        if ( !checkExist ) { // Then we assume that decision do exists
            ptr->valid = true;
        }
        d->decisionsData[id] = ptr;
        return ptr;
    }
    else {
        return fit.value();
    }

}

Nepomuk::Decision::DecisionMetadata 
Nepomuk::DecisionManagementClient::getDecisionMetadata(int id, int & error )
{
    QDBusReply<MetadataAndError> reply = d->interface->decisionMetadata(id);

    if ( reply.isValid() ) {
        MetadataAndError md = reply.value();
        error = md.error;
        return md.metadata;
    }
    else {
        qDebug() << "DO SOMETHING HERE - DBUS FAILED";
        error = Error::DBusError;
    }

    return Nepomuk::Decision::DecisionMetadata();
}

// vim:sw=4 ts=8 expandtab
