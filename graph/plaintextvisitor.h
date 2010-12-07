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

#ifndef __NEPOMUK_GRAPH_PLAIN_TEXT_VISITOR_H_
#define __NEPOMUK_GRAPH_PLAIN_TEXT_VISITOR_H_

#include "sopranograph_export.h"
#include "modelgraphvisitor.h"

class QTextStream;



namespace Nepomuk
{
    namespace Graph
    {

        /*! \brief This class can be used to write the graph to text stream
         * The main purpose of this class is debugging of the model.
         * The behaviour is the folowing:
         * Every node ( not only resource node ) is written,
         * then each propertie of this node is written
         * Example
         * <code>
         * nepomuk:/res/xxx
         *     property1 -> value1 // value1 can be resrouce or literal or blank
         * nepomuk:/res/yyy
         *     propery3 -> value 3
         *     propery4 -> value 4
         * "some text " // This is literal node
         * 5 // Again literal node
         * blank // Blank node
         * </code>
         * If you want to avoid writing non-resource node as top-level node
         * set approprite flag
         */
        class SOPRANOGRAPH_EXPORT PlainTextVisitor : public ModelGraphVisitor
        {

            public:
                enum Mode {WriteNone = 0, WriteResource = 1, WriteLiteral = 2, WriteBlank = 4, WriteAll = 7};
                PlainTextVisitor(QTextStream * stream, Mode mode = WriteResource);
                virtual ~PlainTextVisitor();
                void enter_vertex(const Soprano::Node & node);
                void enter_edge(const Soprano::Node &  currentNode, const Soprano::Node &  propertyNode, const Soprano::Node &  childNode);
                //  write(QTextStream &);
                Mode mode() const;
                void setMode(Mode mode) ;
            private:
                class Private;
                Private * d;
                QString vertexDescription(const Soprano::Node & node);
                /*
                    class Private;
                    QSharedPointer<Private> d;
                    */
        };
    }
}
#endif


