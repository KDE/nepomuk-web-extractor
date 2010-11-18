/*
    This file is part of the Nepomuk KDE project.
    Copyright (C) 2010  Vishesh Handa <handa.vish@gmail.com>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/

#ifndef IDENTIFICATIONREQUEST_P_H
#define IDENTIFICATIONREQUEST_P_H

#include <QtCore/QObject>
#include <QtCore/QUrl>
#include <QtCore/QMultiHash>
#include <QtCore/QSet>

#include <KUrl>

#include "syncfile.h"
#include "identificationset.h"
#include "changelog.h"
#include "simpleresource.h"
#include <Nepomuk/ResourceManager>


namespace Soprano {
    class Node;
    class Statement;
    class Model;
}

namespace Nepomuk {
    namespace Sync {

        class ChangeLog;
        class IdentificationSet;
        class IdentificationRequest;
        class SyncFile;

        class IdentificationRequest::Private
        {
        public:
            /**
             * Contstructor.
             * It will initialize all pointers with NULL and all values that
             * has a incorrect value with this value. For example -1 will be
             * assigned for m_id.
             */
            Private( IdentificationRequest * parent );

            IdentificationRequest * q;
            
            Soprano::Model * m_model;
            ResourceManager * m_resourceManger;

            /**
            * The main identification hash which maps external ResourceUris
            * with the internal ones
            */
            QHash<QUrl, QUrl> m_hash;

            QSet<QUrl> m_notIdentified;

            /**
            * Used to store all the Resources in the identification file
            */
            ResourceHash m_resourceHash;

            IdentificationSet m_identificationSet;
            ChangeLog m_masterChangeLog;
            
            int m_id;
            static int NextId;

            /// Used in changeLog Identification
            ResourceHash m_changeLogHash;

            /**
            * This contains all the urls that are being identified, at any moment.
            * It is used to avoid infinite recursion while generating the sparql
            * query.
            * \sa constructIdentificationQuery queryIdentify;
            */
            QSet<QUrl> m_beingIdentified;

            float m_minScore;

            /**
            * Loading the advanced Identification hash is costly as the entire logfile
            * has to be parsed.
            */
            void loadChangeLogHash();

            //
            // Identification Procedures
            //

            /**
            * Executes a simple sparql query containing all the identifying properties
            */
            bool simpleIdentify( const QUrl & uri );

            /**
            * This addresses the case when the file doesn't exist in the database, but
            * it exists in the system. The function checks if the nie:url exists, and
            * assigns a new nepomuk uri to the resource.
            */
            bool existsIdentify( const QUrl& oldUri );

            /**
            * Used to identify resources which must be added in the internal database.
            * Example - Tags, Contacts
            * It checks if the uri is not a FileDataObject and doesn't contains a nie:url
            * and then simply adds the resource
            */
            bool addIdentify( const QUrl& oldUri );

            /**
            * A very imprecise identification process which probably doesn't work that well
            * It uses the the previous values of identifyingProperty from the ChangeLog
            * and uses them to find the resource.
            */
            bool changeLogIdentify( const QUrl& oldUri );

            /**
            * Calls all the identification procedures one after another
            * Returns false if none can find a match
            *
            * \sa simpleIdentify existsIdentify addIdentify advancedIdentify
            */
            bool identify( const QUrl & uri );

            /**
            * Creates a sparql query for the ResourceStruct.
            * This function may call identify in order to determine the uri of
            * the object. (If it is a nepomuk uri)
            */
            QString constructIdentificationQuery( const Sync::SimpleResource & res );

            /**
            * Checks if the @p oldUri is already in the process of being identified.
            * The function returns false if it is being identified otherwise it
            * returns the value of identify( const QUrl & )
            *
            * \sa identify
            */
            bool queryIdentify( const QUrl & oldUri );

            /**
            * Finds the best possible match for \p rs from the internal model. It uses
            * constructIdentificationQuery to create the query
            *
            * \param minScore dictates the min number of properties that should match
            *
            * \sa constructIdentificationQuery
            */
            QUrl findMatch( const Nepomuk::Sync::SimpleResource& simpleRes );

            /**
            * Creates a new resource in the internal mode which contains the properties
            * and objects present in propHash
            *
            * \return The new resource's uri
            */
            QUrl createNewResource( const Nepomuk::Sync::SimpleResource& res );

            /**
            * Converts \p allStatements into a ResourceHash.
            * This is NOT a generic conversion function. It internally corrects the $HOME location
            * of any object which is of type <file://home/xyz ...>.
            * '/home/xyz' is replaced by $HOME
            */
            static ResourceHash convertToResourceHash( const QList< Soprano::Statement >& allStatements );

            void outputDebugInfo( const QUrl & uri ) const;
        };
    }
}


#endif // IDENTIFICATIONREQUEST_P_H
