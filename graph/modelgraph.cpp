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


#include "modelgraph.h"
#include "childqueryinterface.h"

namespace NG = Nepomuk::Graph;

class NG::ModelGraph::Private : public QSharedData
{
    public:
	Soprano::Model * model;
	ChildQueryInterface * childrenInterface;
};

NG::ModelGraph::ModelGraph( Soprano::Model * model, ChildQueryInterface * interface ):
    d( new Private() )
{
    Q_ASSERT(model);
    Q_ASSERT(interface);
    d->model = model;
    d->childrenInterface = interface;
}

NG::ModelGraph::~ModelGraph()
{
    delete d;
}

#if 0
NG::ModelGraph::ModelGraph(const ModelGraph & rhs):
    d( rhs.d)
{;}


NG::ModelGraph & NG::ModelGraph::operator=(const ModelGraph & rhs)
{
    d = rhs.d;
    return *this;
}
#endif

Soprano::QueryResultIterator NG::ModelGraph::children(const Soprano::Node & node)
{
    return d->childrenInterface->children(d->model,node);
}
