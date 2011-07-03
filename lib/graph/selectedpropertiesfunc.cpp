/*
   Copyright (C) 2010 by Serebriyskiy Artem <v.for.vandal at gmail.com>
   Copyright (C) 2010  Vishesh Handa <handa.vish@gmail.com>

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

#include "selectedpropertiesfunc.h"
#include <QSet>
#include <QUrl>
#include <Soprano/Model>
#include <Soprano/Node>

namespace NG = Nepomuk::Graph;



class NG::SelectedPropertiesFunc::Private
{
    public:
	QSet<QUrl> properties;
	QString queryString;
};

NG::SelectedPropertiesFunc::SelectedPropertiesFunc(const QSet<QUrl> & properties, QObject * parent):
   ChildQueryInterface(parent),
   d( new Private() )
{

    QStringList urlList;
    foreach(const QUrl & url, properties) {
	urlList << Soprano::Node::resourceToN3(url);
    }

    if(urlList.isEmpty()) {
	d->queryString = QString::fromLatin1("select distinct ?s ?p ?o where { %1 ?p ?o. }");
    } else {
	d->queryString = QString::fromLatin1("select distinct ?s ?p ?o where { %1 ?p ?o. "
					    " FILTER( ?p in ( %2 ) ) . } ")
			.arg("%1",
			     urlList.join(", "));
    }

}
Soprano::QueryResultIterator NG::SelectedPropertiesFunc::children(Soprano::Model * model,const Soprano::Node & targetNode)
{
    if ( targetNode.isResource() ) {
	QString query = d->queryString.arg(
	     Soprano::Node::resourceToN3(targetNode.uri())
	 );
	return model->executeQuery(
			  query,
			  Soprano::Query::QueryLanguageSparql
		      );
    }
    else {
	return Soprano::QueryResultIterator();
    }
}

#include "selectedpropertiesfunc.moc"
