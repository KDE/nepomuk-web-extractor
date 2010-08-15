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


#ifndef __NEPOMUK_GRAPH_CHILD_FILTER_INTERFACE_H_
#define  __NEPOMUK_GRAPH_CHILD_FILTER_INTERFACE_H_


#include "childqueryinterface.h"


namespace Soprano {
    class Node;
    class Model;
}

namespace Nepomuk 
{
    namespace Graph
    {
	/*! \brief This class is used to filter stop parsing of  the Node
	 * If condition of filter is not satisfied then given Node is
	 * treated as leaf node.
	 * \note This class can not be used to filter which childs of the Node
	 * should be parsed
	 */
	class NodeFilterInterface : public ChildQueryInterface
	{
	    Q_OBJECT;
	    public:
		NodeFilterInterface( ChildQueryInterface * subQuery, bool own = true, QObject * parent = 0 );  
	        ChildQueryInterface * subQuery() const;
		
		virtual ~NodeFilterInterface();
		Soprano::QueryResultIterator children(Soprano::Model * model,const Soprano::Node &); 
	    protected:
		virtual bool isOk( Soprano::Model * model, const Soprano::Node & ) = 0;
	    private:
		class Private;
		Private * d;
	    
	};
    }
}
#endif

