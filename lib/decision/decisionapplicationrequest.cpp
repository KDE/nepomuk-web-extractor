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

#include "decisionapplicationrequest.h"
#include "decisionapplicationrequest_p.h"
#include "algorithm.h"

#include <nepomuk/datamanagement.h>

namespace ND = Nepomuk::Decision;


ND::DecisionApplicationRequest::DecisionApplicationRequest(const Decision & target, Soprano::Model * targetModel, QObject * parent):
    QObject(parent),
    d_ptr(new DecisionApplicationRequestPrivate())
{
    Q_D(DecisionApplicationRequest);
    d->decision = target;
    d->applied = false;
    d->changes = target.changes();
}

ND::DecisionApplicationRequest::DecisionApplicationRequest(const Decision & target, Soprano::Model * targetModel, DecisionApplicationRequestPrivate & p, QObject * parent):
    QObject(parent),
    d_ptr(&p)
{
    Q_D(DecisionApplicationRequest);
    d->decision = target;
    d->applied = false;
    d->changes = target.changes();
}


bool ND::DecisionApplicationRequest::apply()
{
    Q_D(DecisionApplicationRequest);
    if(d->applied)
        return true;

    KJob * job = applyJob();
    if (!job.exec()) {
        kDebug() << "Decision application failed";
        return false;
    }
    return true;
}

KJob * ND::DecisionApplicationRequest::applyJob()
{
    Q_D(DecisionApplicationRequest);
    if(d->applied)
        return 0;

    KJob * answer = storeResources(d->changes,Nepomuk::IdentifyNew);
    connect(answer,SIGNAL(finished(KJob*)),
            this,SIGNAL(finished(KJob*))
           );

    return answer;

}


ND::DecisionApplicationRequest::~DecisionApplicationRequest()
{
}

QSet<QUrl> ND::DecisionApplicationRequest::targetResources() const
{
#warning BEHAVE INCORRECTLY! DO NOT USE
    Q_D(const DecisionApplicationRequest);
    return d->decision.targetResources();
}
