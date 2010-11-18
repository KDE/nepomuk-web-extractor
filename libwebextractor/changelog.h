/*
    This file is part of the Nepomuk KDE project.
    Copyright (C) 2010  Vishesh Handa <handa.vish@gmail.com>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along
    with this program; if not, write to the Free Software Foundation, Inc.,
    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

*/

#ifndef CHANGELOG_H
#define CHANGELOG_H

#include <QtCore/QUrl>
#include <QtCore/QTextStream>

namespace Soprano {
    class Model;
    class Statement;
}

namespace Nepomuk {
    namespace Sync {

        class ChangeLogRecord;

        /**
         * \class ChangeLog changelog.h
         *
         * Consists of a set of ChangeLogRecords.
         *
         * In order to use the Sync API, a list of changes or a ChangeLog has
         * to be provided. It is advisable to keep the internal records sorted on
         * the basis of their time stamp, but that IS NOT enforced.
         *
         * \sa ChangeLogRecord
         *
         * \author Vishesh Handa <handa.vish@gmail.com>
         */
        class ChangeLog
        {
        public :
            ChangeLog();

            /**
             * Converts \p st into a list of ChangeLogRecords. The dateTime is set
             * to the current time, and all statements are marked as added.
             */
            ChangeLog( const QList<Soprano::Statement> & st );
            ChangeLog( const QList<ChangeLogRecord> & records );

            // trueg: seems to me that the two constructors below would make more sense as static factory methods
            //        since they do not really construct the ChangeLog from the graphUrl and the model but rather
            //        use the two to create a ChangeLog. Dont get me wrong: this is major nitpicking but IMHO it
            //        would make for a more consistent API.
            /**
             * Load all statements from graph \p graphUrl in the \p model into a list of
             * ChangeLogRecord. The dateTime is set to the current time, and all
             * statements are marked as added.
             */
            ChangeLog( const QUrl& graphUrl, Soprano::Model* model);

            /**
             * Load all statements from all contexts in \p graphUrlList into a list of
             * ChangeLogRecord. The dateTime is set to the current time, and all
             * statements are marked as added.
             */
            ChangeLog( const QList<QUrl>& graphUrlList, Soprano::Model* model );

            /// Copy constructor
            ChangeLog( const ChangeLog & rhs );
            virtual ~ChangeLog();

            ChangeLog& operator=( const ChangeLog & rhs );

            static ChangeLog fromUrl( const QUrl & url );
            static ChangeLog fromUrl( const QUrl & url, const QDateTime & min );

            /**
            * Saves all the internal records in url. The records are saved in plain text form
            */
            bool save( const QUrl & url ) const;

            int size() const;
            bool empty() const;
            void clear();

            virtual void add( const ChangeLogRecord & record );

            void sort();

            QList<ChangeLogRecord> toList() const;

            /**
            * Changes all the added statements to removed and vice versa
            */
            void invert();

            /**
            * Removes all the records whose subject is not present in \p nepomukUris
            */
            void filter( const QSet<QUrl> & nepomukUris );

            /**
             * Concatenates the records held by the two ChangeLogs
             */
            // trueg: why not operator+ and operator+=? After all this is not streaming but concatenation.
            ChangeLog & operator<<( const ChangeLog & log );

            ChangeLog& operator <<( const ChangeLogRecord & record );

            void removeRecordsGreaterThan( const QDateTime& dt );
            void removeRecordsLessThan( const QDateTime & dt );

            /**
             * Return uri of all objects and subjects in changelog
             */
            QSet<QUrl> resources() const;
            /**
             * Return uri of all subjects in changelog
             */
            QSet<QUrl> subjects() const;

            // trueg: do we really only have resource objects here? No literals at all?
            /**
             * Return uri of all objects in changelog
             */
            QSet<QUrl> objects() const;

            static QString dateTimeFormat();

        private:
            class Private;
            Private * d;
        };

        QTextStream& operator<<( QTextStream & ts, const ChangeLog & log );
        QDebug operator<<( QDebug debug, const ChangeLog & log );
    }
}

#endif // CHANGELOG_H
