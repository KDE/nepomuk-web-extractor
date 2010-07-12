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

#include "resourceservicedata.h"
#include <QSharedData>
#include <Nepomuk/Resource>
#include <Soprano/Node>
#include "ndco.h"

namespace NW = Nepomuk::WebExtractor;

class NW::ResourceServiceData::Private : public QSharedData
{
    public:
        ServiceDataBackend * backend;
};

NW::ResourceServiceData::ResourceServiceData(Nepomuk::Resource & res)
{
    d = new Private();
    d->backend = ResourceServiceDataManager::instance()->resourceData(res.resourceUri());
    //d->targetRes = res;
}

NW::ResourceServiceData::ResourceServiceData(ResourceServiceData & res):
    d(res.d)
{
    ;
}

NW::ResourceServiceData::ResourceServiceData(Nepomuk::Resource & res, ResourceServiceDataManager * manager)
{
    d = new Private();
    d->backend = manager->resourceData(res.resourceUri());
}

const NW::ResourceServiceData & NW::ResourceServiceData::operator=(const ResourceServiceData & res)
{
    d  = res.d;
    return *this;
}

NW::ResourceServiceData::~ResourceServiceData()
{
    ;
}

void NW::ResourceServiceData::clearServiceInfo()
{
    d->backend->clearServiceInfo();

}

void NW::ResourceServiceData::setExaminedDataPPInfo(const QString & dataPPName, const QString & dataPPVersion)
{
    d->backend->setExaminedDataPPInfo(dataPPName, dataPPVersion);
}

QMap< QString, QString > NW::ResourceServiceData::examinedDataPPInfo()
{
    return d->backend->examinedDataPPInfo();
}

void NW::ResourceServiceData::clearExaminedIfno()
{
    d->backend->clearExaminedIfno();
}

void NW::ResourceServiceData::clearExaminedIfno(const QString & dataPPName)
{
    d->backend->clearExaminedIfno(dataPPName);
}

bool NW::ResourceServiceData::isValid() const
{
    return d->backend->resource().exists();
}

QStringList NW::ResourceServiceData::serviceInfoPropertiesNames() const
{
    return d->backend->serviceInfoPropertiesNames();
}

QVariant NW::ResourceServiceData::property(const char * name) const
{
    return d->backend->property(name);
}
