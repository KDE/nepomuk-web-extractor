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


#include "decisiondata.h"
#include <KDebug>
#include "ndco.h"

namespace NW = Nepomuk::WebExtractor;
namespace NS = Nepomuk::Sync;

NW::DecisionData::DecisionData():
    manager(0),
    decisionsModel(0),
    filterModel(0),
    identsetManager(0)
{
}

NW::DecisionData::DecisionData(
    const DataPP * parent,
    Soprano::Model * decisionsModel,
    IdentificationSetManager * identsetManager
)
{
    Q_ASSERT(parent);
    Q_ASSERT(decisionsModel);
    Q_ASSERT(identsetManager);

    this->authorsData.insert(parent);

    this->timeStamp = QTime::currentTime();
    // Store original model
    this->decisionsModel = decisionsModel;
    // Create filter model
    this->filterModel = new NS::ChangeLogFilterModel(0, decisionsModel, QSet<QUrl>(), NS::ChangeLogFilterModel::Decline);

    // Create manager over it
    this->manager = ResourceManager::createManagerForModel(filterModel);

    // TODO May be it is necessary to create new context for Decision not after Decision
    // was created, but after first PropertyGroup or proxy resource was created for
    // this Decision.This will help to avoid empty Decisions in the model
    this->contextUrl = manager->generateUniqueUri();
    //TODO Resources must be created inside the context url
    this->decisionRes = Nepomuk::Resource(contextUrl, NW::Vocabulary::NDCO::Decision(), manager);
    this->identsetManager = identsetManager;
    setFreeze(false);
}

NW::DecisionData::~DecisionData()
{
    ;
    //kDebug() << "DecisionData is destroyed";
    if(manager)
        manager->deleteInstance();
    delete filterModel;
}

bool NW::DecisionData::isFreezed() const
{
    return m_freeze;
}

void NW::DecisionData::setFreeze(bool val)
{
    m_freeze = val;
}

NW::PropertiesGroup NW::DecisionData::setCurrentGroup(const PropertiesGroup & group)
{
    PropertiesGroup oldGroup = this->m_currentGroup;
    m_currentGroup = group;
    if(!group.isValid()) {
        filterModel->setLog(0);
    } else {
        filterModel->setLog(group.logPtr());
    }
    return oldGroup;
}

NW::PropertiesGroup NW::DecisionData::resetCurrentGroup()
{
    PropertiesGroup oldGroup = this->m_currentGroup;
    filterModel->setLog(0);
    m_currentGroup = PropertiesGroup();
    return oldGroup;
}

NW::PropertiesGroup NW::DecisionData::currentGroup() const
{
    return this->m_currentGroup;
}
#if 0
QUrl NW::DecisionData::createPropertiesGroupUrl()
{
    QUrl answer = manager->generateUniqueUri("grp");
    // Then attach the group to the Decision
    Soprano::Statement st(this->contextUrl, NW::Vocabulary::NDCO::hasPropertiesGroup(), answer, this->contextUrl);
    Soprano::Error::ErrorCode error = this->model->addStatement(st);
    if(error != Soprano::Error::ErrorNone) {
        kError() << " An error when adding PropertiesGroup to the Decision has occured ";
        return QUrl();
    }

    return answer;
}

void NW::DecisionData::registerGroup(PropertiesGroup * ptr)
{
    QUrl uri = createPropertiesGroupUrl();
    ptr->setUrl(uri);
    data.insert(*ptr);
}
#endif
