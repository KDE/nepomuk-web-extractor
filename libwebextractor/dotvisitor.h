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

#ifndef __NEPOMUK_GRAPH_DOT_VISITOR_H_
#define __NEPOMUK_GRAPH_DOT_VISITOR_H_

#include "webextractor_export.h" 
#include "modelgraphvisitor.h"

class QTextStream;



namespace Nepomuk {
    namespace Graph {

	class WEBEXTRACTOR_EXPORT DotVisitor : public ModelGraphVisitor
	{

	    public:
		DotVisitor(QTextStream * stream);
		virtual ~DotVisitor();
		void start(bool);
		void finish(bool);
		void enter_vertex(const Soprano::Node & node);
		void enter_edge(const Soprano::Node &  currentNode, const Soprano::Node &  propertyNode, const Soprano::Node &  childNode);
		//  write(QTextStream &);
	    private:
		class Private;
		Private * d;
		QString vertexDescription(const Soprano::Node & node);
		QString vertexID(const Soprano::Node & node);
		//Q
		/*
		    class Private;
		    QSharedPointer<Private> d;
		    */
	};
    }
}
#endif


