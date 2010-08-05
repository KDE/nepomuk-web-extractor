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

#include "identsetmanager.h"
#include <QHash>
// This is for qHash(QUrl)
#include <Soprano/Node>

namespace NW = Nepomuk::WebExtractor;
namespace NS = Nepomuk::Sync;

class NW::IdentificationSetManager::Private 
{
    public:
    QHash< QUrl, IdentificationSetPtr > sets;
    Soprano::Model * mainModel;
};


NW::IdentificationSetManager::IdentificationSetManager( Soprano::Model * mainModel ):
    d(new IdentificationSetManager::Private() )
{
    Q_ASSERT(mainModel);
    d->mainModel = mainModel;
}

NW::IdentificationSetManager::~IdentificationSetManager()
{
    delete d;
}

NW::IdentificationSetPtr NW::IdentificationSetManager::identificationSet( const QUrl & resourceUrl )
{
    Q_ASSERT(d->mainModel);
    // Check that this indentifications set exists in the manager
    QHash<QUrl, IdentificationSetPtr>::const_iterator it =
	d->sets.find(resourceUrl);
    if ( it != d->sets.end() ) {
	return it.value();
    }
    else {
	// Create new IdentificationSet for resource
	IdentificationSetPtr ptr = IdentificationSetPtr( new NS::IdentificationSet() );
	*(ptr.data()) = NS::IdentificationSet::create(resourceUrl, d->mainModel );
       // Insert ptr
       d->sets.insert(resourceUrl, ptr);
       return ptr;
    }
	
}
