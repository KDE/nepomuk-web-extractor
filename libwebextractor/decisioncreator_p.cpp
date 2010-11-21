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


#include "algorithm.h"
#include <KDebug>
#include "ndco.h"
#include "decisioncreator_p.h"
#include "changelogfiltermodel.h"

namespace NW = Nepomuk::WebExtractor;
namespace NS = Nepomuk::Sync;

NW::DecisionCreatorInternals::DecisionCreatorInternals():
    manager(0),
    decisionsModel(0),
    filterModel(0),
    identsetManager(0)
{
}

NW::DecisionCreatorInternals::DecisionCreatorInternals(
    const DataPP * parent,
    Soprano::Model * decisionsModel,
    IdentificationSetManager * identsetManager
    )
{
    Q_ASSERT(parent);
    Q_ASSERT(decisionsModel);
    Q_ASSERT(identsetManager);

    this->authorsData.insert(parent);

    this->m_data.setTimeStamp(QTime::currentTime());
    // Store original model
    this->decisionsModel = decisionsModel;

    this->identsetManager = identsetManager;
}

NW::DecisionCreatorInternals::~DecisionCreatorInternals()
{
    ;
    //kDebug() << "DecisionCreatorInternals is destroyed";
    /*
    if(manager)
        manager->deleteInstance();
    delete filterModel;
    */
}

/*
bool NW::DecisionCreatorInternals::isFreezed() const
{
    return m_freeze;
}

void NW::DecisionCreatorInternals::setFreeze(bool val)
{
    m_freeze = val;
}
*/

/* ==== Constructing methods ==== */
NW::Decision NW::DecisionCreatorInternals::data()
{
    Decision d = m_data;
    // Add groups, Create common log
    NS::ChangeLog commonLog ;
    kDebug() << "Group creators count: " << groupCreators.size();
    foreach(const PropertiesGroupCreator & grp, groupCreators) {
	PropertiesGroup pg = grp.data();
	Q_ASSERT(pg.isValid());
	Q_ASSERT(pg.isEmpty() == grp.isEmpty());
	d.addGroup(pg);
	commonLog << grp.log();
    }
    // Add auxiliaryIdentificationSet
    QSet<QUrl> ignoreset;
    QHash<QUrl, NS::IdentificationSet>::const_iterator isit =
        d.identificationSets().begin();
    QHash<QUrl, NS::IdentificationSet>::const_iterator isit_end =
        d.identificationSets().end();
    // Iteration is done over identificationSets()
    // ( the hash proxy resource -> identification set ),
    for(; isit != isit_end; isit++) {
        ignoreset << resourceProxyMap[isit.key()];
    }

    d.setAuxiliaryIdentificationSet(NS::IdentificationSet::fromChangeLog(commonLog, decisionsModel, ignoreset));

    // Set proxy map
    d.setResourceProxyMap( this->resourceProxyMap );

    Q_ASSERT(d.isValid());
    return d;
}

/* ==== Working with groups ==== */
NW::PropertiesGroupCreator NW::DecisionCreatorInternals::newGroup(QWeakPointer<DecisionCreatorInternals> weakRef)
{
    if ( !isValid() ) {
	kDebug() << "Create dummy PGC";
	return NW::PropertiesGroupCreator();
    }

    kDebug() << "Create real PGC";
    Q_ASSERT(!weakRef.isNull());

    // First create new property group
    PropertiesGroupCreator answer = PropertiesGroupCreator(weakRef);
    Q_ASSERT(answer.isValid());
    // Add it to the list
    groupCreators.append(answer);

    // Return it
    return answer;
}

/*
NW::PropertiesGroup NW::DecisionCreatorInternals::setCurrentGroup(const PropertiesGroup & group)
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

NW::PropertiesGroup NW::DecisionCreatorInternals::resetCurrentGroup()
{
    PropertiesGroup oldGroup = this->m_currentGroup;
    filterModel->setLog(0);
    m_currentGroup = PropertiesGroup();
    return oldGroup;
}

NW::PropertiesGroup NW::DecisionCreatorInternals::currentGroup() const
{
    return this->m_currentGroup;
}
*/

