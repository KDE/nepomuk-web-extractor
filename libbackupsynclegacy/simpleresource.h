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


#ifndef RESOURCESTRUCT_H
#define RESOURCESTRUCT_H

#include <QtCore/QList>
#include <QtCore/QUrl>
#include <QtCore/QHash>
#include <QtCore/QSharedDataPointer>

#include "nepomuksync_export.h"

namespace Soprano {
    class Node;
    class Statement;
}

namespace Nepomuk {
    namespace Sync {

        /**
        * It represents a resource will all its properties.
        * uri -> Resource uri
        * 
        */
        class NEPOMUKSYNC_EXPORT SimpleResource : public QMultiHash<QUrl, Soprano::Node>
        {
        public :
            SimpleResource();
            SimpleResource( const SimpleResource & rhs );
            virtual ~SimpleResource();

            QList<Soprano::Statement> toStatementList() const;

            bool isFileDataObject() const;
            bool isFolder() const;
            QUrl nieUrl() const;

            QUrl uri() const;
            void setUri( const QUrl & newUri );

            SimpleResource& operator=( const SimpleResource & rhs );
            
            QList<Soprano::Node> property( const QUrl & url ) const;
            
        private:
            class Private;
            QSharedDataPointer<Private> d;
        };

        typedef QHash<QUrl, SimpleResource> ResourceHash;
    }
}
#endif // RESOURCESTRUCT_H
