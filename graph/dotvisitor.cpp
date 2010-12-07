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


#include "dotvisitor.h"
#include <Soprano/Node>
#include <QTextStream>

namespace NG = Nepomuk::Graph;

class NG::DotVisitor::Private
{
    public:
	QTextStream * stream;
	int vnum;
	int edgenum;
	QString vertexID(const Soprano::Node &);
	QString vertexDescription(const Soprano::Node &);
	QHash< Soprano::Node, int > vertices;
};

NG::DotVisitor::DotVisitor(QTextStream * stream):
    d( new Private() )
{
    this->d->stream = stream;
    d->vnum = 0;
    d->edgenum = 0;
}

 NG::DotVisitor::~DotVisitor()
{
    delete d;
}

QString NG::DotVisitor::vertexDescription(const Soprano::Node & node)
{
    if(node.isBlank()) {
        static QString blank("blank");
        return blank;
    } else if(node.isLiteral())
        return node.toString().replace('\"', "'").replace('\n',"\\n");
    else { // Node is resource
        return node.uri().toString();
    }
}
QString NG::DotVisitor::vertexID(const Soprano::Node & node)
{
    if(node.isBlank()) {
        static QString blankTemplate("blank%1");
        d->vnum++;
        return blankTemplate.arg(QString::number(d->vnum));
    } else {
        QHash< Soprano::Node , int >::const_iterator fit =
            d->vertices.find(node);
        if(fit == d->vertices.end()) {
            // Write new node
            d->vnum++;
            d->vertices.insert(node, d->vnum);
            return QString::number(d->vnum);
        } else {
            return QString::number(fit.value());
        }
    }

}

void NG::DotVisitor::enter_vertex(const Soprano::Node &  node)
{
    static QString vertexTemplate = QString("v%1 [label=\"%2\"]\n");
    *d->stream <<  vertexTemplate.arg(vertexID(node), vertexDescription(node));
}

void NG::DotVisitor::enter_edge(const Soprano::Node &  currentNode, const Soprano::Node &  propertyNode, const Soprano::Node &  childNode)
{
    static QString edgeTemplate = QString(" v%1 -> v%2 [label=\"%3\"]\n");
    *d->stream << edgeTemplate.arg(
                  vertexID(currentNode),
                  vertexID(childNode),
                  propertyNode.uri().fragment()
              );
}

void NG::DotVisitor::start(bool val)
{
    if(val)
        *d->stream << "digraph {\n";
}

void NG::DotVisitor::finish(bool val)
{
    if(val)
        *d->stream << "}\n";
}

