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

#ifndef _NEPOMUK_WEBEXTRCT_IDENTSET_MANAGER_H_
#define _NEPOMUK_WEBEXTRCT_IDENTSET_MANAGER_H_

#include <QSharedPointer>

#include <Nepomuk/ResourceManager>

namespace Nepomuk
{
    namespace Sync {
        class IdentificationSet;
    }
    
    namespace WebExtractor
    {
        /*! \brief This class manages IdentificationSets
         * This is class for internal usage. Each DecisionFactory
         * has such cache. When new resource should be copied
         * from the main nepomuk model to the decisions model, the
         * IdentificationSet of this resources is put here. If there
         * is already IdentificationSet for the resource and the resource
         * has not changed, then ptr to this IdentificationSet will be
         * returned. If the resource has been updated, then new IdentificationSet
         * will be created, onl one will be marked as obsolete and ptr to new
         * one will be returned.
         */
        class IdentificationSetManager
        {
            public:
                IdentificationSetManager(Soprano::Model * mainModel = ResourceManager::instance()->mainModel());
                ~IdentificationSetManager();
                Sync::IdentificationSet identificationSet(const QUrl & resourceUrl);
            private:
                // No implementation
                IdentificationSetManager(const IdentificationSetManager &);
                IdentificationSetManager & operator=(const IdentificationSetManager &);

                class Private;
                Private * d;
        };
    }
}
#endif


