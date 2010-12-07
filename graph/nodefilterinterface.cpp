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

#include "nodefilterinterface.h"
#include <QtGlobal>

namespace NG = Nepomuk::Graph;

class NG::NodeFilterInterface::Private
{
    public:
	ChildQueryInterface * subQuery;
};

NG::NodeFilterInterface::NodeFilterInterface( ChildQueryInterface * subQuery, bool own, QObject * parent ):
    ChildQueryInterface(parent),
    d(new Private() )
{
    Q_ASSERT(subQuery);
    d->subQuery = subQuery;
    if (own) {
	d->subQuery->setParent(this);
    }
}

NG::ChildQueryInterface * NG::NodeFilterInterface::subQuery() const
{
    return d->subQuery;
}

NG::NodeFilterInterface::~NodeFilterInterface()
{
    delete d;
}

Soprano::QueryResultIterator NG::NodeFilterInterface::children(Soprano::Model * model,const Soprano::Node & node)
{


    // Virtual call
    if (this->isOk(model,node) )
        return d->subQuery->children(model,node);
    else
        return Soprano::QueryResultIterator();
}

//#include "nodefilterinterface.moc"
