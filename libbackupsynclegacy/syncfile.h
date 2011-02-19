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

#ifndef SYNCFILE_H
#define SYNCFILE_H

#include <QtCore/QUrl>

#include "nepomuksync_export.h"
#include <Nepomuk/ResourceManager>

namespace Soprano {
    class Statement;
    class Model;
}

namespace Nepomuk {
    namespace Sync {
        
        class ChangeLog;
        class IdentificationSet;

        /**
        * \class SyncFile syncfile.h
        *
        * A SyncFile represents a combination of a ChangeLog and an IdentificationSet. It
        * can be saved and loaded from a gzip file.
        * When creating a SyncFile the ChangeLog must be provided. The IdentificationSet will
        * be generated from the given model.
        *
        * \author Vishesh Handa <handa.vish@gmail.com>
        */
        class NEPOMUKSYNC_EXPORT SyncFile
        {
        public:
            SyncFile();
            SyncFile( const SyncFile & rhs );
            virtual ~SyncFile();

            SyncFile & operator=( const SyncFile & rhs );

            /**
            * Create an instance of a SyncFile by reading the file present in \p url.
            * The file should be a local file
            *
            * \sa load
            */
            SyncFile( const QUrl & url );

            /**
            * Creates a SyncFile from a \p stList. The \p stList is internally converted into
            * ChangeLog with the dateTime set to the currentDateTime, and all statements marked
            * as add.
            *
            * \param model The model which should be queried to get the identifying Properties
            */
            SyncFile( const QList<Soprano::Statement>& stList, Soprano::Model * model );

            /**
            * Create a SyncFile from a LogFile.
            *
            * \param model The model which should be queried to get the identifying Properties
            */
            SyncFile( const Nepomuk::Sync::ChangeLog& log, Soprano::Model* model = ResourceManager::instance()->mainModel());

            /**
            * Convenience function to create a SyncFile froma a ChangeLog and IdentificationSet
            */
            SyncFile( const Nepomuk::Sync::ChangeLog& log, const Nepomuk::Sync::IdentificationSet& ident );

            /**
            * Loads the ChangeLog present at \p changeLogUrl and the IdentificationSet present at
            * \p identFileUrl.
            * All the urls provided should be local.
            *
            * \sa save
            */
            bool load( const QUrl& changeLogUrl, const QUrl& identFileUrl );

            /**
            * Load a SyncFile from the url \p syncFileUrl.
            * A SyncFile is gzip of a LogFile and an IdentificationFile
            *
            * \sa save
            */
            bool load( const QUrl & syncFileUrl );

            /**
            * Saves the SyncFile as a gzip of the LogFile and the IdentificationFile
            * at the url \p url
            *
            * \sa load
            */
            bool save( const QUrl & url );

            ChangeLog& changeLog();
            IdentificationSet & identificationSet();

            const ChangeLog& changeLog() const;
            const IdentificationSet & identificationSet() const;
            
        private:
            class Private;
            Private * d;
        };
    }
}

#endif // SYNCFILE_H