bool NW::DecisionCreatorInternals::isValid() const
{
    //return (!contextUrl.isEmpty()) and(!authorsData.isEmpty());
    return (!authorsData.isEmpty());
}

/* ==== Editing Decision ==== */

#if 0
QString NW::DecisionCreatorInternals::description() const
{
    return data->description();
}


void NW::DecisionCreatorInternals::setDescription(const QString & description)
{
    data->setDescription(description);
}

#endif 


QUrl NW::DecisionCreatorInternals::proxyUrl(const Nepomuk::Resource & res)
{
    // Check that decision is valid
    if(!isValid())
        return QUrl();

    if (!res.isValid()) 
        return QUrl();

    if (!res.exists())
        return QUrl();

    /*
    QMap< QUrl, QUrl>::iterator it = d->resourceProxyUrlMap.find(res.resourceUri());
    if(it != d->resourceProxyUrlMap.end())
        return it.value();
    */
    QUrl sourceUrl = res.resourceUri();

    // Now we should create/obtain a IdentificationSet for original resource
    // and perform a deep copy of the original resource to the decisions model
    QHash<QUrl, QUrl>::const_iterator fit =
        resourceProxyMap.find(sourceUrl);

    QUrl proxyUrl;

    if(fit == resourceProxyMap.end()) { // Resource not found


        // First disable any current group
        //PropertiesGroup save = resetCurrentGroup();
        // Perform actual copying
        QUrl newUrl =
            Nepomuk::deep_resource_copy_adjust(res, manager, &(resourceProxyMap))->operator[](res.resourceUri());
        //kDebug() << "Proxy for " << res.resourceUri() << " is: " << newUrl;
        Q_ASSERT(!newUrl.isEmpty());

        // Restore current group
        //setCurrentGroup(save);

        // Add to the list of copied resources
        resourceProxyMap.insert(sourceUrl, newUrl);

        // Create ignore list
        QSet<QUrl> ignoreList = m_data.identificationSets().keys().toSet();

        // Create identification set
        NS::IdentificationSet  set = NS::IdentificationSet::fromResource(sourceUrl, ResourceManager::instance()->mainModel(), ignoreList);
        // Add url to the ACL of the filter log model
        updateModels(newUrl);

        m_data.addIdentificationSet(sourceUrl, set);

        Q_ASSERT(decisionsModel);
        proxyUrl = newUrl;
    } else {
        // Now it is possible situation that resource was copied,
        // but it wasn't marked as target  resource and it's identification set
        // was not created.
        proxyUrl = fit.value();
        if(!m_data.identificationSets().contains(proxyUrl)) {
            // Create ignore list
            QSet<QUrl> ignoreList = m_data.identificationSets().keys().toSet();
            // Create identification set
            NS::IdentificationSet  set = NS::IdentificationSet::fromResource(sourceUrl, ResourceManager::instance()->mainModel(), ignoreList);
            // Add to the ACL of fiter model
            updateModels(proxyUrl);
            // Insert to the map of the identification sets
            m_data.addIdentificationSet(sourceUrl, set);
        }
        //kDebug() << "Resource " << sourceUrl << " has already been copied";
	Q_ASSERT(!proxyUrl.isEmpty());
    }

    kDebug() << "Source url is: " << sourceUrl << "Proxy url is: " <<proxyUrl;
    return proxyUrl;

}


void NW::DecisionCreatorInternals::updateModels(const QUrl & target)
{
    filterModel->addTarget(target);
    foreach(const PropertiesGroupCreator & grp, groupCreators) {
        Sync::ChangeLogFilterModel * model =
            grp.filterModel();
        if(model)
            model->addTarget(target);
    }
}

void NW::DecisionCreatorInternals::initContextUrl()
{
    /*
    if (contextUrl.size())
	return;

    // TODO May be it is necessary to create new context for DecisionCreator not after DecisionCreator
    // was created, but after first PropertyGroup or proxy resource was created for
    // this DecisionCreator.This will help to avoid empty DecisionCreators in the model
    this->contextUrl = manager->generateUniqueUri();
    //TODO Resources must be created inside the context url. Or not
    this->decisionRes = Nepomuk::Resource(contextUrl, NW::Vocabulary::NDCO::Decision(), manager);
    */
}
