/*
   Copyright (C) 2010 by Serebriyskiy Artem <v.for.vandal at gmail.com>
    Copyright (C) 2010  Vishesh Handa <handa.vish@gmail.com>

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

#ifndef __NEPOMUK_MODEL_GRAPH_VISITOR__
#define  __NEPOMUK_MODEL_GRAPH_VISITOR__
#include <QUrl>
#include <Soprano/Node>
#include "webextractor_export.h"

namespace Soprano
{
    class Model;
    class LiteralValue;
}

namespace Nepomuk
{
    namespace Graph
    {

        /*! \brief Visitor concept implementation
         * This is reference visitor concept implementation. All it functions
         * do nothing
         */
        class WEBEXTRACTOR_EXPORT ModelGraphVisitor
        {
            public:
                /*! \brief This function is called when algorithm start
                 * If algorithm doesn't starts - because of error, becase graph is empty and so on, then \p valid will be set ot false.
                 */
                virtual void start(bool valid) {
                    Q_UNUSED(valid);
                }
                /*! \brief This function is called after algorithm finished.
                 * \param valid If set to false, then algorithm have not even started. \p valid is set to false if and only
                 * if the start() was called with false as \p valid
                 *
                 */
                virtual  void finish(bool valid) {
                    Q_UNUSED(valid);
                }

                /*! \brief This function is called on every function that is going to be parsed right now.
                 * Method is called  after the vertex is taken from the queue,but before it's childs are determined
                 * If you need to modificate Node before it's childs are determined, use this function.
                 * The algorithm guarantee that this function will be called only once for each Node.
                         * The default implementation will call one if the folowing methods: enter_resource(), enter_blank(), enter_literal().
                         * You may want to overload this methods instead of overloading enter_vertex()
                 */
                virtual void enter_vertex(const Soprano::Node & node) {
                    if(node.isResource())
                        enter_resource(node);
                    else if(node.isBlank())
                        enter_blank(node);
                    else if(node.isLiteral())
                        enter_literal(node.literal());
                }

                /*! \brief This method is called by default implementation of the enter_vertex
                 * It is called if node is of type Resource
                 */
                virtual void enter_resource(const Soprano::Node & node) {
                    Q_UNUSED(node);
                }

                virtual void enter_blank(const Soprano::Node & node) {
                    Q_UNUSED(node);
                }

                virtual void enter_literal(const Soprano::LiteralValue & value) {
                    Q_UNUSED(value);
                }


                /*! \brief This method is called after vertext was parsed
                 * Method is called after vertext was entered ( of course ) and after all edges was visited ( with enter_edge() )
                 * It is guranteed that there the only calls between enter_vertex and appropriate leave_vertext will be
                 * enter_edge() or meet_vertex()
                 * The default implementation will call one if the folowing methods: leave_resource(), leave_blank(), leave_literal().
                 * You may want to overload this methods instead of overloading leave_vertex()
                 */
                virtual void leave_vertex(const Soprano::Node & node) {
                    if(node.isResource())
                        leave_resource(node);
                    else if(node.isBlank())
                        leave_blank(node);
                    else if(node.isLiteral())
                        leave_literal(node.literal());
                }

                virtual void leave_resource(const Soprano::Node & node) {
                    Q_UNUSED(node);
                }

                virtual void leave_blank(const Soprano::Node & node) {
                    Q_UNUSED(node);
                }

                virtual void leave_literal(const Soprano::LiteralValue & value) {
                    Q_UNUSED(value);
                }


                /*! \brief This function is called on every time the Node is discovered after it was parsed - either in queue or in childs of some other Node.
                 * This function can be called multiple times during algorithm work. You can use this function, for example, to count number
                 * of occurence of the Node in the graph
                         * This function <b> will not </b> be called for vertex that is met first time ( with enter_vertex() )
                 */
                virtual void meet_vertex(const Soprano::Node & node) {
                    Q_UNUSED(node);
                }

                /*! \brief This function is called on every edge.
                 * \p node is parent node, \p currentNode is Soprano::Node, corresponding to \p node. It is  provided only for convinence.
                 * \p propertyNode is property, that connects \p node and \p childNode. \p childNode is the childNode of the \p node.
                 * It is guaranteed that \p currentNode was already parsed ( with enter_vertex ) and that \p node is the node, that
                 * corresponds to the \p currentNode.
                 * The only guarantees about \p childNode are that it will be parsed sooner or later
                 */
                virtual void enter_edge(const Soprano::Node & currentNode, const Soprano::Node & propertyNode, const Soprano::Node & childNode) {
                    Q_UNUSED(currentNode);
                    Q_UNUSED(propertyNode);
                    Q_UNUSED(childNode);
                }

                virtual ~ModelGraphVisitor() {
                    ;
                }
        };
    }
}

#endif
