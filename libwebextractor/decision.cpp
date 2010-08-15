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
#include "decisionapplicationrequest.h"
#include "decision.h"
#include "datapp.h"
#include <QtCore/QSharedData>
#include <QtCore/QTime>
#include <QtCore/QList>
#include <QtCore/QMultiMap>
#include <KDebug>
#include "ndco.h"
#include "decisiondata.h"
#include "identsetmanager.h"
#include "algorithm.h"
#include <nepomuk/mergerequest.h>
#include <nepomuk/changelog.h>

namespace NW = Nepomuk::WebExtractor;
namespace NS = Nepomuk::Sync;



double NW::Decision::truncateRank(double rank)
{
    return boundRank(rank);
}

double NW::Decision::rank() const
{
    return d->rank;
}

float  NW::Decision::pluginVersion() const
{
    Q_ASSERT(!d->authorsData.isEmpty());
    return (*(d->authorsData.begin()))->pluginVersion();
}

QString  NW::Decision::pluginName() const
{
    Q_ASSERT(!d->authorsData.isEmpty());
    return (*(d->authorsData.begin()))->pluginName();
}

NW::Decision::Decision(
    const DataPP * parent,
    Soprano::Model * decisionsModel,
    IdentificationSetManager * identsetManager
):
    d(new NW::DecisionData(parent, decisionsModel, identsetManager))
{
}

NW::Decision::Decision(
):
    d(new NW::DecisionData())
{
};


NW::Decision::~Decision()
{
}

NW::Decision::Decision(const Decision & rhs)
{
    d = rhs.d;
}

const NW::Decision & Nepomuk::WebExtractor::Decision::operator=(const Decision & rhs)
{
    this->d = rhs.d;
    return *this;
}

bool NW::Decision::operator==(const Decision & rhs) const
{
    if(this == &rhs)
        return true;
    if(this->d == rhs.d)
        return true;

#warning Implement this!

    return false;
    //return (d->data == rhs.d->data);
}

bool NW::Decision::operator!=(const Decision & rhs)const
{
    return !(*this == rhs);
}

bool NW::Decision::isEmpty() const
{
    return d->data.isEmpty();
}

bool NW::Decision::isValid() const
{
    return (!d->contextUrl.isEmpty()) and(!d->authorsData.isEmpty());
}

void NW::Decision::setRank(double rank)
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
    return d->filterModel;
}

QString NW::Decision::description() const
{
    return d->description;
}


void NW::Decision::setDescription(const QString & description)
{
    d->description = description;
}

