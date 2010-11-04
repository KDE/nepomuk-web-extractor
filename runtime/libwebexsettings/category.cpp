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

#include "category.h"
#include "datappdescr.h"

#include <QtCore/QSharedData>
#include <QtCore/QList>
#include <QtCore/QString>

#include <Nepomuk/Query/Query>

class Category::Private : public QSharedData
{
public:
    Private()
        : m_interval(1),
          m_maxResStep(3),
          m_uCrit(0.5),
          m_pluginSelectionType(All),
          m_pluginSelectionStep(5) {
    }

    QString m_name;
    Nepomuk::Query::Query m_query;
    QString m_queryDescription;
    int m_interval;
    int m_maxResStep;
    double m_uCrit;
    PluginSelectionType m_pluginSelectionType;
    int m_pluginSelectionStep;

    QList<DataPPDescr> m_plugins;
};

Category::Category()
    : d(new Private)
{
}

Category::Category(const Category& other)
{
    d = other.d;
}

Category::~Category()
{
}

Category& Category::operator=(const Category &other)
{
    d = other.d;
    return *this;
}

bool Category::isValid() const
{
    // FIXME: a cat also needs a query and at least one plugin config!
    return !d->m_name.isEmpty();
}

QString Category::name() const
{
    return d->m_name;
}

void Category::setName(const QString &name)
{
    d->m_name = name;
}

Nepomuk::Query::Query Category::query() const
{
    return d->m_query;
}

void Category::setQuery(const Nepomuk::Query::Query &query)
{
    d->m_query = query;
}

QString Category::queryDescription() const
{
    return d->m_queryDescription;
}

void Category::setQueryDescription(const QString &desc)
{
    d->m_queryDescription = desc;
}

int Category::interval() const
{
    return d->m_interval;
}

void Category::setInterval(int i)
{
    d->m_interval = i;
}

int Category::maxResStep() const
{
    return d->m_maxResStep;
}

void Category::setMaxResSimult(int step)
{
    d->m_maxResStep = step;
}

double Category::uCrit() const
{
    return d->m_uCrit;
}

void Category::setUCrit(double uCrit)
{
    d->m_uCrit = uCrit;
}

Category::PluginSelectionType Category::pluginSelectionType() const
{
    return d->m_pluginSelectionType;
}

void Category::setPluginSelectionType(Category::PluginSelectionType type)
{
    d->m_pluginSelectionType = type;
}

int Category::pluginSelectionStep() const
{
    return d->m_pluginSelectionStep;
}

void Category::setPluginSelectionStep(int step)
{
    d->m_pluginSelectionStep = step;
}

QList<DataPPDescr> Category::plugins() const
{
    return d->m_plugins;
}

void Category::setPlugins(const QList<DataPPDescr> &plugins)
{
    d->m_plugins = plugins;
}

void Category::addPlugin(const DataPPDescr &plugin)
{
    d->m_plugins << plugin;
}

bool Category::operator==(const Category &other) const
{
    return (d->m_name == other.d->m_name &&
            d->m_query == other.d->m_query &&
            d->m_queryDescription == other.d->m_queryDescription &&
            d->m_interval == other.d->m_interval &&
            d->m_maxResStep == other.d->m_maxResStep &&
            d->m_uCrit == other.d->m_uCrit &&
            d->m_pluginSelectionType == other.d->m_pluginSelectionType &&
            d->m_pluginSelectionStep == other.d->m_pluginSelectionStep &&
            d->m_plugins == other.d->m_plugins);
}

bool Category::operator!=(const Category &other) const
{
    return !operator==(other);
}
