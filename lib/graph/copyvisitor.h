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

#ifndef __NEPOMUK_COPY_VISITOR_H_
#define __NEPOMUK_COPY_VISITOR_H_

#include <Soprano/Node> // for qHash(QUrl)
#include "modelgraphvisitor.h"
#include "nepomukgraph_export.h" 


namespace Nepomuk {
    
    class ResourceManager;

    namespace Graph {
	/*! \brief This class will write all recived statements to given model
	 * Also it will convert any subject or object to new object.
	 * So with this function user can perform a deep copy of the resource
	 * T and Node types are ignored
	 * This class should be used as ParseFunc
	 */
	class NEPOMUKGRAPH_EXPORT CopyVisitor: public Graph::ModelGraphVisitor
	{
	    private:
		Soprano::Node vertexProxy();
		class Private;
		Private * d;
	    public:
		CopyVisitor(Nepomuk::ResourceManager * targetManager, QHash<QUrl, QUrl> * answer );
		virtual ~CopyVisitor();


		void enter_vertex(const Soprano::Node  &node); 
		void enter_edge(
			const Soprano::Node & currentNode,
			const Soprano::Node & propertyNode,
			const Soprano::Node & childNode) ;

		Soprano::Node vertexProxy(const Soprano::Node & node); 


	};
    }
}

#endif

