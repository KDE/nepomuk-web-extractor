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
    namespace Decision
    {
	class DecisionAuthor;
        class DecisionCreator;
        class PropertiesGroup;
	/*! \brief Central class for Decision constructing
	 * This class contains functionality that is common for editing 
	 * PropertiesGroups and Decision and that is specific for editing Decision.
	 */
	class DecisionCreatorInternals
        {
            public:
                DecisionCreatorInternals();
                DecisionCreatorInternals(const DecisionAuthor * parent);
                ~DecisionCreatorInternals();
		// Do we need to store the pointers to the our PropertiesGroup ?
		// The DecisionCreatorData store all pointers to the corresponding 
		// PropertiesGroupData, so may be it is enough
                QList<  PropertiesGroupCreator > groupCreators;
                
		// Authors data. Here pointers to the author Executive
		// are stored
		QSet<const DecisionAuthor*>  authorsData;



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
                
		/* ==== State of the creator ==== */
                bool isValid() const;



        };
    }
}
#endif


