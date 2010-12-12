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


#include "vtkgraphvisitor.h"
#include <vtkMutableDirectedGraph.h>
#include <vtkStringArray.h>
#include <vtkCharArray.h>
#include <vtkDataSetAttributes.h>

#include <Soprano/Node>
#include <Soprano/Vocabulary/RDFS>
#include <Soprano/Vocabulary/NAO>

#include <QtDebug>

using namespace Nepomuk::Graph;

class VtkGraphVisitor::Private
{
    public:
        Private();
        vtkMutableDirectedGraph * graph;
        QHash< Soprano::Node, vtkIdType > vertices;
        vtkStringArray * edgeLabels;
        vtkStringArray * vertexLabels;
        vtkCharArray * vertexUserVisibility;
        const char * edgeLabelArrayName;
        const char * vertexLabelArrayName;
        bool valid;
        VtkGraphVisitor::LiteralBehaviour lbehaviour;
        bool vertexLabelCreation;
        bool edgeLabelCreation;
        
};

VtkGraphVisitor::Private::Private():
    graph(0),
    edgeLabels(0),
    vertexLabels(0),
    edgeLabelArrayName(0),
    vertexLabelArrayName(0),
    valid(false),
    lbehaviour(VtkGraphVisitor::VertexProperty),
    vertexLabelCreation(true),
    edgeLabelCreation(true)
{
}


VtkGraphVisitor::VtkGraphVisitor(const char * edgeLabelArrayName, const char * vertexLabelArrayName):
    d(new Private())
{
    if (!edgeLabelArrayName) {
        edgeLabelArrayName = "label";
    }

    if (!vertexLabelArrayName) {
        vertexLabelArrayName = "label";
    }

    d->graph = vtkMutableDirectedGraph::New();
    d->edgeLabelArrayName = edgeLabelArrayName;
    d->vertexLabelArrayName = vertexLabelArrayName;
    d->edgeLabels = 0;
    d->vertexLabels = 0;
    d->valid = true;
    /*
    vtkAbstractArray * ga = 
        d->graph->GetEdgeData()->GetAbstractArray(edgeLabelArrayName); 
    if (!ga ) {
        // There is no array with this name
        d->edgeLabels = 0;
        // Array will be created in start method.
        // Visitor is valid
        d->valid = true;
    }
    else {
        // Check that array is string array.
        vtkStringArray * sga = vtkStringArray::SafeDownCast(ga);
        if ( sga ) {
            // Array is string array
            d->edgeLabels = sga;
            d->valid = true;
        }
        else {
            // Array is not string array. We can not work with it.
            d->valid = true;
        }
        }
        */
}

VtkGraphVisitor::~VtkGraphVisitor()
{
    delete d;
}

void VtkGraphVisitor::start(bool v)
{
    // If algorithm has started, then
    // we can create necessary arrays
    // that will hold edge labels and
    // vertex properties
    if ( v and d->valid ) {
        if (!d->edgeLabels) {
            // Create it
            vtkStringArray * sa = vtkStringArray::New();
            sa->SetName(d->edgeLabelArrayName);
            d->edgeLabels = sa;
        }
        if (!d->vertexLabels) {
            // Create it
            vtkStringArray * sa = vtkStringArray::New();
            sa->SetName(d->vertexLabelArrayName);
            d->vertexLabels = sa;
        }
        /*
        if (!d->vertexUserVisibility ) {
            d->vertexUserVisibility = vtkCharArray::New();
            d->vertexUserVisibility->SetName("VertexUserVisible");
        }
        */
    }
}
void VtkGraphVisitor::finish(bool v)
{
    if (v and d->valid ) {
        // Add to the graph
        d->graph->GetEdgeData()->AddArray(d->edgeLabels);
        // Add to the graph
        d->graph->GetVertexData()->AddArray(d->vertexLabels);
    }
}

/*
void VtkGraphVisitor::enter_resource( const Soprano::Node & node )
{
    // Create a vertex
    vtkIdType id = d->graph->addVertex();
    // Add to hash
    d->vertices.insert(node,id);
}

*/

