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

#include "changelog.h"

#include "propertiesgroup.h"
#include "global.h"

#include <nepomuk/simpleresourcegraph.h>


namespace ND = Nepomuk::Decision;
namespace NS = Nepomuk::Sync;

class ND::PropertiesGroup::Private : public QSharedData
{
    public:
        Private();
        // Rank of the group
        double rank;
        // Changelog. All changes are stored there
        SimpleResourceGraph changes;

        // Description of the group
        QString description;


};

ND::PropertiesGroup::Private::Private():
    rank(0)
{;}



ND::PropertiesGroup::PropertiesGroup():
    d(new Private())
{;}

ND::PropertiesGroup::PropertiesGroup(const SimpleResourceGraph & changes, const QString & description, double rank ):
    d(new Private())
{
    d->description = description;
    d->changes = changes;
    d->rank = rank;
}

ND::PropertiesGroup::PropertiesGroup(const PropertiesGroup & rhs):
    d(rhs.d)
{;}

const ND::PropertiesGroup & ND::PropertiesGroup::operator=( const PropertiesGroup & rhs)
{
    d = rhs.d;
    return *this;
}

ND::PropertiesGroup::~PropertiesGroup()
{;}

QString ND::PropertiesGroup::description() const
{
    return d->description;
}

void ND::PropertiesGroup::setRank(double rank)
{
    d->rank = boundRank(rank);
}

/*
NS::ChangeLog* ND::PropertiesGroup::logPtr() const
{
    return &log;
}
*/

void ND::PropertiesGroup::setDescription( const QString & description )
{
    d->description = description;
}

void ND::PropertiesGroup::setChanges( const Nepomuk::SimpleResourceGraph & changes)
{
    d->changes = changes;
}

bool ND::PropertiesGroup::isValid() const
{
    return true;
    //return !d->log.empty();
}

bool ND::PropertiesGroup::isEmpty() const
{
    return d->changes.isEmpty();
}

Nepomuk::SimpleResourceGraph ND::PropertiesGroup::changes() const
{
    return d->changes;
}

QDataStream & ND::operator<<(QDataStream & stream, const ND::PropertiesGroup & pg)
{
    stream << pg.d->description << pg.d->rank << pg.d->changes; 
    return stream;
}

QDataStream & ND::operator>>(QDataStream & stream, ND::PropertiesGroup & pg )
{
    stream >> pg.d->description >> pg.d->rank >> pg.d->changes; 
    return stream;
}
