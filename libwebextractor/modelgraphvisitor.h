/*
   Copyright (C) 2010 by Serebriyskiy Artem <v.for.vandal at gmail.com>
    Copyright (C) 2010  Vishesh Handa <handa.vish@gmail.com>

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

#ifndef __NEPOMUK_MODEL_GRAPH_VISITOR__
#define  __NEPOMUK_MODEL_GRAPH_VISITOR__
#include <Soprano/Node>
#include <Soprano/Model>
#include <Soprano/Vocabulary/RDFS>
#include <Soprano/BindingSet>
#include <Soprano/QueryResultIterator>
#include <Nepomuk/ResourceManager>
#include <QUrl>
#include <QSet>
#include <QtGlobal>
#include <QHash>
#include <KDebug>

namespace Nepomuk
{
    namespace Graph
    {

        template< typename T, typename Node >
        /*! \brief Visitor concept implementation
         * This is reference visitor concept implementation. All it functions
         * do nothing
         */
        class NullVisitor
        {
            public:
                /*! \brief This function is called when algorithm start
                 * If algorithm doesn't starts - because of error, becase graph is empty and so on, then \p valid will be set ot false.
                 */
                void start(T base, bool valid) {
                    Q_UNUSED(valid);
                    Q_UNUSED(base);
                }
                /*! \brief This function is called after algorithm finished.
                 * \param valid If set to false, then algorithm have not even started. \p valid is set to false if and only
                 * if the start() was called with false as \p valid
                 *
                 */
                void finish(T base,  bool valid) {
                    Q_UNUSED(valid);
                    Q_UNUSED(base);
                }

                /*! \brief This function is called on every function that is going to be parsed right now.
                 * Method is called  after the vertex is taken from the queue,but before it's childs are determined
                 * If you need to modificate Node before it's childs are determined, use this function.
                 * The algorithm guarantee that this function will be called only once for each Node.
                 */
                void enter_vertex(T base, Node node) {
                    Q_UNUSED(base);
                    Q_UNUSED(node);
                }

                /*! \brief This function is called on every time the Node is discovered - either in queue or in childs of some other Node.
                 * This function can be called multiple times during algorithm work. You can use this function, for example, to count number
                 * of occurence of the Node in the graph
                 */
                void meet_vertex(T base, Node node) {
                    Q_UNUSED(base);
                    Q_UNUSED(node);
                }

                /*! \brief This function is called on every edge.
                 * \p node is parent node, \p currentNode is Soprano::Node, corresponding to \p node. It is  provided only for convinence.
                 * \p propertyNode is property, that connects \p node and \p childNode. \p childNode is the childNode of the \p node.
                 * It is guaranteed that \p currentNode was already parsed ( with enter_vertex ) and that \p node is the node, that
                 * corresponds to the \p currentNode.
                 * The only guarantees about \p childNode are that it will be parsed sooner or later
                 */
                void enter_edge(T base, Node node, const Soprano::Node & currentNode, const Soprano::Node & propertyNode, const Soprano::Node & childNode) {
                    Q_UNUSED(base);
                    Q_UNUSED(node);
                    Q_UNUSED(currentNode);
                    Q_UNUSED(propertyNode);
                    Q_UNUSED(childNode);
                }
        };

        template < typename T, typename Node, typename NodeFunc, typename Visitor , typename NextFunc  >
        /*!\brief Visit graph
         * \param depth_limit Algorithm will ignore all nodes whoes depth is more or equal then \p depth_limit. Start vertices of the
         * algorithm ( \p targets ) has depth 0. If set to -1, then no limit will be used.
         */
        void visit_model_graph(
            T base,
            Soprano::Model * model,
            const QList<QUrl> & targets,
            int depth_limit = -1,
            NodeFunc nodeFunc = NodeFunc(),
            Visitor visitor = Visitor(),
            NextFunc nextFunc = NextFunc(),
            bool caching = true)
        {
            Q_ASSERT(model);
            QHash<Soprano::Node, int> queue;
            QHash<Soprano::Node, Node> done;
            // Init start position
            foreach(const QUrl & url, targets) {
                if(!url.isEmpty()) {
                    queue.insert(Soprano::Node(url), 0);
                }
            }
            if(queue.isEmpty() || !depth_limit) {
                visitor.start(base, false);
                visitor.finish(base, false);
                return;
            }


            int currentDepth = 0;

            visitor.start(base, true);
            // Take next Node
            Soprano::Node currentNode = queue.begin().key();
            queue.erase(queue.begin());
            do {
                //kDebug() << "Current node: " << currentNode;
                // Create new Node instance
                Node n = nodeFunc(
                             base,
                             const_cast<const Soprano::Node&>(currentNode)
                         );

                // Call enter method
                visitor.enter_vertex(base, n);

                // Get all node childs
                QString queryString = nextFunc(
                                          base,
                                          n,
                                          const_cast<const Soprano::Model*>(model)
                                      );

                //kDebug() << "Next childs query: " << queryString;

                // Move node to done. This is necessary in case edge one node as subject and object
                done.insert(currentNode, n);

                // Take childs
                // Depth check.
                if((depth_limit != -1) and(currentDepth + 1 < depth_limit))
                    continue;

                if(queryString.size()) {
                    // Query must provide bindings for ?o - object and ?p - property

                    Soprano::QueryResultIterator it = model->executeQuery(
                                                          queryString,
                                                          Soprano::Query::QueryLanguageSparql
                                                      );
                    if(caching) {
                        QList<Soprano::BindingSet> bindings = it.allBindings();
                        foreach(const Soprano::BindingSet & b,  bindings) {
                            // Work with child
                            Soprano::Node childNode = b.value("o");
                            typename QHash<Soprano::Node, Node >::iterator dit = done.find(childNode);
                            if(dit != done.end()) {
                                // Call enter method
                                visitor.meet_vertex(base, dit.value());
                            } else {
                                // Put node to the queue
                                typename QHash<Soprano::Node, int>::iterator qit =
                                    queue.find(childNode);
                                if(qit == queue.end()) {
                                    // Queue doesn't contain node
                                    queue.insert(childNode, currentDepth + 1);
                                } else { // Node is already in queue
                                    // Update it depth.
                                    if(qit.value() > currentDepth + 1)
                                        qit.value() = currentDepth + 1;
                                }

                                Soprano::Node propertyNode = b.value("p");

                                // Make some actions
                                visitor.enter_edge(base, n, currentNode, propertyNode, childNode);

                            }
                        }
                    } else {
                        while(it.next()) {
                            // Put node to the queue
                            Soprano::Node childNode = it.binding("o");
                            typename QHash<Soprano::Node, Node >::const_iterator dit = done.find(childNode);
                            if(dit != done.end()) {
                                // Call enter method
                                visitor.meet_vertex(base, dit.value());
                                // Do not add to the queue, because vertex has been already parsed
                            } else {
                                // Put node to the queue
                                typename QHash<Soprano::Node, int>::iterator qit =
                                    queue.find(childNode);
                                if(qit == queue.end()) {
                                    // Queue doesn't contain node
                                    queue.insert(childNode, currentDepth + 1);
                                } else { // Node is already in queue
                                    // Update it depth.
                                    if(qit.value() > currentDepth + 1)
                                        qit.value() = currentDepth + 1;
                                }
                            }
                            Soprano::Node propertyNode = it.binding("p");

                            // Make some actions
                            visitor.enter_edge(base, n, currentNode, propertyNode, childNode);

                        }
                    }
                }





                // Take next Node
                while(!queue.isEmpty()) {
                    typename QHash<Soprano::Node, int>::iterator bit = queue.begin();
                    currentNode = bit.key();
                    queue.erase(bit);
                    if(!done.contains(currentNode)) {
                        currentDepth = bit.value();
                        break;
                    }
                }
            } while(!queue.isEmpty());

            visitor.finish(base, true);
        }

        // Now some convinient classes
        //

        template< typename T, typename Node>
        class NodeCast
        {
            public:
                Soprano::Node operator()(T base, Node node) {
                    Q_UNUSED(base);
                    return Soprano::Node(node);
                }
        };

        /*! \brief This class will return query that select all given properties.
         * Only the properties, whose object is Resource will be returned
         * The global ngUri function must be defineg for Node*.
         * This class should be used as NextFunc
         */
        template < typename T, typename Node, typename NodeGetter = NodeCast<T, Node> >
        class SelectedResourcePropertiesFunc
        {
            public:
                enum IgnoreFlags { NoIgnore = 0x0, IgnoreOntology = 0x1 };
            private:
                QString m_queryString;
                NodeGetter m_getter;
                IgnoreFlags m_flags;
            public:
                SelectedResourcePropertiesFunc(const QSet<QUrl> & properties = QSet<QUrl>(), IgnoreFlags flags = NoIgnore, NodeGetter getter = NodeGetter()) {

                    QStringList urlList;
                    foreach(const QUrl & url, properties) {
                        urlList << Soprano::Node::resourceToN3(url);
                    }
                    if(urlList.isEmpty()) {
                        m_queryString = QString::fromLatin1("select distinct ?s ?p ?o where { %1 ?p ?o. }");
                    } else {
                        m_queryString = QString::fromLatin1("select distinct ?s ?p ?o where { %1 ?p ?o. "
                                                            " FILTER( ?p in ( %2 ) ) . } ")
                                        .arg("%1",
                                             urlList.join(", "));
                    }
                    m_getter = getter;
                    m_flags = flags;
                }

                QString  operator()(T  base, Node node, const Soprano::Model * model) {
                    Q_UNUSED(base); Q_UNUSED(model);
                    Soprano::Node targetNode = m_getter(base, node);

                    if(targetNode.isLiteral())
                        return QString();

                    // If ontology classes should be ignored
                    if(m_flags & IgnoreOntology) {
                        // Check if resource is part of ontology
                        // Check if resource has type Class
                        static QString askTypeTemplate = QString(
                                                             "ask where { %1 a %2 }").arg(
                                                             "%1",
                                                             Soprano::Node::resourceToN3(
                                                                     Soprano::Vocabulary::RDFS::Class()
                                                             )
                                                         );
                        QString askQuery = askTypeTemplate.arg(
                                               Soprano::Node::resourceToN3(targetNode.uri())
                                           );
                        kDebug() << "Ask type query: " << askQuery;

                        Soprano::QueryResultIterator it = model->executeQuery(
                                                              askQuery,
                                                              Soprano::Query::QueryLanguageSparql
                                                          );
                        if(it.next()) {
                            if(it.boolValue()) {
                                // Node should be ignored
                                return QString();
                            }
                        }
                    }

                    QString answer = m_queryString.arg(
                                         Soprano::Node::resourceToN3(targetNode.uri())
                                     );
                    return answer;
                }
        };

    }
}



