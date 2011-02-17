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

#include "copyvisitor.h"
#include <Nepomuk/ResourceManager>
#include <Soprano/Model>
#include <Soprano/Statement>
#include <Soprano/Node>
#include <QHash>
#include <QUrl>


namespace NG = Nepomuk::Graph;

class NG::CopyVisitor::Private
{
    public:
	Nepomuk::ResourceManager * targetManager;
	QHash< QUrl, QUrl> * proxyUrls;
};

NG::CopyVisitor::CopyVisitor(Nepomuk::ResourceManager * targetManager, QHash<QUrl, QUrl> * answer ):
   d(new Private() ) 
{
    Q_ASSERT(targetManager);
    Q_ASSERT(answer);
    d->targetManager = targetManager;
    d->proxyUrls = answer;
}

NG::CopyVisitor::~CopyVisitor()
{
    delete d;
}

void NG::CopyVisitor::enter_vertex(const Soprano::Node & node) 
{
    // Simply create proxy
    vertexProxy(node);
}

void NG::CopyVisitor::enter_edge(const Soprano::Node & currentNode,
		const Soprano::Node & propertyNode,
		const Soprano::Node & childNode) {
    Soprano::Node subjProxyNode = vertexProxy(currentNode);
    Soprano::Node objProxyNode = vertexProxy(childNode);

    // Add statement

    d->targetManager->mainModel()->addStatement(Soprano::Statement(
		subjProxyNode,
		propertyNode,
		objProxyNode
	    ));
}

Soprano::Node NG::CopyVisitor::vertexProxy(const Soprano::Node & node) 
{
    if(node.isBlank())
	return node;

    if(node.isLiteral())
	return node;

    Soprano::Node answer;
    QHash< QUrl, QUrl >::const_iterator it =
	d->proxyUrls->find(node.uri());
    if(it == d->proxyUrls->end()) {
	// Create new

	// Create new url
	// FIXME What label should be passed to the generateUniqueUri
	answer = Soprano::Node(
		     d->targetManager->generateUniqueUri("unknown")
		 );
	Q_ASSERT(!answer.uri().isEmpty());

	// Add to table
	//kDebug() << "Adding proxy to " << node.uri() << " : " << answer.uri();
	d->proxyUrls->insert(node.uri(), answer.uri());
    } else {
	answer  = Soprano::Node(it.value());
    }

    return answer;
}
