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

#include <QList>
#include <QSharedPointer>
#include <QString>
#include <QHash>
#include <QTime>

//#include "identificationset.h"

#include "propertiesgroup.h"

namespace Nepomuk {
    namespace Decision {
	class DecisionData : public QSharedData
	{
	    public:
		// Rank of the Decision. Must be approximately 0 <= rank < 1.
		// The exact boundaries are storeg in globals
		double rank;
		// Every meaningfull Decision consist of one or more 
		// PropertiesGroup. The actuall changes are stored inside
		// PropertiesGroup as NS::ChangeLog object 
		QList< PropertiesGroup > groups;
		// The human-readable description of the Decision
		QString description;

		// This is our storage for all IdentificationSets for all our
		// proxied resources. The key is the PROXY url, not the source one 
		QHash< QUrl, NS::IdentificationSet > resourceProxyISMap;

		// time stamp. The time when the creation of this decision started
		QTime timeStamp;

	};

    }
}

#endif
