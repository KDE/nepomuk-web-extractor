
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


#ifndef __NEPOMUK_GRAPH_VISITED_NODE_FILTER_H_
#define  __NEPOMUK_GRAPH_VISITED_NODE_FILTER_H_


#include "nodefilterinterface.h"
#include "nepomukgraph_export.h" 
#include <Soprano/Node> // for qHash(QUrl)
#include <QSet>
#include <QUrl>


namespace Soprano {
    class Node;
    class Model;
}

namespace Nepomuk 
{
    namespace Graph
    {
	/*! \brief This class stops parsing all nodes that are not Resources
	 * It check the type of the node (rdfs:type ) and if it is not resource, then
	 * node is leaf node
	 */
	class NEPOMUKGRAPH_EXPORT VisitedNodeFilter : public NodeFilterInterface
	{
	    Q_OBJECT;
	    public:
		VisitedNodeFilter( const QSet<QUrl> &  visited, ChildQueryInterface * subQuery, bool own = true, QObject * parent = 0 );  
		
		virtual ~VisitedNodeFilter();
	    protected:
		virtual bool isOk( Soprano::Model * model, const Soprano::Node & ) ;
	    private:
		class Private;
		Private * d;
	    
	};
    }
}
#endif

