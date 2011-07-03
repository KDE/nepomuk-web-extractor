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

#include <QSharedData>
#include <Nepomuk/Resource>
#include <Soprano/Node>
#include <KDebug>

#include "resourceservicedata.h"
#include "ndco.h"

namespace NW = Nepomuk::WebExtractor;

class NW::ResourceServiceData::Private : public QSharedData
{
    public:
        ServiceDataBackend * backend;
};

NW::ResourceServiceData::ResourceServiceData()
{
    d = new Private();
    d->backend = ResourceServiceDataManager::instance()->resourceData(Nepomuk::Resource());
}


NW::ResourceServiceData::ResourceServiceData(const Nepomuk::Resource & res)
{
    d = new Private();
    d->backend = ResourceServiceDataManager::instance()->resourceData(res);
    //d->targetRes = res;
}

NW::ResourceServiceData::ResourceServiceData(const ResourceServiceData & res):
    d(res.d)
{
    ;
}

NW::ResourceServiceData::ResourceServiceData(const Nepomuk::Resource & res, ResourceServiceDataManager * manager)
{
    d = new Private();
    if ( !manager )
	manager = ResourceServiceDataManager::instance();

    d->backend = manager->resourceData(res);
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

void NW::ResourceServiceData::setExaminedDataPPInfo(const QString & dataPPName, int dataPPVersion)
{
    d->backend->setExaminedDataPPInfo(dataPPName, dataPPVersion);
}

QMap< QString, int > NW::ResourceServiceData::examinedDataPPInfo()
{
    return d->backend->examinedDataPPInfo();
}

void NW::ResourceServiceData::clearExaminedInfo()
{
    d->backend->clearExaminedInfo();
}

void NW::ResourceServiceData::clearExaminedInfo(const QString & dataPPName)
{
    d->backend->clearExaminedInfo(dataPPName);
}

bool NW::ResourceServiceData::checkExaminedDataPPInfo(const QMap< QString, int> & dataPP) const
{
    return d->backend->checkExaminedDataPPInfo(dataPP);
}

bool NW::ResourceServiceData::checkExaminedDataPPInfo(const QString  & name) const
{
    return d->backend->checkExaminedDataPPInfo(name);
}

bool NW::ResourceServiceData::checkExaminedDataPPInfo(const QString  & name, int version) const
{
    return d->backend->checkExaminedDataPPInfo(name, version);
}

bool NW::ResourceServiceData::isValid() const
{
    bool answer = d->backend->resource().exists();
    if (!answer) {
	kDebug() << "Resource " << d->backend->resource().resourceUri() << " doesn't exist";
    }
    return answer;
}

QStringList NW::ResourceServiceData::serviceInfoPropertiesNames() const
{
    return d->backend->serviceInfoPropertiesNames();
}

QVariant NW::ResourceServiceData::property(const char * name) const
{
    return d->backend->property(name);
}
QMap< QString, QDateTime > NW::ResourceServiceData::examinedDataPPDates()
{
    return d->backend->examinedDataPPDates();
}
QDateTime NW::ResourceServiceData::examinedDate(const QString & name)
{
    return d->backend->examinedDate(name);
}
