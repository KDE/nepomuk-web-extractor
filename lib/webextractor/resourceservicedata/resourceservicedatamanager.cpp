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



#include "resourceservicedatamanager.h"
#include "nepomukservicedatabackend.h"
#include "nepomukservicedatabackendfactory.h"
#include <QSharedData>
#include <Nepomuk/Resource>

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
    static ResourceServiceDataManager * defaultManager =
        new ResourceServiceDataManager(
        new NepomukServiceDataBackendFactory()
    );
    return defaultManager;
}

void NW::ResourceServiceDataManager::setOverrideBackend(ServiceDataBackendFactory * backendFactory)
{
    Q_ASSERT(backendFactory);
    d->backendFactory = backendFactory;
}

NW::ServiceDataBackend * NW::ResourceServiceDataManager::resourceData(const Nepomuk::Resource & res)
{
    return d->backendFactory->backend(res);
}

Nepomuk::Query::Query NW::ResourceServiceDataManager::queryUnparsedResources(const Nepomuk::Query::Term & mainTerm, const QMap<QString, int> & assignedDataPP, Soprano::Model * model)
{
    return d->backendFactory->queryUnparsedResources(mainTerm, assignedDataPP, model);
}
