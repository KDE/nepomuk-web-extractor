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
#include "datapp.h"

namespace Nepomuk {
    namespace WebExtractor {
	// TODO Make a normal class from this one
	// Anyway it is not exported class
	class Decision;
	class PropertiesGroup;
	class DecisionData 
	{
	    public:
		// TODO move rank to upper class because it it modificated frequently
		double rank;
		QSet<  PropertiesGroup > data;
		QSet<const DataPP*>  authorsData;
		unsigned int hash;
		QTime timeStamp;
		//QString pluginName;
		//QString pluginVersion;
		// Manager of the model
		ResourceManager * manager;
		// This is the model where PropertiesGroups of this Decision store all data. 
		// It is the  same as manager->mainModel, extracted only for speed
		Soprano::Model * model;
		// This may is used  for storing pairs < original resource url, it's
		// proxy resource url> .
		QMap<QUrl,QUrl> resourceProxyUrlMap;
		
		// This is url of the Decision itself
		QUrl contextUrl;

		// System use Nepomuk::Resource to create instance of the Decision.
		// Nepomuk::Resource( contextUrl, NDCO::Decision(), some manager )
		// Here this resource is stored. Currently it is not used after creation
		// , but may be it will be used later
		Nepomuk::Resource decisionRes;

		// The human-readable description of the Decision
		QString description;

		/*! \brief Return true if DecisionData is freezed
		 */
		bool isFreezed() const;

		friend class Decision;
		friend class PropertiesGroup;

		~DecisionData();
	    private:
		// Create and register new properties group context
		QUrl createPropertiesGroupUrl();
		// Freeze flag. After decision is freezed, it can not be modificated
		// untill unfreezed.
		bool m_freeze;
		void setFreeze(bool val);
	};
    }
}
#endif
