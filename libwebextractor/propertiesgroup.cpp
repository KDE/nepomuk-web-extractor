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


namespace NW = Nepomuk::WebExtractor;
namespace NS = Nepomuk::Sync;

class NW::PropertiesGroup::Private : public QSharedData
{
    public:
	Private();
	// Rank of the group
	double rank;
	// Changelog. All changes are stored there
	NS::ChangeLog log;

	// Description of the group
	QString description;


};

NW::PropertiesGroup::Private::Private():
    rank(0)
{;}



NW::PropertiesGroup::PropertiesGroup():
    d(new Private())
{;}

NW::PropertiesGroup::PropertiesGroup(const Nepomuk::Sync::ChangeLog & log, const QString & description, double rank ):
    d(new Private())
{
    d->description = description;
    d->log = log;
    d->rank = rank;
}

NW::PropertiesGroup::PropertiesGroup(const PropertiesGroup & rhs):
    d(rhs.d)
{;}

const NW::PropertiesGroup & NW::PropertiesGroup::operator=( const PropertiesGroup & rhs)
{
    d = rhs.d;
    return *this;
}

NW::PropertiesGroup::~PropertiesGroup()
{;}

QString NW::PropertiesGroup::description() const
{
    return d->description;
}

void NW::PropertiesGroup::setRank(double rank)
{
    d->rank = boundRank(rank);
}

/*
NS::ChangeLog* NW::PropertiesGroup::logPtr() const
{
    return &log;
}
*/

void NW::PropertiesGroup::setDescription( const QString & description )
{
    d->description = description;
}

void NW::PropertiesGroup::setLog( const Nepomuk::Sync::ChangeLog & log)
{
    d->log = log;
}

bool NW::PropertiesGroup::isValid() const
{
    return true;
    //return !d->log.empty();
}

bool NW::PropertiesGroup::isEmpty() const
{
    return d->log.empty();
}

NS::ChangeLog NW::PropertiesGroup::log() const
{
    return d->log;
}
