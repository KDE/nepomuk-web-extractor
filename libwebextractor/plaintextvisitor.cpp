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


#include "plaintextvisitor.h"
#include <Soprano/Node>
#include <QTextStream>

namespace NG = Nepomuk::Graph;

class NG::PlainTextVisitor::Private
{
    public:
        Private();
        QTextStream * stream;
        PlainTextVisitor::Mode mode;
        QString vertexDescription(const Soprano::Node &);
};

NG::PlainTextVisitor::Private::Private():
    stream(0)
{
    ;
}

NG::PlainTextVisitor::PlainTextVisitor(QTextStream * stream, Mode mode):
    d(new Private())
{
    this->d->stream = stream;
    d->mode = mode;

}

NG::PlainTextVisitor::~PlainTextVisitor()
{
    delete d;
}

QString NG::PlainTextVisitor::vertexDescription(const Soprano::Node & node)
{
    if(node.isBlank()) {
        static QString blank("blank");
        return blank;
    } else if(node.isLiteral())
        return node.toString().replace('\"', "'");
    else { // Node is resource
        return node.uri().toString();
    }
}
void NG::PlainTextVisitor::enter_vertex(const Soprano::Node &  node)
{
    if(node.isResource() and !(d->mode & WriteResource))
        return;

    if(node.isLiteral() and !(d->mode & WriteLiteral))
        return;

    if(node.isBlank() and !(d->mode & WriteBlank))
        return;

    *d->stream <<   vertexDescription(node) << '\n';
}

void NG::PlainTextVisitor::enter_edge(const Soprano::Node &  currentNode, const Soprano::Node &  propertyNode, const Soprano::Node &  childNode)
{
    *d->stream << "    " << propertyNode.uri().fragment() << " -> " <<
               vertexDescription(childNode) << '\n';
}


NG::PlainTextVisitor::Mode NG::PlainTextVisitor::mode() const
{
    return d->mode;
}

void NG::PlainTextVisitor::setMode(Mode mode)
{
    d->mode = mode;
}
