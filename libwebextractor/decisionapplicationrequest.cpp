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

#include "mergerequest.h"
#include "identificationset.h"

#include <KDebug>
#include <Soprano/Statement>

#include "decisionapplicationrequest.h"
#include "decisionapplicationrequest_p.h"
#include "algorithm.h"

namespace NW = Nepomuk::WebExtractor;
namespace NS = Nepomuk::Sync;

NW::DecisionApplicationRequest::DecisionApplicationRequest(const Decision & target, Soprano::Model * targetModel, QObject * parent):
    QObject(parent),
    d_ptr(new DecisionApplicationRequestPrivate())
{
    Q_D(DecisionApplicationRequest);
    d->decision = target;
    d->applied = false;
    d->identified = false;
    d->targetsIdentified = false;
    d->targetModel = targetModel;
    d->log = target.log();
}

NW::DecisionApplicationRequest::DecisionApplicationRequest(const Decision & target, Soprano::Model * targetModel, DecisionApplicationRequestPrivate & p, QObject * parent):
    QObject(parent),
    d_ptr(&p)
{
    Q_D(DecisionApplicationRequest);
    d->decision = target;
    d->applied = false;
    d->identified = false;
    d->targetsIdentified = false;
    d->targetModel = targetModel;
    d->log = target.log();
}

void NW::DecisionApplicationRequest::reset()
{
    Q_D(DecisionApplicationRequest);
    // Clear all
    delete d->mainRequest;
    d->mainRequest = 0;
    d->identified = false;
    d->applied = false;
    d->targetsIdentified  = false;
    d->targetResourceIdentificationHash.clear();
}


void NW::DecisionApplicationRequest::identifyTargets(bool reset)
{
    Q_D(DecisionApplicationRequest);
    if(d->targetsIdentified and !reset)
        return;

    if(reset) {
        DecisionApplicationRequest::reset();
    }

    // Now it is necessary to identify the Decision target resources.

    bool success = true;
    d->decision.cleanUnused();
    QSet<QUrl> targetResources = d->decision.targetResources();
    // Now, because we have cleaned unused identifications sets,
    // we can iterate over sets hash as if we would iterate over target resources set
    QHash<QUrl, NS::IdentificationSet>::const_iterator it = d->decision.identificationSets().begin();
    QHash<QUrl, NS::IdentificationSet>::const_iterator it_end = d->decision.identificationSets().end();
    for(; it != it_end; it++) {
        // sourceUrl is(was)  url of the target resource in it's identification set
        // Thats why it is called 'source'
        QUrl sourceUrl = it.key();
        Q_ASSERT(!sourceUrl.isEmpty());

        kDebug() << "Identification set for the resource " << sourceUrl << "is:";
        kDebug() << it.value().toList();

        NS::IdentificationRequest * req = new NS::IdentificationRequest(NS::ChangeLog(), it.value(), d->targetModel);
        connect(req, SIGNAL(identified(QUrl, QUrl)), this, SIGNAL(identified(QUrl, QUrl)));
        req->load();

        kDebug() << "The following resources are going to be identified: " << req->unidentified();
        req->identifyAll();

        if(!req->done()) {
            kDebug() << "Identification of target resource failed. Unidentified:";
            kDebug() << req->unidentified();
            success = false;
            break;
        }

        // The identified uri in the target model
        // mpUri == (m)a(p)ped (Uri)
        QUrl mpUri = req->mappedUri(sourceUrl);
	kDebug() << "Source url " << sourceUrl << "was identified as : " << mpUri ; 
        Q_ASSERT(!mpUri.isEmpty());

	// Now we have identified so-called SOURCE url.
	// But in identification hash we will add mapping for
	// PROXY url, because proxy url is used in changelog
	QUrl proxyUrl;
	QHash<QUrl,QUrl>::const_iterator fit = 
	    d->decision.resourceProxyMap().find(sourceUrl);

	if ( fit == d->decision.resourceProxyMap().end() ) { 
	    // Resource is not in resourceProxyMap. This mean that proxy is the same as
	    // source
	    proxyUrl = it.key();
	}
	else {
	    proxyUrl = fit.value();
	}

        kDebug() << "Target resource " << proxyUrl << " was identified as :" <<
                 mpUri ;
		 /* <<  '\n' <<
                 "It's proxy (" << proxyUrl << ")" <<
                 "will be forced to match this resource";*/



        // Add to our own database
        d->targetResourceIdentificationHash.insert(proxyUrl, mpUri);

        delete req;
    }

    // Set targetsIdentified
    d->targetsIdentified = success;
}

void NW::DecisionApplicationRequest::identify(bool reset)
{
    Q_D(DecisionApplicationRequest);
    // If identification stage has already be successfuly performed
    // and it should not be reseted
    if(d->identified and !reset)
        return;

    if(reset) {
        this->reset();
    }

    d->identified = false;

    // Identify main resources
    if(!d->targetsIdentified) {
        identifyTargets();
        // If identification was unsuccessfull,
        // then return
        if(!d->targetsIdentified) {
            return;
        }
    }

    // Create main identification request.
    // But do not launch it. In this iset the identification statements
    // of resources that are Decision target resources should be excluded.
    /*
    QSet<QUrl> ignoreset;
    QHash<QUrl, QUrl>::const_iterator prit =
        d->targetResourceIdentificationHash.begin();
    QHash<QUrl, NS::IdentificationSet>::const_iterator prit_end =
        d->targetResourceIdentificationHash.end();
    // Iteration is done over identificationSets()
    // ( the hash proxy resource -> identification set ),
    for(; prit != prit_end; prit++) {
        ignoreset << prit.key();
    }
    */
    /* Debugging section */
#if 0
    QSet<QUrl> chlUri = d->log.resources();
    if(chlUri.size() > 0) {
        kDebug() << "Dump resources in changelog";
        QTextStream ostr(stdout);
        dump_resources_as_text(chlUri, d->decision.decisionsModel, ostr, -1);
    } else {
        kDebug() << "No resource in changelog";
    }
#endif
    /* End of debugging section */

    NS::IdentificationSet iset = d->decision.auxiliaryIdentificationSet();
    kDebug() << "Decision identification set" << iset.toList();
    d->mainRequest = new NS::IdentificationRequest(d->log, iset, d->targetModel);
    d->mainRequest->load();

    QHash<QUrl, QUrl>::const_iterator mit = d->targetResourceIdentificationHash.begin();
    QHash<QUrl, QUrl>::const_iterator mit_end = d->targetResourceIdentificationHash.end();

    // Add main resources to the main request database
    for(; mit != mit_end; mit++) {
        d->mainRequest->provideResourceUri(
            mit.key(),
            mit.value()
        );
    }

    kDebug() << "Main unidentified: " << d->mainRequest->unidentified();


    // Start main identification
    d->mainRequest->identifyAll();

    d->identified = d->mainRequest->done();

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

QHash<QUrl, QUrl> NW::DecisionApplicationRequest::targetsMappings() const
{
    Q_D(const DecisionApplicationRequest);
    return d->targetResourceIdentificationHash;
}

bool NW::DecisionApplicationRequest::isTargetsIdentified() const
{
    Q_D(const DecisionApplicationRequest);
    return d->targetsIdentified;
}

NW::DecisionApplicationRequest::~DecisionApplicationRequest()
{
    reset();
}

QSet<QUrl> NW::DecisionApplicationRequest::targetResources() const
{
#warning BEHAVE INCORRECTLY! DO NOT USE
    Q_D(const DecisionApplicationRequest);
    return d->decision.targetResources();
}
