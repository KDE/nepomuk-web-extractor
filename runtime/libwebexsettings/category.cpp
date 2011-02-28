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

#include <QtCore/QSet>
#include <QtCore/QString>

#include <Nepomuk/Query/Query>

#include <KConfigGroup>
#include <KRandom>

namespace {
QString pluginSelectionTypeToString(Category::PluginSelectionType type) {
    if(type == Category::All )
        return QLatin1String("all");
    else
        return QLatin1String("stepwise");
}

Category::PluginSelectionType pluginSelectionTypeFromString(const QString& s) {
    if(s == QLatin1String("all"))
        return Category::All;
    else
        return Category::Stepwise;
}
}

class Category::Private
{
public:
    Private()
        : m_global(false),
          m_enabled(true),
          m_interval(1),
          m_maxResSimult(3),
          m_uCrit(0.5),
          m_aCrit(0.9),
          m_pluginSelectionType(All),
          m_pluginSelectionStep(5) {
    }

    bool m_global;
    QString m_id;

    bool m_enabled;
    QString m_name;
    Nepomuk::Query::Query m_query;
    QString m_queryDescription;
    int m_interval;
    uint m_maxResSimult;
    double m_uCrit;
    double m_aCrit;
    PluginSelectionType m_pluginSelectionType;
    int m_pluginSelectionStep;

    QList<DataPPDescr> m_plugins;

    Category* q;
};


Category::Category(bool global, QObject* parent)
    : QObject(parent),
      d(new Private)
{
    d->q = this;
    d->m_global = global;
}


Category::~Category()
{
    delete d;
}


bool Category::isValid() const
{
    // FIXME: a cat also needs a query and at least one plugin config!
    return !d->m_name.isEmpty();
}

bool Category::isGlobal() const
{
    return d->m_global;
}

bool Category::enabled() const
{
    return d->m_enabled;
}

void Category::setEnabled(bool enabled)
{
    if(d->m_enabled != enabled) {
        d->m_enabled = enabled;
        emit changed(this);
    }
}

QString Category::name() const
{
    return d->m_name;
}

void Category::setName(const QString &name)
{
    // global categories can be overwritten locally but not renamed
    if(d->m_name != name && !isGlobal()) {
        d->m_name = name;
        emit changed(this);
    }
}

Nepomuk::Query::Query Category::query() const
{
    return d->m_query;
}

void Category::setQuery(const Nepomuk::Query::Query &query)
{
    if(d->m_query != query) {
        d->m_query = query;
        emit changed(this);
    }
}

QString Category::queryDescription() const
{
    return d->m_queryDescription;
}

void Category::setQueryDescription(const QString &desc)
{
    if(d->m_queryDescription != desc) {
        d->m_queryDescription = desc;
        emit changed(this);
    }
}

int Category::interval() const
{
    return d->m_interval;
}

void Category::setInterval(int i)
{
    if(d->m_interval != i) {
        d->m_interval = i;
        emit changed(this);
    }
}

uint Category::maxResSimult() const
{
    return d->m_maxResSimult;
}

void Category::setMaxResSimult(uint step)
{
    if(d->m_maxResSimult != step) {
        d->m_maxResSimult = step;
        emit changed(this);
    }
}

double Category::uCrit() const
{
    return d->m_uCrit;
}

void Category::setUCrit(double uCrit)
{
    if(d->m_uCrit != uCrit) {
        d->m_uCrit = uCrit;
        emit changed(this);
    }
}

double Category::aCrit() const
{
    return d->m_aCrit;
}

void Category::setACrit(double aCrit)
{
    if(d->m_aCrit != aCrit) {
        d->m_aCrit = aCrit;
        emit changed(this);
    }
}

Category::PluginSelectionType Category::pluginSelectionType() const
{
    return d->m_pluginSelectionType;
}

void Category::setPluginSelectionType(Category::PluginSelectionType type)
{
    if(d->m_pluginSelectionType != type) {
        d->m_pluginSelectionType = type;
        emit changed(this);
    }
}

