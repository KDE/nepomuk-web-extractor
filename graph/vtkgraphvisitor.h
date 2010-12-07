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

#ifndef __NEPOMUK_GRAPH_VTK_VISITOR_H_
#define __NEPOMUK_GRAPH_VTK_VISITOR_H_

#include "sopranograph_export.h" 
#include "modelgraphvisitor.h"
#include <vtkType.h>

class vtkMutableDirectedGraph;



namespace Nepomuk {
    namespace Graph {

        class SOPRANOGRAPH_EXPORT VtkGraphVisitor : public ModelGraphVisitor
        {

            public:
                VtkGraphVisitor(const char * edgeLabelArrayName = 0, const char * vertexLabelArrayName = 0);
                virtual ~VtkGraphVisitor();
                virtual void start(bool);
                virtual void finish(bool);
                //void finish(bool);
                //void enter_vertex(const Soprano::Node & node);
                virtual void enter_edge(const Soprano::Node &  currentNode, const Soprano::Node &  propertyNode, const Soprano::Node &  childNode);
                //  write(QTextStream &);
                bool isValid() const;

                vtkMutableDirectedGraph * graph() const;
                const char * edgeLabelArrayName() const;
                const char * vertexLabelArrayName() const;
            private:
                class Private;
                Private * d;
                // Prohibited
                VtkGraphVisitor( const VtkGraphVisitor &);
                void operator=(const VtkGraphVisitor &);
                void init();
                QString vertexDescription(const Soprano::Node & node);
                vtkIdType vertexID(const Soprano::Node & node);
        };
    }
}
#endif


