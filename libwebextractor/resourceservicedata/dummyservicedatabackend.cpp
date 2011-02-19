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


#include "dummyservicedatabackend.h"

namespace NW = Nepomuk::WebExtractor;

NW::DummyServiceDataBackend::DummyServiceDataBackend( const Nepomuk::Resource & res):
    ServiceDataBackend(res)
{;}

void NW::DummyServiceDataBackend::setExaminedDataPPInfo(const QString & dataPPName, int dataPPVersion, const QDateTime & date)
{
    Q_UNUSED(dataPPName);
    Q_UNUSED(dataPPVersion);
    Q_UNUSED(date);
}

QMap< QString, int > NW::DummyServiceDataBackend::examinedDataPPInfo()
{
    return QMap<QString, int>();
}

bool NW::DummyServiceDataBackend::checkExaminedDataPPInfo(const QMap< QString, int> & dataPPInfo)
{
    Q_UNUSED(dataPPInfo);
    return false;
}

bool NW::DummyServiceDataBackend::checkExaminedDataPPInfo( const QString & name )
{
    Q_UNUSED(name);
    return false;
}

bool NW::DummyServiceDataBackend::checkExaminedDataPPInfo( const QString & name, int version )
{
    Q_UNUSED(name);
    Q_UNUSED(version);
    return false;
}

void NW::DummyServiceDataBackend::clearExaminedInfo()
{;}

void NW::DummyServiceDataBackend::clearExaminedInfo(const QString & name)
{
    Q_UNUSED(name);
}