vtkIdType VtkGraphVisitor::newVertex( const Soprano::Node & node )
{
        // Create a vertex
        QString l;
        vtkIdType id = d->graph->AddVertex();
        if (d->vertexLabelCreation ) {
            Q_ASSERT(d->vertexLabels);
            // Automaticaly create necessary label
            l = node.uri().toString();
            d->vertexLabels->InsertValue(id,l.toStdString());
        }

        // Make it user invisible
        qDebug() << "Node: " << node.uri() << "Id: " << id << " Label: " << l;

        return id;
}

vtkIdType VtkGraphVisitor::newEdge( vtkIdType v1, vtkIdType v2, const Soprano::Node & node)
{
        // Add edge
        vtkIdType edgeId = d->graph->AddEdge(v1, v2, 0 ).Id;
        if (d->edgeLabelCreation ) {
            // Create data of the edge
            d->edgeLabels->InsertValue(edgeId,node.uri().fragment().toStdString());
        }
        return edgeId;
}

vtkIdType VtkGraphVisitor::vertexID( const Soprano::Node & node )
{
    vtkIdType id;
    QHash<Soprano::Node,vtkIdType>::const_iterator fit = d->vertices.find(node);
    if ( fit == d->vertices.end() ) {
        // Create a vertex
        id = newVertex(node);
        // Add to hash
        d->vertices.insert(node,id);
    }
    else {
        id = fit.value();
    }

    // Return
    return id;
}

void VtkGraphVisitor::enter_edge( const Soprano::Node & currentNode, const Soprano::Node & propertyNode, const Soprano::Node & childNode )
{
    if (!d->valid)
        return;

    // Get id of the current node
    vtkIdType id = vertexID(currentNode);
    // If child node is resource then it is an edge.
    // If child node is property then we have to add it to 
    // the list of properties value of the vertex
    if ( childNode.isResource() ) {
        vtkIdType childId = vertexID(childNode);
        newEdge(id,childId,propertyNode);
    }
    else if ( childNode.isBlank() ){
        // What to do with blank nodes ?
    }
    else if (childNode.isLiteral() ) {
        // The behaviour depends from the d->lbehaviour;
        switch (d->lbehaviour )
        {
            case ( SingleNode ) : {
                                   vtkIdType childId = vertexID(childNode);
                                    // Add edge
                                    newEdge(id,childId,propertyNode);
                                    break;
                               }
            case ( MultipleNodes ) : {
                                   vtkIdType childId = newVertex(childNode);
                                    // Add edge
                                    newEdge(id,childId,propertyNode);
                                    break;
                               }
            case ( VertexProperty ) : {
                                          // Check whether this property is in the list
                                          // of tracked propreties
                                          break;
                                      }
            default : {
                          qDebug() << "Unknow behaviour";
                      }
        }

    
        // If label populating is enabled, then some properties is treated in special way

        if (d->vertexLabelCreation) {
            // Treat label in a separate way
            if ( propertyNode.uri() == Soprano::Vocabulary::RDFS::label() or propertyNode.uri() == Soprano::Vocabulary::NAO::prefLabel() ) {
                QString ls = childNode.literal().toString();
                d->vertexLabels->InsertValue(id,ls.toStdString());
                qDebug() << "ID: " << id << " Label: " << ls; 
            }
        }
    }
    else {
        // Some new type ? interesting...
        qDebug() << "Node is not resource, not literal, not blank. What is it ?";
    }
}

vtkMutableDirectedGraph * VtkGraphVisitor::graph() const
{
    return d->graph;
}

const char * VtkGraphVisitor::edgeLabelArrayName() const
{
    if (!d->valid)
        return 0;
    return d->edgeLabelArrayName;
}

const char * VtkGraphVisitor::vertexLabelArrayName() const
{
    if (!d->valid)
        return 0;
    return d->vertexLabelArrayName;
}

VtkGraphVisitor::LiteralBehaviour VtkGraphVisitor::literalBehaviour() const
{
    return d->lbehaviour;
}

void VtkGraphVisitor::setLiteralBehaviour( LiteralBehaviour b)
{
    if ( b > VertexProperty )
        b = VertexProperty;
    if ( b < 0 )
        b = SingleNode;
    d->lbehaviour = b;
}
//#include "vtkgraphvisitor.moc"
