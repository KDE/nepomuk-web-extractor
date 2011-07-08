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

#include "decision.h"
#include "global.h"
#include "decisioncreator.h"
#include "decisioncreator_p.h"
#include "propertiesgroupcreator.h"

namespace ND = Nepomuk::Decision;




ND::DecisionCreator::DecisionCreator(
    const DecisionAuthor * parent
):
    d(new ND::DecisionCreatorInternals(parent))
{
    // Add default group
    PropertiesGroupCreator defGroup = newGroup();
    Q_ASSERT(defGroup.isValid());
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

void ND::DecisionCreator::setChanges( const SimpleResourceGraph & graph)
{
    defaultGroup().setChanges(graph);
}
