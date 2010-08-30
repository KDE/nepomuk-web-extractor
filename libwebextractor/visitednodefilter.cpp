
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

#include "visitednodefilter.h"

namespace NG = Nepomuk::Graph;

class NG::VisitedNodeFilter::Private
{
    public:
	QSet<QUrl > visited;
};

NG::VisitedNodeFilter::VisitedNodeFilter( const  QSet<QUrl> &  visited, ChildQueryInterface * subQuery, bool own , QObject * parent ):
    NodeFilterInterface(subQuery,own,parent),
    d(new Private() )
{
    d->visited = visited;
}

bool NG::VisitedNodeFilter::isOk( Soprano::Model * model, const Soprano::Node & node)
{
    Q_UNUSED(model)
    return !d->visited.contains(node.uri());
}

NG::VisitedNodeFilter::~VisitedNodeFilter()
{
    delete d;
}
