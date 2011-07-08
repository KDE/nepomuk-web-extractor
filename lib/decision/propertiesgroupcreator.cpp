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

#include "propertiesgroupcreator.h"
#include "propertiesgroupcreator_p.h"
#include "decisioncreator_p.h"
#include "global.h"

namespace ND = Nepomuk::Decision;



ND::PropertiesGroupCreator::PropertiesGroupCreator( QWeakPointer<DecisionCreatorInternals> internals):
   d(new ND::PropertiesGroupCreatorPrivate())
{
    Q_ASSERT(!internals.isNull());
    this->internals = internals;


}

ND::PropertiesGroupCreator::PropertiesGroupCreator():
   d(new ND::PropertiesGroupCreatorPrivate())
{
}

ND::PropertiesGroupCreator::~PropertiesGroupCreator()
{;}

ND::PropertiesGroupCreator::PropertiesGroupCreator(const PropertiesGroupCreator & rhs)
{
    this->d = rhs.d;
    this->internals = rhs.internals;
}

const ND::PropertiesGroupCreator & ND::PropertiesGroupCreator::operator=(const PropertiesGroupCreator & rhs)
{
    if(this == &rhs)
        return *this;
    this->d = rhs.d;
    this->internals = rhs.internals;
    return *this;
}


double ND::PropertiesGroupCreator::rank() const
{
    return d->rank;
}

void ND::PropertiesGroupCreator::setRank(double newRank)
{
    d->rank = boundRank(newRank);
}

/*
QUrl ND::PropertiesGroupCreator::uri() const
{
    return d->url;
}
*/

bool ND::PropertiesGroupCreator::isValid() const
{
    return !internals.isNull();
}

bool ND::PropertiesGroupCreator::isEmpty() const
{
    return d->changes.isEmpty();
}

QString ND::PropertiesGroupCreator::description() const
{
    return d->description;
}

void ND::PropertiesGroupCreator::setDescription(const QString & description)
{
    d->description = description;
}

void ND::PropertiesGroupCreator::setChanges( const SimpleResourceGraph & graph)
{
    d->changes = graph;
}


/* Disabled as unstable 
Nepomuk::SimpleResourceGraph ND::PropertiesGroupCreator::changes() const
{
    return d->changes;
}
*/

ND::PropertiesGroup ND::PropertiesGroupCreator::data() const
{
    return PropertiesGroup(d->changes,d->description,d->rank);
}


bool ND::PropertiesGroupCreator::operator==(const PropertiesGroupCreator & rhs) const
{
    if(this == &rhs)
        return true;

    if(this->d == rhs.d)
        return true;

#warning IMPLEMENT THIS!
    return false; /*(this->d->mainTarget == rhs.d.mainTarget ) and */
    //(this->d->statements == rhs.d->statements);
}


bool ND::PropertiesGroupCreator::operator!=(const PropertiesGroupCreator & rhs) const
{
    return !(*this == rhs);
}


ND::PropertiesGroupCreator & ND::operator<<(ND::PropertiesGroupCreator & grp, const Soprano::Statement & st)
{

    grp.d->changes.addStatement(st);
    return grp;
}

#if 0
unsigned int ND::qHash(const Nepomuk::Decision::PropertiesGroupCreator & g)
{
    return g.d->hash;
}
#endif
