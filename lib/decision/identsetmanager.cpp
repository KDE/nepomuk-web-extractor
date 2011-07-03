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

#include "identsetmanager.h"
#include <QHash>
// This is for qHash(QUrl)
#include <Soprano/Node>

#include "identificationset.h"

namespace ND = Nepomuk::Decision;
namespace NS = Nepomuk::Sync;

class ND::IdentificationSetManager::Private
{
    public:
        //QHash< QUrl, IdentificationNode > sets;
        Soprano::Model * mainModel;
};


ND::IdentificationSetManager::IdentificationSetManager(Soprano::Model * mainModel):
    d(new IdentificationSetManager::Private())
{
    Q_ASSERT(mainModel);
    d->mainModel = mainModel;
}

ND::IdentificationSetManager::~IdentificationSetManager()
{
    delete d;
}

#if 0
ND::IdentificationGraph NW::IdentificationSetManager::identificationGraph(const QUrl & resourceUrl, IdentificationGraph::UpdateMode mode)
{
    QHash<QUrl, IdentificationNode>::iterator fit =
        d->sets.find(resourceUrl);

    if((mode == IdentificationGraph::NoUpdate))
        if((fit != d->sets.end())) {
            // No update is necessary and all existing information is already
            // in database
            // Just build new graph
            // TODO Add caching for graphs
            IdentificationGraph answer(this);
            QQueue<QUrl> queue;
            queue << resourceUrl;
            while(queue.size() > 0) {
                // Take next
                QUrl next = queue.dequeue();
                while(answer.hasNode(next))
                    next = queue.dequeue();

                Q_ASSERT(d->sets.contains(next));
                IdentificationNode node = d->sets[next];
                answer.serviceAddNode(next, node);

                // Add all non-parsed nodes to queue
                foreach(const QUrl & child, node.childrens()) {
                    queue << child;
                }
            }
            return answer;
        } else {
            // Build nodes for unavailable resources
            IdentificationGraph answer(this);
            QQueue<QUrl> queue;
            queue << resourceUrl;
            while(queue.size() > 0) {
                // Take next
                QUrl next = queue.dequeue();
                while(answer.hasNode(next))
                    next = queue.dequeue();

                QHash<QUrl, IdentificationNode>::const_iterator fit =
                    d->sets.find(next);

                if(fit != d->sets.end()) {
                    // it is already in datbase
                    answer.serviceAddNode(next, fit.value());

                } else {
                    static QString queryTemplate = QString::fromLatin1("select distinct  ?p ?o where { %1 ?p ?o. "
                                                   "{ ?p %2 %3 .} "
                                                   "UNION { ?p %2 %4. } "
                                                   "  } ")
                                                   .arg(
                                                       "%1",
                                                       Soprano::Node::resourceToN3(Soprano::Vocabulary::RDFS::subPropertyOf()),
                                                       Soprano::Node::resourceToN3(Nepomuk::Vocabulary::backupsync::identifyingProperty()),
                                                       Soprano::Node::resourceToN3(Soprano::Vocabulary::RDF::type()));
                    QString query = queryTemplate.arg(
                                        Soprano::Node::resourceToN3(next)
                                    );
                    IdentificationNode newNode;
                    newNode.parent = 0;
                    newNode.nodeUrl = next;

                    // Get timestamp
                    static QString dateQueryTemplate =
                        QString("select ?d ORDER BY ASC(?d) LIMIT 1 where { %1 %2 ?d }").arg(
                            "%1",
                            Soprano::Node::resourceToN3(
                                Soprano::Vocabulray::NAO::lastModified()
                            )
                        );
                    QString dateQuery = dateQueryTemplate.arg(
                                            Soprano::Node::resourceToN3(next)
                                        );

                    Soprano::QueryResultIterator dit =
                        d->mainModel->executeQuery(
                            dateQuery,
                            Soprano::Query::QueryLanguageSparql
                        );
                    QDateTime timeStamp = QDateTime::currentTime();

                    while(dit.next()) {
                        timeStamp = dit.binding("d").literal().toDateTime();
                    }

                    Soprano::QueryResultIterator rit =
                        d->mainModel->executeQuery(
                            query,
                            Soprano::Query::QueryLanguageSparql
                        );


                    // Get identification properties
                    while(rit.next()) {
                        newNode.statements << Soprano::Statement(
                                               next,
                                               rit.binding("p"),
                                               rit.binding("o")
                                           );
                    }

                    answer.serviceAddNode(next, newNode);

                    // Add all non-parsed nodes to queue
                    foreach(const QUrl & child, node.childrens()) {
                        queue << child;
                    }
                }
            }

            // Return answer
            return answer;
        }




}

// Build graph from scratch

}
#endif

NS::IdentificationSet ND::IdentificationSetManager::identificationSet(const QUrl & resourceUrl)
{
    return NS::IdentificationSet::fromResource(resourceUrl, d->mainModel);
}
