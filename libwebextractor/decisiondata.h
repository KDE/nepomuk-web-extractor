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

#ifndef __webextractor_decision_data_h_
#define __webextractor_decision_data_h_

#include <QTime>
#include <QSet>
#include <QUrl>
#include <QMap>
#include <Nepomuk/ResourceManager>
#include <Nepomuk/Resource>
#include <Soprano/Model>
#include "propertiesgroup.h"
#include "changelogfiltermodel.h"
#include "datapp.h"
#include "identsetmanager.h"

namespace Nepomuk
{
    namespace WebExtractor
    {
        // TODO Make a normal class from this one
        // Anyway it is not exported class
        class Decision;
        class PropertiesGroup;
        class DecisionData
        {
            public:
                DecisionData();
                DecisionData(const DataPP * parent,  Soprano::Model * decisionsModel, IdentificationSetManager * identsetManager);
                // TODO move rank to upper class because it it modificated frequently
                double rank;
                QSet<  PropertiesGroup > data;
                QSet<const DataPP*>  authorsData;
                unsigned int hash;
                QTime timeStamp;


                // Manager of the filter model
                ResourceManager * manager;

                // This is main Decisions model
                Soprano::Model * decisionsModel;

                // This is filtered model. It is used by manager.
                // The log used in this model is log of current PropertiesGroup
                // or 0 if there is no current PropertiesGroup
                Sync::ChangeLogFilterModel * filterModel;

                // This may is used  for storing pairs < original resource url, it's
                // copy url> .
                QHash<QUrl, QUrl> resourceProxyMap;

                // This is class where all IdentificationSets for all created
                // proxy resources are stored
                IdentificationSetManager * identsetManager;
                // This is our storage for all IdentificationSets for all our
                // proxied resources
                QHash< QUrl, Nepomuk::Sync::IdentificationSet > resourceProxyISMap;

                // This is url of the Decision itself
                QUrl contextUrl;

                // System use Nepomuk::Resource to create instance of the Decision.
                // Nepomuk::Resource( contextUrl, NDCO::Decision(), some manager )
                // Here this resource is stored. Currently it is not used after creation
                // , but may be it will be used later
                Nepomuk::Resource decisionRes;

                // The human-readable description of the Decision
                QString description;


                // Current group. Invalid if there is no current group
                PropertiesGroup m_currentGroup;

                /*! \brief Return true if DecisionData is freezed
                 */
                bool isFreezed() const;

                friend class Decision;
                friend class PropertiesGroup;

                ~DecisionData();
            private:
                PropertiesGroup setCurrentGroup(const PropertiesGroup &);
                PropertiesGroup resetCurrentGroup();
                PropertiesGroup currentGroup() const;
                // Create and register new properties group context
                //QUrl createPropertiesGroupUrl();
                void registerGroup(PropertiesGroup group);
                // Freeze flag. After decision is freezed, it can not be modificated
                // untill unfreezed.
                bool m_freeze;
                void setFreeze(bool val);
                bool isValid() const;
                /*! \brief Create new proxy url for the resource
                 *  This call will create new proxy url for the given resource
                 *  and update all log filter models
                 */
                QUrl proxyUrl(const Nepomuk::Resource &);

                // This function will add given url as target to the
                // decision main log model and to the log model of all
                // currently registered groups
                void updateModels(const QUrl & target);
        };
    }
}
#endif
