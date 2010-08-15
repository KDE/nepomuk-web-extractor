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
#include <Soprano/Node>
#include <Nepomuk/ResourceManager>
#include <Nepomuk/Resource>
#include "modelgraphvisitor.h"
#include "copyvisitor.h"
#include "plaintextvisitor.h"
#include "visitednodefilter.h"
#include "graphalgorithm.h"
#include "selectedpropertiesfunc.h"
#include "plaintextvisitor.h"
#include "modelgraph.h"

namespace NG = Nepomuk::Graph;

QUrl Nepomuk::deep_resource_copy(const Nepomuk::Resource & from,  Nepomuk::ResourceManager * to, bool sameModels)
{
    // Fix parameters
    if(!to) {
        to = ResourceManager::instance();
    }

    // Make a copy
    QSet<QUrl> targets;
    QHash<QUrl, QUrl> convtable;
    targets << from.resourceUri();
    NG::CopyVisitor * visitor = new NG::CopyVisitor(to, &convtable);
    NG::SelectedPropertiesFunc * childrenFunc = new NG::SelectedPropertiesFunc();
    NG::ModelGraph * mg = new NG::ModelGraph(from.manager()->mainModel(), childrenFunc);

    Nepomuk::Graph::visit_model_graph
    (
        mg,
        targets,
        visitor
        - 1,
        sameModels
    );

    delete visitor;
    delete childrenFunc;
    delete mg;

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
    QSet<QUrl> targets;
    targets << from.resourceUri();

    NG::CopyVisitor * visitor = new NG::CopyVisitor(to, convtable);
    NG::VisitedNodeFilter * childrenFunc = new NG::VisitedNodeFilter(
        visited,
        new NG::SelectedPropertiesFunc()
    );
    NG::ModelGraph * mg = new NG::ModelGraph(from.manager()->mainModel(), childrenFunc);

    Nepomuk::Graph::visit_model_graph
    (
        mg,
        targets,
        visitor,
        -1,
        sameModels
    );

    delete visitor;
    delete childrenFunc;
    delete mg;

    return convtable;
}


void Nepomuk::dump_resource_as_text(const Nepomuk::Resource & resource, QTextStream & stream, int depth_limit)
{
    dump_resource_as_text(
        resource.resourceUri(),
        resource.manager()->mainModel(),
        stream,
        depth_limit);
}

void Nepomuk::dump_resource_as_text(const QUrl & resource, Soprano::Model * model, QTextStream & stream, int depth_limit)
{
    // Make a copy
    QSet<QUrl> targets;
    targets << resource;

    dump_resources_as_text(targets, model, stream, depth_limit);

}

void Nepomuk::dump_resources_as_text(const QSet<QUrl> & resources, Soprano::Model * model, QTextStream & stream, int depth_limit)
{
    NG::PlainTextVisitor * visitor = new NG::PlainTextVisitor(&stream);
    NG::SelectedPropertiesFunc * childrenFunc = new NG::SelectedPropertiesFunc();

    NG::ModelGraph * mg = new NG::ModelGraph(model, childrenFunc);

    Nepomuk::Graph::visit_model_graph
    (
        mg,
        resources,
        visitor,
        depth_limit,
        false
    );

    delete visitor;
    delete childrenFunc;
    delete mg;
}
/*
QUrl Nepomuk::ngUri( const Soprano::Node & node)
{
    return node.uri();
}
*/
