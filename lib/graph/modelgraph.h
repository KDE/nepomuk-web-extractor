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


#ifndef __NEPOMUK_GRAPH_MODEL_GRAPH_H_
#define  __NEPOMUK_GRAPH_MODEL_GRAPH_H_

#include <Soprano/QueryResultIterator>
#include "nepomukgraph_export.h" 

namespace Soprano
{
    class Model;
    class Node;
}

namespace Nepomuk 
{
    namespace Graph
    {
	class ChildQueryInterface;
	class NEPOMUKGRAPH_EXPORT ModelGraph
	{
	    public:
		ModelGraph(Soprano::Model*, ChildQueryInterface *);
		~ModelGraph();
		Soprano::QueryResultIterator children(const Soprano::Node &);

	    private:
		// Only defenition, no implementatin
		ModelGraph(const ModelGraph & rhs);
		ModelGraph & operator=(const ModelGraph & rhs);

		class Private;
		Private *  d;
	};
    }
}
#endif
