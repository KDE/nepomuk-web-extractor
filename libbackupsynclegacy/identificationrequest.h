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

#ifndef IDENTIFICATIONREQUEST_H
#define IDENTIFICATIONREQUEST_H

#include <QtCore/QObject>
#include <QtCore/QUrl>
#include <QtCore/QMultiHash>

#include <KUrl>

#include <Nepomuk/ResourceManager>
#include "nepomuksync_export.h"

namespace Soprano {
    class Statement;
    class Model;
}

namespace Nepomuk {
    namespace Sync {
        
        class ChangeLog;
        class IdentificationSet;
        class SyncFile;

        /**
        * \class IdentificationRequest identificationrequest.h
        *
        * Identifies common resources between two models. It identifies the resources on
        * the basis of the identifying statements provided in the IdentificationSet.
        *
        * All Resource which are NOT a nfo:FileDataObject are added to the model if they
        * cannot be identified. If a nfo:FileDataObject cannot be identified then it's nie:url
        * is check. If the file exists, the resource and all it's identifying properties are added.
        *
        * \author Vishesh Handa <handa.vish@gmail.com>
        */
        class NEPOMUKSYNC_EXPORT IdentificationRequest : public QObject
        {
            Q_OBJECT

        public :
            IdentificationRequest( QObject * parent = 0 );

            /**
            * Utilizes the main model for identification
            */
            IdentificationRequest( const SyncFile & sf, QObject* parent = 0 );

            /**
             * Creates an IdentificationRequest
             *
             * \param model Used to identifying the resources in the syncfile
             */
            IdentificationRequest( const SyncFile & sf, Soprano::Model* model = ResourceManager::instance()->mainModel(), QObject* parent = 0 );

            /**
             * The IdentificationSet is implicitly generated from the \p fromModel.
             * \p toModel is used to identify the resources present in the ChangeLog \p log.
             */
            IdentificationRequest( const ChangeLog & log, Soprano::Model * fromModel,
                                   Soprano::Model * toModel, QObject * parent = 0 );

            IdentificationRequest( const ChangeLog & log, const IdentificationSet & identSet,
                                   Soprano::Model * toModel, QObject * parent = 0 );

        Q_SIGNALS:
            void identified( int id, const QUrl & oldUri, const QUrl & newUri );
            void identified( const QUrl & oldUri, const QUrl & newUri );
            void notIdentified( int id, const QList<Soprano::Statement> & st );
            void notIdentified( const QUrl & uri );

            void completed( float percent );

            /**
             * Emitted during identification if more than one resource was found with the
             * exact same score \p score. Here \p score > minScore.
             *
             * \param uri Original uri
             * \param matchedUris List of uris matched with score \p score
             */
            void duplicateMatch( const QUrl & uri, const QList<QUrl> & matchedUris, float score );
            
        public:
            /**
            * Loads the Identification File from the syncFile into resource hash
            * for easier lookup. The actual loading is only done once no matter how
            * many times this function is called.
            * The loading is not done in the constructor because it can be a lenthy process
            */
            void load();

            /**
            * Starts the whole identification process. This is a HUGE operation and it
            * will take loads of time processing.
            * Don't call this function in the main thread unless you want it to become
            * unresponsive for sometime.
            */
            void identifyAll();

            /**
            * By utilizing the identification hash it modifies the logFile to change all
            * the external resourceUris to the internal ones.
            * The new logfile will only contain statements whose uri has been identified.
            *
            * This function modifies the internal ChangeLog and if it is called multiple
            * times it will return an empty ChangeLog. It will NOT return an empty ChangeLog
            * is some more resources were identified after it's previous call.
            */
            ChangeLog convert();

            /**
            * Returns the unqiue id of the IdentificationRequest
            */
            int id() const;

            /**
            * This returns true if ALL the external ResourceUris have been identified.
            * If this is false, you should manually identify some of the resources by
            * providing the nie:url or the resourceUri.
            *
            * \sa provideNieUrl provideResourceUri
            */
            bool done() const;

            /**
            * This method is used for manual identification of a resource by providing its
            * new nie:url. It does not actually perform any identification, but rather
            * changes the nie:url of resourceUri and all other resources with similar
            * nie:urls.
            * This function should be called only after identifyAll() has been run atleast once.
            * identifyAll() should be run after calling this function to perform the actual
            * identification.
            */
            void provideNieUrl( const QUrl & resourceUri, const QUrl & nieUrl );

            /**
            * Used for manual identification when both the old Resource uri, and
            * the new resource Uri is provided.
            * It simple adds it to the identification hash
            */
            void provideResourceUri( const QUrl & oldUri, const QUrl & newUri );

            /**
            * Ignores resourceUri (It will no longer play a part in identification )
            * if @p ignoreSub is true and resourceUri is a Folder, all the sub folders
            * and files are also ignored.
            */
            bool ignore( const QUrl & resourceUri, bool ignoreSub );

            /**
            * Returns the detected uri for the given resourceUri.
            * This method usefull only after identifyAll() method was called
            */
            QUrl mappedUri( const QUrl & resourceUri ) const;

            /**
             * Returns mappings of the identified uri
             */
            QHash<QUrl,QUrl> mappings() const;

            /**
             * Returns urls that were not successfully identified
             */
            QSet<QUrl> unidentified() const;

            /**
             * Returns the min % of the number of statements that should match during identification
             * in order for a resource to be successfully identified.
             *
             * Returns a value between [0,1]
             */
            float minScore() const;

            void setMinScore( float score );

        private:
            class Private;
            Private * d;

            // This methods should be called from the constructor
            void initModel( Soprano::Model * model = 0);
            void initModel( Nepomuk::ResourceManager * manager );

        };
    }
}


#endif // IDENTIFICATIONREQUEST_H