NS::ChangeLog NW::Decision::log() const
{
    NS::ChangeLog answer;
    foreach(const PropertiesGroup & grp, d->data) {
        answer << grp.log();
    }

    return answer;
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

/*
QList<QUrl> NW::Decision::groupsUrls() const
{
    QList<QUrl> answer;
    foreach( const PropertiesGroup & grp, d->data)
    {
    answer << grp.uri();
    }
    return answer;
}
*/

/*
void NW::Decision::setCurrentGroup( const PropertiesGroup & group)
{
    d->setCurrentGroup(group);
}
*/

void NW::Decision::resetCurrentGroup()
{
    d->resetCurrentGroup();
}


QUrl NW::Decision::proxyUrl(const Nepomuk::Resource & res)
{
    // Check that decision is valid
    if(!isValid())
        return QUrl();

    // If freezed
    if(d->isFreezed())
        return QUrl();

    /*
    QMap< QUrl, QUrl>::iterator it = d->resourceProxyUrlMap.find(res.resourceUri());
    if(it != d->resourceProxyUrlMap.end())
        return it.value();
    */
    QUrl sourceUrl = res.resourceUri();

    // Now we should create/obtain a IdentificationSet for original resource
    // and perform a deep copy of the original resource to the decisions model
    QHash<QUrl, QUrl>::const_iterator fit =
        d->resourceProxyMap.find(sourceUrl);

    if(fit == d->resourceProxyMap.end()) { // Resource not found


        // First disable any current group
        PropertiesGroup save = d->resetCurrentGroup();
        // Perform actual copying
        QUrl newUrl =
            Nepomuk::deep_resource_copy_adjust(res, d->manager, &(d->resourceProxyMap))->operator[](res.resourceUri());
        //kDebug() << "Proxy for " << res.resourceUri() << " is: " << newUrl;
        Q_ASSERT(!newUrl.isEmpty());

        // Restore current group
        d->setCurrentGroup(save);

        // Add to the list of copied resources
        d->resourceProxyMap.insert(sourceUrl, newUrl);

        // Create ignore list
        QSet<QUrl> ignoreList = d->resourceProxyISMap.keys().toSet();

        // Create identification set
        NS::IdentificationSet  set = NS::IdentificationSet::fromResource(sourceUrl, ResourceManager::instance()->mainModel(), ignoreList);
        // Add url to the ACL of the filter log model
        d->filterModel->addTarget(newUrl);

        d->resourceProxyISMap.insert(sourceUrl, set);
        // Add a hint
        Q_ASSERT(d->decisionsModel);
        /*
        d->decisionsModel->addStatement(
            newUrl,
            NW::Vocabulary::NDCO::aliasHint(),
            Soprano::LiteralValue(
            res.resourceUri().toString()
            ),
            d->contextUrl
            */
        return newUrl;
    } else {
        // Now it is possible situation that resource was copied,
        // but it wasn't marked as target  resource and it's identification set
        // was not created.
        if(!d->resourceProxyISMap.contains(sourceUrl)) {
            // Create ignore list
            QSet<QUrl> ignoreList = d->resourceProxyISMap.keys().toSet();
            // Create identification set
            NS::IdentificationSet  set = NS::IdentificationSet::fromResource(sourceUrl, ResourceManager::instance()->mainModel(), ignoreList);
            // Add to the ACL of fiter model
            d->filterModel->addTarget(fit.value());
            // Insert to the map of the identification sets
            d->resourceProxyISMap.insert(sourceUrl, set);
        }
        //kDebug() << "Resource " << sourceUrl << " has already been copied";
    }

    QUrl answer = fit.value();
    Q_ASSERT(!answer.isEmpty());
    return answer;

}

Nepomuk::Resource NW::Decision::proxyResource(const Nepomuk::Resource & res)
{
    // Call proxyUrl
    QUrl answer = proxyUrl(res);
    // Create resoruce and return it.
    return Nepomuk::Resource(answer, QUrl(), d->manager);
}

QHash<QUrl, QUrl> NW::Decision::proxies() const
{
    return d->resourceProxyMap;

}

/*
void NW::Decision::addStatement(const Soprano::Statement & statement, double rank)
{
    PropertiesGroup grp;
    grp << statement;
    grp.setRank(rank);
    addGroup(grp);
}
*/
/*
void NW::Decision::addGroup( const PropertiesGroup & grp)
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

NW::DecisionApplicationRequest * NW::Decision::applicationRequest(Soprano::Model * targetModel) const
{
    Q_ASSERT(targetModel);
    if(!isValid())
        return 0;

    // Get changelog
    NS::ChangeLog log = this->log();

    return new DecisionApplicationRequest(d, log, targetModel);
}


bool NW::Decision::apply(Soprano::Model * targetModel) const
{

    Q_ASSERT(targetModel);

    if(!isValid())
        return false;

    DecisionApplicationRequest * req = this->applicationRequest(targetModel);

    bool answer = req->apply();
    delete req;
    return answer;

}

void NW::Decision::addToUserDiscretion()
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

QList<QUrl> NW::Decision::mainResources() const
{
    return d->resourceProxyMap.values();
}

void NW::Decision::addAuthor(const DataPP * author)
{
    d->authorsData.insert(author);
}

unsigned int NW::qHash(const Nepomuk::WebExtractor::Decision & des)
{
    return des.d->hash;
}
