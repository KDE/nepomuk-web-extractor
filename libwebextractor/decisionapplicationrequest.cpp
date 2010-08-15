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


#include "decisionapplicationrequest.h"
#include "decisionapplicationrequest_p.h"
#include <nepomuk/mergerequest.h>
#include "algorithm.h"

namespace NW = Nepomuk::WebExtractor;
namespace NS = Nepomuk::Sync;

NW::DecisionApplicationRequest::DecisionApplicationRequest(QSharedPointer<DecisionData> target, Nepomuk::Sync::ChangeLog log, Soprano::Model * targetModel, QObject * parent):
    QObject(parent),
    d_ptr(new DecisionApplicationRequestPrivate())
{
    Q_D(DecisionApplicationRequest);
    d->decisionData = target;
    d->applied = false;
    d->identified = false;
    d->targetModel = targetModel;
    d->log = log;
}

NW::DecisionApplicationRequest::DecisionApplicationRequest(QSharedPointer<DecisionData>  target, Nepomuk::Sync::ChangeLog log, Soprano::Model * targetModel, DecisionApplicationRequestPrivate & p, QObject * parent):
    QObject(parent),
    d_ptr(&p)
{
    Q_D(DecisionApplicationRequest);
    d->decisionData = target;
    d->applied = false;
    d->identified = false;
    d->targetModel = targetModel;
    d->log = log;
}

void NW::DecisionApplicationRequest::reset()
{
    Q_D(DecisionApplicationRequest);
    // Clear all
    delete d->mainRequest;
    d->mainRequest = 0;
    d->identified = false;
    d->applied = false;
    d->mainResourceIdentificationHash.clear();
}


void NW::DecisionApplicationRequest::identify(bool reset)
{
    Q_D(DecisionApplicationRequest);
    if(d->identified and !reset)
        return;

    if(reset) {
        this->reset();
    }

    d->identified = false;

    // Firts create main identification request.
    // But do not launch it. In this iset the identification statements
    // of resources that are Decision target resources should be excluded.
    QSet<QUrl> ignoreset;
    QHash<QUrl, NS::IdentificationSet>::const_iterator isit =
        d->decisionData->resourceProxyISMap.begin();
    QHash<QUrl, NS::IdentificationSet>::const_iterator isit_end =
        d->decisionData->resourceProxyISMap.end();
    for(; isit != isit_end; isit++) {
        ignoreset << d->decisionData->resourceProxyMap[isit.key()];
    }
    /* Debugging section */
#if 0
    QSet<QUrl> chlUri = d->log.resources();
    if(chlUri.size() > 0) {
        kDebug() << "Dump resources in changelog";
        QTextStream ostr(stdout);
        dump_resources_as_text(chlUri, d->decisionData->decisionsModel, ostr, -1);
    } else {
        kDebug() << "No resource in changelog";
    }
#endif
    /* End of debugging section */

    NS::IdentificationSet iset = NS::IdentificationSet::fromChangeLog(d->log, d->decisionData->decisionsModel, ignoreset);
    kDebug() << "Decision identification set" << iset.toList();
    d->mainRequest = new NS::IdentificationRequest(d->log, iset, d->targetModel);
    d->mainRequest->load();

    kDebug() << "Main unidentified: " << d->mainRequest->unidentified();

    // Now it is necessary to identify the Decision main resources.

    QHash<QUrl, NS::IdentificationSet>::const_iterator it = d->decisionData->resourceProxyISMap.begin();
    QHash<QUrl, NS::IdentificationSet>::const_iterator it_end = d->decisionData->resourceProxyISMap.end();
    for(; it != it_end; it++) {
        // proxyUrl is like url of the main resource in the Decision storage model
        // Thats why it is called 'proxy'
        QUrl proxyUrl = d->decisionData->resourceProxyMap[it.key()];
        Q_ASSERT(!proxyUrl.isEmpty());

        NS::IdentificationRequest * req = new NS::IdentificationRequest(NS::ChangeLog(), it.value(), d->targetModel);
        connect(req, SIGNAL(identified(QUrl, QUrl)), this, SIGNAL(identified(QUrl, QUrl)));
        req->load();
        req->identifyAll();

        if(!req->done()) {
            kDebug() << "Identification of target resource failed";
            return;
        }

        // The identified uri in the target model
        // mpUri == (m)a(p)ped (Uri)
        QUrl mpUri = req->mappedUri(it.key());
        kDebug() << "Target resource " << it.key() << " was identified as :" <<
                 mpUri << '\n' <<
                 "It's proxy (" << proxyUrl << ")" <<
                 "will be forced to match this resource";


        // Add to the main request database
        d->mainRequest->provideResourceUri(
            proxyUrl,
            mpUri
        );

        // Add to our own database
        d->mainResourceIdentificationHash.insert(proxyUrl, mpUri);

        delete req;
    }

    // Start main identification
    d->mainRequest->identifyAll();

    d->identified = true;




}

bool NW::DecisionApplicationRequest::apply()
{
    Q_D(DecisionApplicationRequest);
    if(d->applied)
        return true;

    if(!d->identified)
        identify();
    else
        kDebug() << "Already identified";

    if(!d->identified)  {
        kDebug() << "Identification failed";
        return false;
    }


    //if(!d->mainRequest->done()) {
    // This check is WRONG! The previous one is right, but doesn't work :(
    if(!d->mainRequest->unidentified().isEmpty()) {

        kError() << "Identification was performed and failed.";
        return false;
    }

    NS::ChangeLog newChangeLog = d->mainRequest->convert();
    NS::MergeRequest mergeReq(newChangeLog, d->targetModel);
    mergeReq.merge();

    d->applied = true;
    return true;
}

QHash<QUrl, QUrl> NW::DecisionApplicationRequest::mappings() const
{
    Q_D(const DecisionApplicationRequest);
    if(!d->mainRequest)
        return QHash<QUrl, QUrl>();

    return  d->mainRequest->mappings();
}

QSet<QUrl> NW::DecisionApplicationRequest::unidentified() const
{
    Q_D(const DecisionApplicationRequest);
    if(!d->mainRequest)
        return QSet<QUrl>();

    return  d->mainRequest->unidentified();
}

QHash<QUrl, QUrl> NW::DecisionApplicationRequest::mainMappings() const
{
    Q_D(const DecisionApplicationRequest);
    return d->mainResourceIdentificationHash;
}

NW::DecisionApplicationRequest::~DecisionApplicationRequest()
{
    reset();
}

QSet<QUrl> NW::DecisionApplicationRequest::mainResources() const
{
    Q_D(const DecisionApplicationRequest);
    // If mainResources is already cached, then return them
    if(d->mainResources.isEmpty()) {

        QSet<QUrl> changeLogResources = d->log.resources();
        // Iterate over all main resources of the Decision.
        foreach(const QUrl & proxyUrl, d->decisionData->resourceProxyMap.values()) {
            // If proxyUrl( aka main Decision resource) is contained in changelog,
            // then it is main resource for this application request.
            // If not - then it is not.
            if(changeLogResources.contains(proxyUrl))
                d->mainResources.insert(proxyUrl);
        }
    }

    return d->mainResources;
}
