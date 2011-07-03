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
#include <QtCore/QSharedData>
#include <QtCore/QTime>
#include <QtCore/QList>
#include <QtCore/QMultiMap>
#include <KDebug>
#include "mergerequest.h"
#include "changelog.h"

#include "decision.h"
#include "global.h"
#include "changelogfiltermodel.h"
#include "decisioncreator.h"
#include "decisioncreator_p.h"
#include "propertiesgroupcreator.h"
#include "identsetmanager.h"

namespace ND = Nepomuk::Decision;
namespace NS = Nepomuk::Sync;



ND::DecisionCreator::DecisionCreator(
    const DecisionAuthor * parent,
    Soprano::Model * decisionsModel,
    IdentificationSetManager * identsetManager
):
    d(new ND::DecisionCreatorInternals(parent, decisionsModel, identsetManager))
{
    // Add default group
    PropertiesGroupCreator defGroup = newGroup();
    Q_ASSERT(defGroup.isValid());
    // Init manager and model. They will use this default group
    // We need first to init manager. This will init model too
    d->manager = defGroup.manager();
    Q_ASSERT(d->manager);

    // set model
    d->filterModel = defGroup.filterModel();
    Q_ASSERT(d->filterModel);

}

ND::DecisionCreator::DecisionCreator(
):
    d(new ND::DecisionCreatorInternals())
{
};


ND::DecisionCreator::~DecisionCreator()
{
}

ND::DecisionCreator::DecisionCreator(const DecisionCreator & rhs)
{
    d = rhs.d;
}

ND::DecisionCreator & Nepomuk::Decision::DecisionCreator::operator=(const DecisionCreator & rhs)
{
    this->d = rhs.d;
    return *this;
}

bool ND::DecisionCreator::operator==(const DecisionCreator & rhs) const
{
    if(this == &rhs)
        return true;
    if(this->d == rhs.d)
        return true;

    return false;
}

bool ND::DecisionCreator::operator!=(const DecisionCreator & rhs)const
{
    return !(*this == rhs);
}

bool ND::DecisionCreator::isEmpty() const
{
    foreach(const PropertiesGroupCreator & grp, d->groupCreators) {
	if (!grp.isEmpty()) {
	    return true;
	}
    }
    return false;

}

bool ND::DecisionCreator::isValid() const
{
    return d->isValid();
}

void ND::DecisionCreator::setRank(double rank)
{
    rank = boundRank(rank);

    d->m_data.setRank(rank);
}

double ND::DecisionCreator::rank() const
{
    return d->m_data.rank();
}

/*
QUrl ND::DecisionCreator::uri() const
{
    return d->contextUrl;
}
*/

Nepomuk::ResourceManager * ND::DecisionCreator::manager() const
{
    return d->manager;
}


Soprano::Model * ND::DecisionCreator::model() const
{
    return d->filterModel;
}

QString ND::DecisionCreator::description() const
{
    return d->m_data.description();
}


void ND::DecisionCreator::setDescription(const QString & description)
{
    d->m_data.setDescription(description);
}

ND::Decision ND::DecisionCreator::data() const
{
    return d->data();
}


ND::PropertiesGroupCreator ND::DecisionCreator::defaultGroup() 
{
    if (isValid()) {
	Q_ASSERT(d->groupCreators.size());
	return d->groupCreators[0];
    }
    else {
	return PropertiesGroupCreator();
    }

}

ND::PropertiesGroupCreator ND::DecisionCreator::newGroup()
{
    return d->newGroup(d.toWeakRef());
}

/*
QList<QUrl> ND::DecisionCreator::groupsUrls() const
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
void ND::DecisionCreator::setCurrentGroup( const PropertiesGroup & group)
{
    d->setCurrentGroup(group);
}
*/

/*
void ND::DecisionCreator::resetCurrentGroup()
{
    d->resetCurrentGroup();
}
*/

QUrl ND::DecisionCreator::proxyUrl(const Nepomuk::Resource & res)
{
    return d->proxyUrl(res);
}

Nepomuk::Resource ND::DecisionCreator::proxyResource(const Nepomuk::Resource & res)
{
    // Call proxyUrl
    QUrl answer = proxyUrl(res);
    // Create resoruce and return it.
    return Nepomuk::Resource(answer, QUrl(), d->manager);
}

QHash<QUrl, QUrl> ND::DecisionCreator::proxies() const
{
    return d->resourceProxyMap;

}

/*
void ND::DecisionCreator::addStatement(const Soprano::Statement & statement, double rank)
{
    PropertiesGroup grp;
    grp << statement;
    grp.setRank(rank);
    addGroup(grp);
}
*/
/*
void ND::DecisionCreator::addGroup( const PropertiesGroup & grp)
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

/*
void ND::DecisionCreator::freeze()
{
    this->d->setFreeze(true);
}

bool ND::DecisionCreator::isFreezed() const
{
    return this->d->isFreezed();
}
*/

#if 0
void ND::DecisionCreator::addToUserDiscretion()
{
    /*
    kDebug() << "Write DecisionCreator to user discretion list";
    foreach ( const PropertiesGroup  &  lst, d->data )
    {
    foreach( const Soprano::Statement  & st, lst.data() )
    {
        kDebug() << st;
    }
    }
    */
}

#endif

QList<QUrl> ND::DecisionCreator::mainResources() const
{
    return d->resourceProxyMap.values();
}

/*
void ND::DecisionCreator::addAuthor(const Executive * author)
{
    d->authorsData.insert(author);
}
*/

/*
unsigned int ND::qHash(const Nepomuk::Decision::DecisionCreator & des)
{
    return des.d->hash;
}
*/
