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


#ifndef __NEPOMUK_GRAPH_CHILD_QUERY_INTERFACE_H_
#define  __NEPOMUK_GRAPH_CHILD_QUERY_INTERFACE_H_

#include <QObject>
#include <Soprano/QueryResultIterator>

namespace Soprano
{
    class Model;
    class Node;
}

namespace Nepomuk 
{
    namespace Graph
    {
	class ModelGraph;

	class ChildQueryInterface : public QObject
	{
	    Q_OBJECT;
	    public:
	        ChildQueryInterface( QObject * parent = 0):
		    QObject(parent) {;}
		friend class ModelGraph;
		virtual Soprano::QueryResultIterator children(Soprano::Model * model,const Soprano::Node &) = 0; 
	    
	};
    }
}
#endif

