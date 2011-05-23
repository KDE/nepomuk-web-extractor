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

#ifndef MERGEREQUEST_H
#define MERGEREQUEST_H

#include <QtCore/QPair>
#include <QtCore/QUrl>
#include <QtCore/QMultiHash>
#include <QtCore/QDateTime>
#include <QtCore/QQueue>

#include <Nepomuk/ResourceManager>

#include "nepomuksync_export.h"

namespace Soprano {
    class Statement;
    class Model;
}

namespace Nepomuk {
    namespace Sync {

        class ChangeLog;

        /**
         * \class MergeRequest mergerequest.h
         *
         * This class performing the actual merging of the Records with the contents of the
         * model provided in the constructor. The records in the ChangeLog provided should have
         * already been identified.
         *
         * The class provides merging and not syncing. The Records are not merged on the basis
         * of timestamps. They are simply added to the model. If you require syncing please bug
         * me and I'll make the SyncRequest public.
         *
         * \author Vishesh Handa <handa.vish@gmail.com>
         */
        class NEPOMUKSYNC_EXPORT MergeRequest : public QObject
        {
            Q_OBJECT
        public :
            /**
             * Constructor -
             * \param log An Identified ChangeLog.
             * \param model The model the changes should be performed on.
             */
            MergeRequest( const Nepomuk::Sync::ChangeLog& log, Soprano::Model* model = ResourceManager::instance()->mainModel() );
            MergeRequest();
            virtual ~MergeRequest();

            /**
            * Perform the actual merge.
            */
            // trueg: should there be any feedback?
            void merge();

        private:
            class Private;
            Private *d;

        };
    }
}

//Q_DECLARE_METATYPE( Nepomuk::MergeRequest );


#endif // MERGEREQUEST_H
