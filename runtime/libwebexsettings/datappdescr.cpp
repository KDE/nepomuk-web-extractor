/*
   This file is part of the Nepomuk KDE project.
   Copyright (C) 2010 Sebastian Trueg <trueg@kde.org>

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) version 3, or any
   later version accepted by the membership of KDE e.V. (or its
   successor approved by the membership of KDE e.V.), which shall
   act as a proxy defined in Section 6 of version 3 of the license.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with this library.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "datappdescr.h"
#include "category.h"
#include "categoriespool.h"
#include "webextractor_plugin.h"

#include <KConfigGroup>
#include <KRandom>

class DataPPDescr::Private : public QSharedData
{
public:
    Private()
        : m_rank(1.0),
          m_coff(1.0),
          m_trusted(true),
          m_enabled(true),
          m_category(0) {
    }

    QString m_id;

    double m_rank;
    double m_coff;
    bool m_trusted;
    bool m_enabled;

    Category* m_category;
    KService::Ptr m_service;
};

DataPPDescr::DataPPDescr(KService::Ptr service)
    : d(new Private())
{
    d->m_service = service;
    d->m_id = KRandom::randomString(10);
}

DataPPDescr::DataPPDescr(const DataPPDescr& other)
    : d(other.d)
{
}

DataPPDescr::~DataPPDescr()
{
}


DataPPDescr & DataPPDescr::operator =(const DataPPDescr &other)
{
    d = other.d;
    return *this;
}

void DataPPDescr::save(KConfigGroup& config) const
{
    config.writeEntry("id", identifier());
    config.writeEntry("rank", rank());
    config.writeEntry("coff", coff());
    config.writeEntry("trusted", trusted());
    config.writeEntry("enabled", enabled());
    config.writeEntry("plugin", service()->name());
    if(category())
        config.writeEntry("category", category()->identifer());
}

// static
DataPPDescr DataPPDescr::load(const KConfigGroup &config, Nepomuk::CategoriesPool* pool)
{
    DataPPDescr datapp(pool->pluginByName(config.readEntry("plugin", QString())));
    datapp.d->m_id = config.readEntry("id", KRandom::randomString(10));
    datapp.d->m_rank = config.readEntry("rank", datapp.rank());
    datapp.d->m_coff = config.readEntry("coff", datapp.coff());
    datapp.d->m_trusted = config.readEntry("trusted", datapp.trusted());
    datapp.d->m_enabled = config.readEntry("enabled", datapp.enabled());
    datapp.d->m_category = pool->categoryById(config.readEntry("category", QString()));
    return datapp;
}

double DataPPDescr::rank() const
{
    return d->m_rank;
}

double DataPPDescr::coff() const
{
    return d->m_coff;
}

bool DataPPDescr::trusted() const
{
    return d->m_trusted;
}

bool DataPPDescr::enabled() const
{
    return d->m_enabled;
}

void DataPPDescr::setRank(double rank)
{
    d->m_rank = rank;
}

void DataPPDescr::setCoff(double coff)
{
    d->m_coff = coff;
}

void DataPPDescr::setTrusted(bool trusted)
{
    d->m_trusted = trusted;
}

void DataPPDescr::setEnabled(bool enabled)
{
    d->m_enabled = enabled;
}

bool DataPPDescr::operator==(const DataPPDescr& other) const
{
    return (service() == other.service() &&
            rank() == other.rank() &&
            coff() == other.coff() &&
            trusted() == other.trusted() &&
            enabled() == other.enabled());
}

Category * DataPPDescr::category() const
{
    return d->m_category;
}

KService::Ptr DataPPDescr::service() const
{
    return d->m_service;
}

void DataPPDescr::setCategory(Category *cat)
{
    d->m_category = cat;
}

bool DataPPDescr::isValid() const
{
    return d->m_category != 0 && !d->m_service.isNull();
}

QString DataPPDescr::identifier() const
{
    return d->m_id;
}

Nepomuk::WebExtractorPlugin * DataPPDescr::createPlugin() const
{
    KPluginFactory *factory = KPluginLoader(service()->library()).factory();
    if (factory) {
        return factory->create<Nepomuk::WebExtractorPlugin>();
    }
    else {
        kError() << "KPluginFactory could not load the plugin:" << service()->library();
        return 0;
    }
}
