/*
   Copyright (C) 2010 by Serebriyskiy Artem <v.for.vandal at gmail.com>

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#ifndef __NEPOMUK_ALGORITHM_H_
#define __NEPOMUK_ALGORITHM_H_

#include <QUrl>
#include <Soprano/Model>
#include <Soprano/Node>
#include <QUrl>
#include <Nepomuk/Resource>
#include <Nepomuk/ResourceManager>
#include "modelgraphvisitor.h"
#include "webextractor_export.h"


namespace Nepomuk
{

    /*! Make a deep copy of the resource.
     * \param from Resource to copy
     * \param to ResourceManager that manages a model to copy to
     * \param sameModels Set this parameter to true if model of \p from and model of \p to
     * are the same. This will prevent deadlocks
     * \return Url of the new resource
     */
    QUrl  deep_resource_copy(const Nepomuk::Resource & from,  Nepomuk::ResourceManager * to = 0, bool sameModels = true);


    /*! Make a deep copy or the resource, respecting already copied parts.
     * This function is usefull when you want to copy resources and it childs to the
     * model in several steps. This function guarantees that each resource will be copied
     * only once.
     * \param convtable This is table of previously copied resources. In form
     * < original resource url, copy resource url>
     */
    QHash<QUrl, QUrl> *  deep_resource_copy_adjust(const Nepomuk::Resource & from,  Nepomuk::ResourceManager * to = 0,   QHash<QUrl, QUrl> * convtable = 0 , bool sameModels = true);

    //WEBEXTRACTOR_EXPORT QUrl  ngUri( const Soprano::Node & node);

    /*! \brief This class will write all recived statements to given model
     * Also it will convert any subject or object to new object.
     * So with this function user can perform a deep copy of the resource
     * T and Node types are ignored
     * This class should be used as ParseFunc
     */
    template < typename T, typename Node, typename NodeGetter = Graph::NodeCast<T, Node> >
    class ConvertWriteVisitor: public Graph::NullVisitor<T, Node>
    {
        private:
            Nepomuk::ResourceManager * m_targetManager;
            QHash< QUrl, QUrl> * m_proxyUrls;
            Soprano::Node vertexProxy();
            NodeGetter m_getter;
        public:
            ConvertWriteVisitor(Nepomuk::ResourceManager * targetManager, QHash<QUrl, QUrl> * answer , NodeGetter getter = NodeGetter()) {
                Q_ASSERT(targetManager);
                Q_ASSERT(answer);
                m_targetManager = targetManager;
                m_proxyUrls = answer;
                m_getter = getter;
            }

            void enter_vertex(T base, Node node) {
                // Simply create proxy
                vertexProxy(m_getter(base, node));
            }

            void enter_edge(T base, Node n, const Soprano::Node & currentNode,
                            const Soprano::Node & propertyNode,
                            const Soprano::Node & childNode) {
                Q_UNUSED(base); Q_UNUSED(n);
                Soprano::Node subjProxyNode = vertexProxy(currentNode);
                Soprano::Node objProxyNode = vertexProxy(childNode);

                // Add statement

                m_targetManager->mainModel()->addStatement(Soprano::Statement(
                            subjProxyNode,
                            propertyNode,
                            objProxyNode
                        ));
            }

            Soprano::Node vertexProxy(const Soprano::Node & node) {
                if(node.isBlank())
                    return node;

                if(node.isLiteral())
                    return node;

                Soprano::Node answer;
                QHash< QUrl, QUrl >::const_iterator it =
                    m_proxyUrls->find(node.uri());
                if(it == m_proxyUrls->end()) {
                    // Create new

                    // Create new url
                    // FIXME What label should be passed to the generateUniqueUri
                    answer = Soprano::Node(
                                 m_targetManager->generateUniqueUri("unknown")
                             );
                    Q_ASSERT(!answer.uri().isEmpty());

                    // Add to table
                    kDebug() << "Adding proxy to " << node.uri() << " : " << answer.uri();
                    m_proxyUrls->insert(node.uri(), answer.uri());
                } else {
                    answer  = Soprano::Node(it.value());
                }

                return answer;
            }
    };

    template < typename T, typename Node,  typename SubFunc = Graph::SelectedResourcePropertiesFunc<T, Node>, typename NodeGetter = Graph::NodeCast<T, Node> >
    class IgnoreVisitedFunc
    {
        private:
            QSet<QUrl > * m_visited;
            SubFunc m_subFunc;
            NodeGetter m_getter;
            typedef SubFunc sub_type;
        public:
            IgnoreVisitedFunc(QSet<QUrl> * visited, NodeGetter getter = NodeGetter(), SubFunc subFunc = SubFunc()) {
                if(!visited)
                    m_visited = new QSet<QUrl>();
                else
                    this->m_visited = visited;

                m_subFunc = subFunc;
                m_getter = getter;
            }

            QString  operator()(T  base, Node node, const Soprano::Model * model) {
                Q_UNUSED(base); Q_UNUSED(model);
                Soprano::Node targetNode = m_getter(base, node);
                if(m_visited->contains(targetNode.uri()))
                    return QString();
                else
                    return m_subFunc(base, node, model);

            }


    };

    template < typename T>
    class DummyNodeFunc
    {
        public:
            Soprano::Node operator()(T base, const Soprano::Node & subject) {
                Q_UNUSED(base);
                return Soprano::Node(subject);
            }
    };

}

#endif
