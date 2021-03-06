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

#include "simpleexecutivereply.h"
#include "simpleexecutivereply_p.h"
#include "executive.h"

#include <decision/decisionfactory.h>

namespace NW = Nepomuk::WebExtractor;
namespace ND = Nepomuk::Decision;

NW::SimpleExecutiveReply::SimpleExecutiveReply(Executive * parent, const ND::DecisionFactory * factory, const Nepomuk::Resource & res):
    ExecutiveReply(*new SimpleExecutiveReplyPrivate(factory->newDecisionList(parent)), parent)
{
    Q_D(SimpleExecutiveReply);
    d->m_factory = factory;
    d->m_res = res;
}

NW::SimpleExecutiveReply::SimpleExecutiveReply(Executive * parent, const ND::DecisionFactory * factory, const Nepomuk::Resource & res , SimpleExecutiveReplyPrivate & p):
    ExecutiveReply(p, parent)
{
    Q_D(SimpleExecutiveReply);
    d->m_res = res;
    d->m_factory = factory;
}


void NW::SimpleExecutiveReply::finish()
{
    // Call realFinish() with qt metacall
    QMetaObject::invokeMethod(this, "realFinish", Qt::QueuedConnection);
}

void  NW::SimpleExecutiveReply::realFinish()
{
    // If there is no error, then emit only finished signal.
    // If there is an error, them first emit error signal, then finished signal
    Q_D(SimpleExecutiveReply);
    if(d->m_error == NoError) {
        emitFinished();
    } else {
        emitError();
        return;
    }

}


ND::DecisionCreator NW::SimpleExecutiveReply::newDecision()
{
    Q_D(SimpleExecutiveReply);
    return d->m_factory->newDecision(parentExecutive());
}

ND::DecisionList NW::SimpleExecutiveReply::newDecisionList()
{
    Q_D(SimpleExecutiveReply);
    return d->m_factory->newDecisionList(parentExecutive());
}

ND::DecisionList NW::SimpleExecutiveReply::decisions() const
{
    Q_D(const SimpleExecutiveReply);
    return d->m_decisions;
}

const Nepomuk::Resource & NW::SimpleExecutiveReply::resource()
{
    Q_D(SimpleExecutiveReply);
    return d->m_res;
}

/*
NW::DecisionList & NW::SimpleExecutiveReply::mainDecisionList()
{
    Q_D(SimpleExecutiveReply);
    return d->m_decisions;
}
*/

void NW::SimpleExecutiveReply::addDecision(const ND::Decision & decision)
{
    Q_D(SimpleExecutiveReply);
    d->m_decisions.addDecision(decision);
    return;
}
void NW::SimpleExecutiveReply::addDecision(const ND::DecisionCreator & decisionCreator)
{
    addDecision(decisionCreator.data());
}
void NW::SimpleExecutiveReply::addDecision(const ND::Decision & decision,
        ND::MergePolitics politics, double coff , bool noAuto)
{
    Q_D(SimpleExecutiveReply);
    d->m_decisions.addDecision(decision, politics, coff, noAuto);
    return;
}

void NW::SimpleExecutiveReply::addDecision(
        const ND::DecisionCreator & decisionCreator,
        ND::MergePolitics politics, double coff , bool noAuto)
{
    addDecision(decisionCreator.data(),politics,coff,noAuto);
}

double NW::SimpleExecutiveReply::approximateThreshold() const
{
    Q_D(const SimpleExecutiveReply);
    return d->m_factory->approximateThreshold();
}
