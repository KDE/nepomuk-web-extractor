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

#include "algorithm.h"
#include "modelgraphvisitor.h"
#include <Soprano/Node>
#include <Nepomuk/ResourceManager>



QUrl Nepomuk::deep_resource_copy(const Nepomuk::Resource & from,  Nepomuk::ResourceManager * to , bool sameModels)
{
    // Fix parameters
    if(!to) {
        to = ResourceManager::instance();
    }

    // Make a copy
    QList<QUrl> targets;
    QHash<QUrl, QUrl> convtable;
    targets << from.resourceUri();
    typedef void * BaseType; /* Base class, because we don't need it */
    typedef Soprano::Node NodeType; /* Type of the Node. It can be ignored too */
    typedef DummyNodeFunc<void*> NodeFunc; /* For given Node return itself as Node descriptor */
    typedef ConvertWriteVisitor<void*, Soprano::Node> ParseFunc; /* Conversation func. This function will perform actual copying */
    typedef Nepomuk::Graph::SelectedResourcePropertiesFunc<void*, Soprano::Node> NextFunc;/* This function will return as childs all nodes that are resource and there is triple ( subject, ?p, child )*/
    Nepomuk::Graph::visit_model_graph <
    BaseType,
    NodeType,
    NodeFunc,
    ParseFunc,
    NextFunc
    > (
        0,
        from.manager()->mainModel(),
        targets,
        -1,
        NodeFunc(),
        ParseFunc(to, &convtable),
        NextFunc(),
        sameModels
    );

    return convtable[from.resourceUri()];

}

QHash<QUrl, QUrl> *  Nepomuk::deep_resource_copy_adjust(
    const Nepomuk::Resource & from,
    Nepomuk::ResourceManager * to ,
    QHash<QUrl, QUrl> * convtable,
    bool sameModels
)
{
    if(!convtable)
        convtable = new QHash<QUrl, QUrl>();

    QSet<QUrl> visited = convtable->keys().toSet();

    // Fix parameters
    if(!to) {
        to = ResourceManager::instance();
    }

    // Make a copy
    QList<QUrl> targets;
    targets << from.resourceUri();


    typedef void * BaseType; /* Base class, because we don't need it */
    typedef Soprano::Node NodeType; /* Type of the Node. It can be ignored too */
    typedef DummyNodeFunc<void*> NodeFunc; /* For given Node return itself as Node descriptor */
    typedef ConvertWriteVisitor <BaseType, NodeType>
    Visitor; /* Conversation func. This function will perform actual copying */
    typedef IgnoreVisitedFunc <
    BaseType,
    NodeType,
    Nepomuk::Graph::SelectedResourcePropertiesFunc<void*, Soprano::Node>
    > NextFunc;/* This function will return as childs all nodes that are resource and there is triple ( subject, ?p, child )*/
    Nepomuk::Graph::visit_model_graph <
    BaseType,
    NodeType,
    NodeFunc,
    Visitor,
    NextFunc
    > (
        0,
        from.manager()->mainModel(),
        targets,
        -1,
        NodeFunc(),
        Visitor(to, convtable),
        NextFunc(&visited),
        sameModels
    );

    return convtable;
}



/*
QUrl Nepomuk::ngUri( const Soprano::Node & node)
{
    return node.uri();
}
*/