// Backup storage for obsolete version of template

#if 0
template < typename T, typename Node, typename NodeFunc, typename ParseFunc, typename NextFunc  >
class ModelGraphVisitor
{
    public:
        ModelGraphVisitor(T * base, Soprano::Model * model, NodeFunc nodeFunc = NodeFunc(), ParseFunc parseFunc = ParseFunc(), NextFunc nextFunc = NextFunc());
        {
            Q_ASSERT(base);
            Q_ASSERT(model);
            m_base = base;
            m_model = model;
            m_nodeFunc = nodeFunc;
            m_parseFunc = parseFunc;
            m_nextFunc = nextFunc;
        }
        void addProperties(const QList< QUrl > & properties) {
            foreach(const QUrl & url, properties)
            m_properties << url;
        }

        void addTarget(const QList< QUrl> & targets);
        {
            foreach(const QUrl & url, targets)
            m_targets << url;
        }

        void setUseCaching(bool val) {
            m_caching = val;
        }

        bool useCaching() const {
            return m_caching;
        }

        void execute();

    private:
        QList<QUrl> m_targets;
        QSet<QUrl> m_properties;
        //QSet<Soprano::Node> m_queue;
        m_nodeFunc;
        m_parseFunc;
        m_nextFunc;
        T * m_base;
        Soprano::Model * m_model;
        bool m_caching;

};

