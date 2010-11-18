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

#ifndef _NEPOMUK_WEBEXTRCT_DESICION_PRIVATE_H_
#define _NEPOMUK_WEBEXTRCT_DESICION_PRIVATE_H_

#include <QString>
#include <QUrl>
#include <QList>
#include <QHash>
#include <QSharedPointer>
#include <nepomuk/changelog.h>
#include <nepomuk/identificationrequest.h>

#include "propertiesgroupcreator.h"
#include "decision.h"
namespace Soprano
{
    class Model;
}

namespace Nepomuk
{
    namespace Sync {
	class ChangeLogFilterModel;
    }

    class ResourceManager;
    namespace WebExtractor
    {
	class DataPP;
        class DecisionCreator;
        class IdentificationSetManager;
        class PropertiesGroup;
	/*! \brief Central class for Decision constructing
	 * This class contains functionality that is common for editing 
	 * PropertiesGroups and Decision and that is specific for editing Decision.
	 */
	class DecisionCreatorInternals
        {
            public:
                DecisionCreatorInternals();
                DecisionCreatorInternals(const DataPP * parent,  Soprano::Model * decisionsModel, IdentificationSetManager * identsetManager);
                ~DecisionCreatorInternals();
		// Do we need to store the pointers to the our PropertiesGroup ?
		// The DecisionCreatorData store all pointers to the corresponding 
		// PropertiesGroupData, so may be it is enough
                QList<  PropertiesGroupCreator > groupCreators;
                
		// Authors data. Here pointers to the author DataPP
		// are stored
		QSet<const DataPP*>  authorsData;


		// Set of the target resources and their identification set
		// This may is used  for storing pairs < original resource url, it's
		// copy url> .
		QHash<QUrl, QUrl> resourceProxyMap;


                // Manager of the filter model
                ResourceManager * manager;

                // This is filtered model. It is used by manager.
                // The log used in this model is log of current PropertiesGroup
                // or 0 if there is no current PropertiesGroup
                Sync::ChangeLogFilterModel * filterModel;

                // This is main DecisionCreators model
                Soprano::Model * decisionsModel;

                // This is class where all IdentificationSets for all created
                // proxy resources are stored
                IdentificationSetManager * identsetManager;

		// This is the pointer to the actual decision data - an
		// instance of the Decision class
		//Decision::Ptr data;

                // Current group. Invalid if there is no current group
                //PropertiesGroup m_currentGroup;

		// Decision object. Decision is a COW object so we can easily return
		// it.
		Decision m_data;




		/* ==== Constructing methods ==== */
		/*! \brief This function return a Decision object 
		 * It takes a copy of the internal Decision object, popultates
		 * it with data from existing valid PropertiesGroupCreators
		 * and return it. The original internal Decision object
		 * states untouched
		 */
		Decision data();

		/* ==== Working with groups ==== */
		PropertiesGroupCreator newGroup(QWeakPointer<DecisionCreatorInternals> weakRef);
                //PropertiesGroup setCurrentGroup(const PropertiesGroup &);
                //PropertiesGroup resetCurrentGroup();
                //PropertiesGroup currentGroup() const;
                
		/* ==== State of the creator ==== */
                bool isValid() const;

		/* ==== Editing Decision ==== */

                /*! \brief Create new proxy url for the resource
                 *  This call will create new proxy url for the given resource
                 *  and update all log filter models
                 */
                QUrl proxyUrl(const Nepomuk::Resource &);

	    private:
		void initContextUrl();

                // This function will add given url as target to the
                // decision main log model and to the log model of all
                // currently registered groups
                void updateModels(const QUrl & target);
        };
    }
}
#endif


