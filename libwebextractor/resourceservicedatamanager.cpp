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



#include "resourceservicedatamanager.h"
#include "nepomukservicedatabackend.h"
#include <QSharedData>

namespace NW = Nepomuk::WebExtractor;

class NW::ResourceServiceDataManager::Private : public QSharedData
{
    public:
        ServiceDataBackendFactory * backendFactory;
};

NW::ResourceServiceDataManager::ResourceServiceDataManager(ServiceDataBackendFactory * backendFactory)
{
    Q_ASSERT(backendFactory);
    d = new Private();
    d->backendFactory = backendFactory;
}

NW::ResourceServiceDataManager::ResourceServiceDataManager(const ResourceServiceDataManager & rhs):
    d(rhs.d)
{
    ;
}

NW::ResourceServiceDataManager::~ResourceServiceDataManager()
{
}

NW::ResourceServiceDataManager * NW::ResourceServiceDataManager::instance()
{
    static ResourceServiceDataManager * defaultManager = new ResourceServiceDataManager(new ServiceDataBackendFactoryTemplate<NepomukServiceDataBackend>());
    return defaultManager;
}

void NW::ResourceServiceDataManager::setOverrideBackend(ServiceDataBackendFactory * backendFactory)
{
    Q_ASSERT(backendFactory);
    d->backendFactory = backendFactory;
}

NW::ServiceDataBackend * NW::ResourceServiceDataManager::resourceData(const QUrl & url)
{
    return d->backendFactory->backend(url);
}