template < typename T, typename Node, typename NodeFunc, typename ParseFunc, typename NextFunc  >
void ModelGraphVisitor<T, Node, NodeFunc, ParseFunc, NextFunc>::execute()
{
    QSet<Soprano::Node> queue;
    QHash<Soprano::Node, Node* > done;
    r
    // Init start position
    foreach(const QUrl & url, m_targets) {
        queue << Soprano::Node(url);
    }
    if(queue.isEmpty())
        return;

    // Take next Node
    Soprano::Node currentNode = *queue.begin();
    queue.erase(queue.begin());
    do {
        // Create new Node instance
        Node * n = m_nodeFunc(
                       m_base,
                       const_cast<const Soprano::Node&>(currentNode)
                   );

        // Get all node childs
        QString queryString = m_nextFunc(
                                  m_base,
                                  , m_properties);
        // Query must provide bindings for ?o - object and ?p - property
        Soprano::QueryResultIterator it = m_model->executeQuery(
                                              queryString,
                                              Soprano::QueryLanguageSparql
                                          );
        if(m_caching) {
            QList<BindingSet> bindings = it.allBindings();
            foreach(const BindingSet & b,  bindings) {
                // Put node to the queue
                Soprano::Node childNode = v.value("o");
                queue << childNode;
                Soprano::Node propertyNode = v.value("p");

                // Make some actions
                m_parseFunc(m_base, propertyNode, childNode);

            }
        } else {
            while(it.next()) {
                // Put node to the queue
                Soprano::Node childNode = it.binding("o");
                queue << childNode;
                Soprano::Node propertyNode = it.binding("p");

                // Make some actions
                m_parseFunc(m_base, propertyNode, childNode);

            }
        }





        // Take next Node
        while(!queue.isEmpty()) {
            currentNode = *queue.begin();
            queue.erase(queue.begin());
            if(!done.contains(currentNode))
                break;
        }
    } while(!queue.isEmpty())
    }
#endif
#endif
