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

#include "resourcenodefilter.h"
#include <Soprano/Model>
#include <Soprano/Node>
#include <Soprano/Vocabulary/RDFS>
#include <Soprano/Vocabulary/RDF>
#include <KDebug>

namespace NG = Nepomuk::Graph;
class NG::ResourceNodeFilter::Private
{
};

NG::ResourceNodeFilter::ResourceNodeFilter( ChildQueryInterface * subQuery, bool own, QObject * parent ):
    NodeFilterInterface(subQuery,own,parent),
    d( new Private() )
{
    ;
}

bool NG::ResourceNodeFilter::isOk( Soprano::Model * model, const Soprano::Node & node)
{
    // If node is blank or literal - anything, except resources, then it is not necessary 
    // to call isOk, because non-resource literals can not have children
    if ( !node.isResource() )
	return false;


    static QString askTypeTemplate = QString(
					 "ask where { %1 a ?t.  FILTER( ?t in ( %2, %3) ) }").arg(
					 "%1",
					 Soprano::Node::resourceToN3(
						 Soprano::Vocabulary::RDFS::Class()
					     ),
					 Soprano::Node::resourceToN3(
						 Soprano::Vocabulary::RDF::Property()
					 )
				     );
    QString askQuery = askTypeTemplate.arg(
			   Soprano::Node::resourceToN3(node.uri())
		       );
    //kDebug() << "Ask type query: " << askQuery;

    Soprano::QueryResultIterator it = model->executeQuery(
					  askQuery,
					  Soprano::Query::QueryLanguageSparql
				      );
    if(it.next()) {
	return !it.boolValue();
    }
    return false;
}

NG::ResourceNodeFilter::~ResourceNodeFilter()
{
    delete d;
}

