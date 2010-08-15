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


#ifndef __NEPOMUK_GRAPH_RESOURCE_NODE_FILTER_H_
#define  __NEPOMUK_GRAPH_RESOURCE_NODE_FILTER_H_


#include "nodefilterinterface.h"
#include "webextractor_export.h" 


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
	class WEBEXTRACTOR_EXPORT ResourceNodeFilter : public NodeFilterInterface
	{
	    Q_OBJECT;
	    public:
		ResourceNodeFilter( ChildQueryInterface * subQuery, bool own = true, QObject * parent = 0 );  
		
		virtual ~ResourceNodeFilter();
	    protected:
		virtual bool isOk( Soprano::Model * model, const Soprano::Node & ) ;
	    private:
		class Private;
		Private * d;
	    
	};
    }
}
#endif


