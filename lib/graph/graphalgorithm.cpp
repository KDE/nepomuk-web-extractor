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

#include "graphalgorithm.h"
#include "modelgraph.h"
#include "modelgraphvisitor.h"

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

namespace NG = Nepomuk::Graph;

void NG::visit_model_graph(
    ModelGraph * graph,
    const QSet<QUrl> & targets,
    ModelGraphVisitor * visitor,
    int depth_limit ,
    bool caching)
{
    QList<ModelGraphVisitor*> visitors;
    visitors << visitor;
    visit_model_graph(graph, targets, visitors, depth_limit, caching);
}

void NG::visit_model_graph(
    ModelGraph * graph,
    const QSet<QUrl> & targets,
    QList<ModelGraphVisitor *> visitors,
    int depth_limit ,
    bool caching)
{
    Q_ASSERT(graph);
    QHash<Soprano::Node, int> queue;
    QSet<Soprano::Node> done;
    // Init start position
    foreach(const QUrl & url, targets) {
        if(!url.isEmpty()) {
            // TODO Check for existence before adding to the queue
            queue.insert(Soprano::Node(url), 0);
        }
    }
    if(queue.isEmpty() || !depth_limit || visitors.isEmpty()) {
        foreach(ModelGraphVisitor * visitor, visitors) {
            visitor->start(false);
            visitor->finish(false);
        }
        return;
    }


    int currentDepth = 0;

    foreach(ModelGraphVisitor * visitor, visitors) {
        visitor->start(true);
    }
    // Take next Node
    Soprano::Node currentNode = queue.begin().key();
    queue.erase(queue.begin());
    do {
        //kDebug() << "Current node: " << currentNode;
        // Create new Node instance
        // Call enter method
        foreach(ModelGraphVisitor * visitor, visitors) {
            visitor->enter_vertex(currentNode);
        }


        // Move node to done. This is necessary in case edge one has node as subject and object
        done.insert(currentNode);

        // Take childs
        // Depth check.
        if((depth_limit != -1) and(currentDepth + 1 < depth_limit))
            continue;

        Soprano::QueryResultIterator it = graph->children(currentNode);

        if(it.isValid()) {
            // Query must provide bindings for ?o - object and ?p - property

            if(caching) {
                QList<Soprano::BindingSet> bindings = it.allBindings();
                foreach(const Soprano::BindingSet & b,  bindings) {
                    // Work with child
                    Soprano::Node childNode = b.value("o");
                    QSet<Soprano::Node >::iterator dit = done.find(childNode);
                    if(dit != done.end()) {
                        // Call enter method
                        foreach(ModelGraphVisitor * visitor, visitors) {
                            visitor->meet_vertex(*dit);
                        }
                    } else {
                        // Put node to the queue
                        QHash<Soprano::Node, int>::iterator qit =
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
                        foreach(ModelGraphVisitor * visitor, visitors) {
                            visitor->enter_edge(currentNode, propertyNode, childNode);
                        }

                    }

                }
            } else {
                while(it.next()) {
                    // Put node to the queue
                    Soprano::Node childNode = it.binding("o");
                    QSet<Soprano::Node >::const_iterator dit = done.find(childNode);
                    if(dit != done.end()) {
                        // Call enter method
                        foreach(ModelGraphVisitor * visitor, visitors) {
                            visitor->meet_vertex(*dit);
                        }
                        // Do not add to the queue, because vertex has been already parsed
                    } else {
                        // Put node to the queue
                        QHash<Soprano::Node, int>::iterator qit =
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
                    foreach(ModelGraphVisitor * visitor, visitors) {
                        visitor->enter_edge(currentNode, propertyNode, childNode);
                    }

                }
            }
        }


        // Call leave_vertex
        foreach(ModelGraphVisitor * visitor, visitors) {
            visitor->leave_vertex(currentNode);
        }





        // Take next Node
        while(!queue.isEmpty()) {
            QHash<Soprano::Node, int>::iterator bit = queue.begin();
            currentNode = bit.key();
            if(!done.contains(currentNode)) {
                currentDepth = bit.value();
                // Remove node from the queue
                queue.erase(bit);
                break;
            } else { // Call meet_vertex
                foreach(ModelGraphVisitor * visitor, visitors) {
                    visitor->meet_vertex(currentNode);
                }
                // Remove node from the queue
                queue.erase(bit);
            }
        }
    } while(!queue.isEmpty());

    foreach(ModelGraphVisitor * visitor, visitors) {
        visitor->finish(true);
    }
}
