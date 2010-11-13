/*
   Copyright (C) 2010 by Serebriyskiy Artem <v.for.vandal at gmail.com>

   This library is free software; you can redistribute it and/or modify
   it under the terms of the GNU Library General Public License as published by
   the Free Software Foundation; either version 3, or (at your option)
   any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#include <QtCore/QSet>
#include <QtCore/QSharedData>
#include <Soprano/Model>
#include <Nepomuk/ResourceManager>
#include <nepomuk/changelog.h>

#include "propertiesgroupcreator.h"
#include "propertiesgroupcreator_p.h"
#include "decisioncreator_p.h"
#include "changelogfiltermodel.h"
#include "global.h"

namespace NW = Nepomuk::WebExtractor;
namespace NS = Nepomuk::Sync;


NW::PropertiesGroupCreator::PropertiesGroupCreator( QWeakPointer<DecisionCreatorInternals> internals):
   d(new NW::PropertiesGroupCreatorPrivate())
{
    //Q_ASSERT(manager);
    Q_ASSERT(!internals.isNull());
    this->d->hash = 0;
    this->internals = internals;

    // By default filter model will be set to 0. When first time a manager function
    // will be accessed, this member will be initialized
    d->filterModel = 0;
    d->manager = 0;

}

NW::PropertiesGroupCreator::PropertiesGroupCreator():
   d(new NW::PropertiesGroupCreatorPrivate())
{
}

NW::PropertiesGroupCreator::~PropertiesGroupCreator()
{;}

NW::PropertiesGroupCreator::PropertiesGroupCreator(const PropertiesGroupCreator & rhs)
{
    this->d = rhs.d;
    this->internals = rhs.internals;
}

const NW::PropertiesGroupCreator & NW::PropertiesGroupCreator::operator=(const PropertiesGroupCreator & rhs)
{
    if(this == &rhs)
        return *this;
    this->d = rhs.d;
    this->internals = rhs.internals;
    return *this;
}

QUrl NW::PropertiesGroupCreator::proxyUrl(const Nepomuk::Resource & res)
{
    if (!isValid())
	return QUrl();

    // We need to init model here because call to the internals->proxyUrl()
    // will iterate over all properties group to add newly created 
    // proxyUrl to the logging models
    // If we don't call it, then nothing bad happens and when first time
    // model()/manager() will be called, the log will be initialized 
    // and all urls-to-log will be pulled from the internals anyway.
    // But usually user call proxyUrl when he want to work with Decision
    // so it looks smart enough to initialize it here.
    if(!d->filterModel)
        initFilterModel();

    return internals.toStrongRef()->proxyUrl(res);
}

Nepomuk::Resource NW::PropertiesGroupCreator::proxyResource(const Nepomuk::Resource & res)
{
    if (!isValid())
	return Nepomuk::Resource();

    // Call proxyUrl. This call is important, becaus
    // in proxyUrl filterModel will be initialize if
    // necessary
    QUrl answer = proxyUrl(res);

    if(!d->manager)
        initFilterManager();
    // Create resoruce and return it.
    return Nepomuk::Resource(answer, QUrl(), d->manager);
}

double NW::PropertiesGroupCreator::rank() const
{
    return d->rank;
}

void NW::PropertiesGroupCreator::setRank(double newRank)
{
    d->rank = boundRank(newRank);
}

/*
QUrl NW::PropertiesGroupCreator::uri() const
{
    return d->url;
}
*/

Nepomuk::ResourceManager * NW::PropertiesGroupCreator::manager()
{
    if(!isValid())
        return 0;

    if(!d->manager) {
        // This is first call. Create  a manager and a filter model
        initFilterManager();
    }
    return d->manager;
}

Soprano::Model * NW::PropertiesGroupCreator::model()
{
    if(!isValid())
        return 0;

    if(!d->filterModel) {
        // This is first call. Create  a manager and a filter model
        initFilterModel();
    }
    return d->filterModel;
}

bool NW::PropertiesGroupCreator::isValid() const
{
    return !internals.isNull();
}

bool NW::PropertiesGroupCreator::isEmpty() const
{
    return d->log.empty();
}

QString NW::PropertiesGroupCreator::description() const
{
    return d->description;
}

void NW::PropertiesGroupCreator::setDescription(const QString & description)
{
    d->description = description;
}

NS::ChangeLog NW::PropertiesGroupCreator::log() const
{
    return d->log;
}

NW::PropertiesGroup NW::PropertiesGroupCreator::data() const
{
    return PropertiesGroup(d->log,d->description,d->rank);
}

#if 0
void NW::PropertiesGroupCreator::makeCurrent()
{
    // Return if we are invalid
    if(!internals)
        return;

    //registerGroup();

    internals->setCurrentGroup(*this);
}

void NW::PropertiesGroupCreator::resetCurrent()
{
    if(!internals)
        return;

    internals->resetCurrentGroup();
}
#endif

NS::ChangeLogFilterModel * NW::PropertiesGroupCreator::filterModel() const
{
    return d->filterModel;
}

void NW::PropertiesGroupCreator::initFilterModel()
{
    QSharedPointer<DecisionCreatorInternals> internalsStrong = internals.toStrongRef();
    Q_ASSERT(internals);
    d->filterModel = new NS::ChangeLogFilterModel(&d->log, internalsStrong->decisionsModel, QSet<QUrl>(), NS::ChangeLogFilterModel::Decline);
    // Now take current proxy url and add them as targets
    foreach(const QUrl & proxyUrl, internalsStrong->resourceProxyMap) {
        d->filterModel->addTarget(proxyUrl);
    }

}

void NW::PropertiesGroupCreator::initFilterManager()
{
    Q_ASSERT(internals);
    if(!d->filterModel)
        initFilterModel();

    d->manager = ResourceManager::createManagerForModel(d->filterModel);
}
/*
QUrl NW::PropertiesGroupCreator::mainProxyResourceUrl()
{
    return d->mainProxyResourceUrl;
}
*/


bool NW::PropertiesGroupCreator::operator==(const PropertiesGroupCreator & rhs) const
{
    if(this == &rhs)
        return true;

    if(this->d == rhs.d)
        return true;

#warning IMPLEMENT THIS!
    return false; /*(this->d->mainTarget == rhs.d.mainTarget ) and */
    //(this->d->statements == rhs.d->statements);
}


bool NW::PropertiesGroupCreator::operator!=(const PropertiesGroupCreator & rhs) const
{
    return !(*this == rhs);
}


NW::PropertiesGroupCreator & NW::operator<<(NW::PropertiesGroupCreator & grp, const Soprano::Statement & st)
{

    if(!grp.model())
        return grp;

    grp.model()->addStatement(st);
    return grp;
}

#if 0
unsigned int NW::qHash(const Nepomuk::WebExtractor::PropertiesGroupCreator & g)
{
    return g.d->hash;
}
#endif
