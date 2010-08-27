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


#include "servicedatabackend.h"

namespace NW = Nepomuk::WebExtractor;

class NW::ServiceDataBackend::Private
{
    public:
        Nepomuk::Resource  res;
};

NW::ServiceDataBackend::ServiceDataBackend(const Nepomuk::Resource & res):
    d(new Private())
{
    d->res = res;
}

void NW::ServiceDataBackend::clearServiceInfo()
{
    clearExaminedInfo();
}

NW::ServiceDataBackend::~ServiceDataBackend()
{
    delete d;
}

QStringList NW::ServiceDataBackend::serviceInfoPropertiesNames() const
{
    return QStringList();
}

QMap< QString, QDateTime > NW::ServiceDataBackend::examinedDataPPDates()
{
    return QMap< QString, QDateTime >();
}

bool NW::ServiceDataBackend::checkExaminedDataPPInfo(const QMap< QString, int> & dataPP)
{
    return (examinedDataPPInfo() == dataPP);
}

bool NW::ServiceDataBackend::checkExaminedDataPPInfo(const QString & name)
{
    return (examinedDataPPInfo().contains(name));
}

bool NW::ServiceDataBackend::checkExaminedDataPPInfo(const QString & name, int version)
{
    QMap<QString, int> info = examinedDataPPInfo();
    return (info.contains(name) and(info[name] == version));
}

QDateTime NW::ServiceDataBackend::examinedDate(const QString & name)
{
    return QDateTime();
}

Nepomuk::Resource  NW::ServiceDataBackend::resource() const
{
    return d->res;
}


const QUrl & NW::ServiceDataBackend::uri() const
{
    return d->res.resourceUri();
}