int Category::pluginSelectionStep() const
{
    return d->m_pluginSelectionStep;
}

void Category::setPluginSelectionStep(int step)
{
    if(d->m_pluginSelectionStep != step) {
        d->m_pluginSelectionStep = step;
        emit changed(this);
    }
}

QList<DataPPDescr> Category::plugins() const
{
    return d->m_plugins;
}

bool Category::operator==(const Category &other) const
{
    return (d->m_global == other.d->m_global &&
            d->m_enabled == other.d->m_enabled &&
            d->m_name == other.d->m_name &&
            d->m_query == other.d->m_query &&
            d->m_queryDescription == other.d->m_queryDescription &&
            d->m_interval == other.d->m_interval &&
            d->m_maxResSimult == other.d->m_maxResSimult &&
            d->m_uCrit == other.d->m_uCrit &&
            d->m_aCrit == other.d->m_aCrit &&
            d->m_pluginSelectionType == other.d->m_pluginSelectionType &&
            d->m_pluginSelectionStep == other.d->m_pluginSelectionStep &&
            d->m_plugins == other.d->m_plugins);
}

bool Category::operator!=(const Category &other) const
{
    return !operator==(other);
}

void Category::save(KConfigGroup& catGroup) const
{
    // save basic settings
    catGroup.writeEntry("id", identifier());
    catGroup.writeEntry("name", name());
    catGroup.writeEntry("query", query().toString());
    catGroup.writeEntry("query_desc", queryDescription());
    catGroup.writeEntry("interval", interval());
    catGroup.writeEntry("max_res_step", maxResSimult());
    catGroup.writeEntry("u_crit", uCrit());
    catGroup.writeEntry("a_crit", aCrit());
    catGroup.writeEntry("plugin_selection_type", pluginSelectionTypeToString(d->m_pluginSelectionType));
    catGroup.writeEntry("plugin_selection_step", pluginSelectionStep());
}

void Category::load(const KConfigGroup& catGroup)
{
    // read basic settings
    d->m_id = catGroup.readEntry("id", QString());
    setName(catGroup.readEntry("name", name()));
    setQuery(Nepomuk::Query::Query::fromString(catGroup.readEntry("query", query().toString())));
    setQueryDescription(catGroup.readEntry("query_desc", queryDescription()));
    setInterval(catGroup.readEntry("interval", interval()));
    setMaxResSimult(catGroup.readEntry("max_res_step", maxResSimult()));
    setUCrit(catGroup.readEntry("u_crit", uCrit()));
    setACrit(catGroup.readEntry("a_crit", aCrit()));
    setPluginSelectionType(pluginSelectionTypeFromString(catGroup.readEntry("plugin_selection_type", pluginSelectionTypeToString(pluginSelectionType()))));
    setPluginSelectionStep(catGroup.readEntry("plugin_selection_step", pluginSelectionStep()));

    QStringList groups = catGroup.groupList();
    foreach( const QString & groupName, groups)
    {
        KConfigGroup group = catGroup.group(groupName);
        DataPPDescr dpd = DataPPDescr::load(group);
        this->addPlugin(dpd);
    }
}


QDebug operator<<(QDebug& dbg, const Category& cat)
{
#warning IMPLEMENTME
    return dbg;
}

void Category::setPlugins(const QList<DataPPDescr> &plugins)
{
    d->m_plugins = plugins;
    for(int i = 0; i < d->m_plugins.count(); ++i )
        d->m_plugins[i].setCategory(this);
    emit changed(this);
}

void Category::addPlugin(const DataPPDescr& plugin)
{
    d->m_plugins << plugin;
    //d->m_plugins.last().setCategory(this);
    emit changed(this);
}

QString Category::identifier() const
{
    if(d->m_id.isEmpty())
        d->m_id = KRandom::randomString(10);
    return d->m_id;
}

#include "category.moc"
