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

#include "decision_model.h"
#include "client.h"
#include "decision_proxy.h"
#include "respond_proxies.h"

#include <QtCore/QSharedDataPointer>
#include <QtCore/QtDebug>

using namespace Nepomuk;

class Nepomuk::DecisionModel::Private : public QSharedData
{
    public:
        Private():client(0) {;}
        ~Private() {;}
        QList<DecisionProxy> decisions;
        DecisionManagementClient * client;
};

DecisionModel::DecisionModel(DecisionManagementClient * client):
    d( new Private() )
{
    QHash<int, QByteArray> roles;
    roles[Description] = "description";
    setRoleNames(roles);

    d->client = client;
}

DecisionModel::~DecisionModel()
{;}

DecisionModel::DecisionModel(const DecisionModel & rhs):
    d(rhs.d)
{;}

DecisionModel & DecisionModel::operator=( const DecisionModel & rhs)
{
    this->d = rhs.d;
    return *this;
}

#if 0
void DecisionModel::setUriList(const QList<QUrl> & list)
{
    // Clear old data
    beginResetModel();
    d->decision.clear();
    endResetModel();

    // Request new data
    IdListProxy * ilp = d->client->getDecisions(/*NOT IMPLEMENTED */);

}
#endif

void DecisionModel::setUri(const QUrl & uri )
{
    // Clear old data
    beginResetModel();
    d->decisions.clear();
    endResetModel();

    qDebug() << "Uri: " << uri;

    IdListProxy * ilp = d->client->getDecisions(uri);
    connect(ilp, SIGNAL(finished(IdListProxy *,QList<int>)),
            this, SLOT(onLoaded(IdListProxy *,QList<int>))
                );
    connect(ilp, SIGNAL(failed(IdListProxy *)),
            this,SLOT(onFailed(IdListProxy *))
           );

}

int DecisionModel::rowCount(const QModelIndex &) const
{
    return d->decisions.size();
}

QVariant DecisionModel::data(const QModelIndex & index, int role ) const
{
    if ( index.row() < 0 || index.row() > d->decisions.size() )
        return QVariant();

    int row = index.row();
    switch (role) 
    {
        case Description : { return d->decisions[row].description(); }
        case Proxy : { return QVariant::fromValue(d->decisions[row]); }
        default: { return QVariant(); }
    }

    return QVariant();
}

void DecisionModel::onLoaded(IdListProxy *p, QList<int> idList)
{
    beginInsertRows(QModelIndex(),0,d->decisions.size());
    foreach( int id, idList )
    {
        d->decisions << DecisionProxy(d->client,id);
    }
    endInsertRows();

    qDebug() << "Items recieved: " << d->decisions.size();

    p->deleteLater();

}

void DecisionModel::onFailed(IdListProxy * p)
{
    qDebug() << "Failed to communicate with server";
    p->deleteLater();
}
