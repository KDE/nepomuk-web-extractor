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

#ifndef __NEPOMUK_GRAPH_SELECTED_PROPERTIES_FUNC_H_
#define  __NEPOMUK_GRAPH_SELECTED_PROPERTIES_FUNC_H_

#include "sopranograph_export.h" 
#include "childqueryinterface.h"
#include <QSet>
#include <QUrl>

namespace Nepomuk 
{
    namespace Graph
    {

        class SOPRANOGRAPH_EXPORT SelectedPropertiesFunc : public ChildQueryInterface
        {
            Q_OBJECT;
            public:
                enum ConfigFlag { NoConfigFlags = 0x0, AddLiteralProperties = 0x1, AddDefaultProperties = 0x2 }; 
                Q_DECLARE_FLAGS( ConfigFlags, ConfigFlag );
                SelectedPropertiesFunc( const QSet<QUrl> & properties = QSet<QUrl>(), ConfigFlags cfg = NoConfigFlags, QObject * parent = 0);
                ConfigFlags configFlags() const;
                void setConfigFlags( ConfigFlags flags );
            friend class ModelGraph;
                virtual Soprano::QueryResultIterator children(Soprano::Model * model,const Soprano::Node &); 
            private:
                void updateData( const QSet<QUrl> & properties, ConfigFlags flags );
                class Private;
                Private * d;
            
        };
    }
}


#endif
