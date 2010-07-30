/*
   Copyright (C) 2010 by Serebriyskiy Artem <v.for.vandal at gmail.com>

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
#include "propertiesgroup.h"
#include "decision.h"
#include "datapp.h"
#include <QtCore/QSharedData>
#include <QtCore/QTime>
#include <QtCore/QList>
#include <QtCore/QMultiMap>
#include <KDebug>
#include "ndco.h"
#include "decisiondata.h"

namespace NW = Nepomuk::WebExtractor;



double Nepomuk::WebExtractor::Decision::truncateRank(double rank)
{
    return WE::boundRank(rank);
}

double Nepomuk::WebExtractor::Decision::rank() const
{
    return d->rank;
}

QString  Nepomuk::WebExtractor::Decision::pluginVersion() const
{
    Q_ASSERT(!d->authorsData.isEmpty());
    return (*(d->authorsData.begin()))->pluginVersion();
}

QString  Nepomuk::WebExtractor::Decision::pluginName() const
{
    Q_ASSERT(!d->authorsData.isEmpty());
    return (*(d->authorsData.begin()))->pluginName();
}

Nepomuk::WebExtractor::Decision::Decision(
    const DataPP * parent,
    ResourceManager * manager
):
    d(new Nepomuk::WebExtractor::DecisionData())
{
    Q_ASSERT(manager);
    // If parent == 0 then this is invalid Decision
    if(parent)
        d->authorsData.insert(parent);

    d->timeStamp = QTime::currentTime();
    d->manager = manager;
    d->model = manager->mainModel();
    // TODO May be it is necessary to create new context for Decision not after Decision
    // was created, but after first PropertyGroup was created for this Decision.
    // This will help to avoid empty Decisions in the model
    d->contextUrl = manager->generateUniqueUri();
    //TODO Resources must be created inside the context url
    d->decisionRes = Nepomuk::Resource(d->contextUrl, NW::Vocabulary::NDCO::Decision(), d->manager);
    d->setFreeze(false);
}

Nepomuk::WebExtractor::Decision::Decision(
):
    d(new Nepomuk::WebExtractor::DecisionData())
{
};


Nepomuk::WebExtractor::Decision::~Decision()
{
    kDebug() << "Decision is destroyed";
}

Nepomuk::WebExtractor::Decision::Decision(const Decision & rhs)
{
    d = rhs.d;
}

const Nepomuk::WebExtractor::Decision & Nepomuk::WebExtractor::Decision::operator=(const Decision & rhs)
{
    this->d = rhs.d;
    return *this;
}

bool Nepomuk::WebExtractor::Decision::operator==(const Decision & rhs) const
{
    if(this == &rhs)
        return true;
    if(this->d == rhs.d)
        return true;

#warning Implement this!

    return false;
    //return (d->data == rhs.d->data);
}

bool Nepomuk::WebExtractor::Decision::operator!=(const Decision & rhs)const
{
    return !(*this == rhs);
}

bool Nepomuk::WebExtractor::Decision::isEmpty() const
{
    return d->data.isEmpty();
}

bool Nepomuk::WebExtractor::Decision::isValid() const
{
    return (!d->contextUrl.isEmpty()) and(!d->authorsData.isEmpty());
}

void Nepomuk::WebExtractor::Decision::setRank(double rank)
{
    // If freezed
    if(d->isFreezed())
        return ;

    rank = truncateRank(rank);

    d->rank = rank;
}

QUrl NW::Decision::uri() const
{
    return d->contextUrl;
}

Nepomuk::ResourceManager * NW::Decision::manager() const
{
    return d->manager;
}


Soprano::Model * NW::Decision::model() const
{
    return d->model;
}

QString NW::Decision::description() const
{
    return d->description;
}


void NW::Decision::setDescription(const QString & description)
{
    d->description = description;
}


NW::PropertiesGroup NW::Decision::newGroup()
{
    // Check that decision is valid
    if(!isValid())
        return PropertiesGroup(0);

    // If freezed
    if(d->isFreezed())
        return PropertiesGroup(0);

    // First create new property group
    PropertiesGroup pg = PropertiesGroup(this->d.data());

    d->data.insert(pg);
    return pg;

}

QSet< NW::PropertiesGroup > NW::Decision::groups() const
{
    return d->data;
}


QUrl NW::Decision::proxyResource(const Nepomuk::Resource & res)
{
    // Check that decision is valid
    if(!isValid())
        return QUrl();

    // If freezed
    if(d->isFreezed())
        return QUrl();

    QMap< QUrl, QUrl>::iterator it = d->resourceProxyUrlMap.find(res.resourceUri());
    if(it != d->resourceProxyUrlMap.end())
        return it.value();

    // TODO Copy identifying properties from the source resource to new one
    QUrl newUrl = d->manager->generateUniqueUri();

    // Add a hint
    d->model->addStatement(
        newUrl,
        NW::Vocabulary::NDCO::aliasHint(),
        Soprano::LiteralValue(
            res.resourceUri().toString()
        ),
        d->contextUrl
    );

    // Add to the map
    d->resourceProxyUrlMap.insert(res.resourceUri(), newUrl);

    // TODO Mark resource as syncable with some properties
    //
    return newUrl;

}

QMap<QUrl, QUrl> NW::Decision::proxies() const
{
    return d->resourceProxyUrlMap;
}

/*
void Nepomuk::WebExtractor::Decision::addStatement(const Soprano::Statement & statement, double rank)
{
    PropertiesGroup grp;
    grp << statement;
    grp.setRank(rank);
    addGroup(grp);
}
*/
/*
void Nepomuk::WebExtractor::Decision::addGroup( const PropertiesGroup & grp)
{
    //rank = Private::truncateRank(rank);

    // Check that none of this statemnt's exist in model.
    // Those that's exist - ignore

    // Add statements
    //d->data.insert(rank,statements);
    d->data << grp;

    // Increase hash
    d->hash ^= qHash(grp);
}
*/

void NW::Decision::freeze()
{
    this->d->setFreeze(true);
}

bool NW::Decision::isFreezed() const
{
    return this->d->isFreezed();
}

void Nepomuk::WebExtractor::Decision::apply() const
{
    /*
    kDebug() << "Write statements to storage";
    foreach ( const PropertiesGroup & lst, d->data )
    {
    foreach( const Soprano::Statement  & st, lst.data() )
    {
        kDebug() << st;
    }
    }
    */
}

void Nepomuk::WebExtractor::Decision::addToUserDiscretion()
{
    /*
    kDebug() << "Write Decision to user discretion list";
    foreach ( const PropertiesGroup  &  lst, d->data )
    {
    foreach( const Soprano::Statement  & st, lst.data() )
    {
        kDebug() << st;
    }
    }
    */
}

void Nepomuk::WebExtractor::Decision::addAuthor(const DataPP * author)
{
    d->authorsData.insert(author);
}

unsigned int Nepomuk::WebExtractor::qHash(const Nepomuk::WebExtractor::Decision & des)
{
    return des.d->hash;
}
