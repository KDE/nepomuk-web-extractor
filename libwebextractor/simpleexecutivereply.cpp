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
#include "decisionfactory.h"
#include "simpleexecutivereply_p.h"

namespace NW = Nepomuk::WebExtractor;

NW::SimpleExecutiveReply::SimpleExecutiveReply(Executive * parent, const DecisionFactory * factory, const Nepomuk::Resource & res):
    ExecutiveReply(*new SimpleExecutiveReplyPrivate(factory->newDecisionList(parent)), parent)
{
    Q_D(SimpleExecutiveReply);
    d->m_factory = factory;
    d->m_res = res;
}

NW::SimpleExecutiveReply::SimpleExecutiveReply(Executive * parent, const DecisionFactory * factory, const Nepomuk::Resource & res , SimpleExecutiveReplyPrivate & p):
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
        emit finished();
        return;
    } else {
        emit ExecutiveReply::error(d->m_error);
        return;
    }

}


NW::DecisionCreator NW::SimpleExecutiveReply::newDecision()
{
    Q_D(SimpleExecutiveReply);
    return d->m_factory->newDecision(parentExecutive());
}

NW::DecisionList NW::SimpleExecutiveReply::newDecisionList()
{
    Q_D(SimpleExecutiveReply);
    return d->m_factory->newDecisionList(parentExecutive());
}

NW::DecisionList NW::SimpleExecutiveReply::decisions() const
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

void NW::SimpleExecutiveReply::addDecision(const Decision & decision)
{
    Q_D(SimpleExecutiveReply);
    d->m_decisions.addDecision(decision);
    return;
}
void NW::SimpleExecutiveReply::addDecision(const DecisionCreator & decisionCreator)
{
    addDecision(decisionCreator.data());
}
void NW::SimpleExecutiveReply::addDecision(const Decision & decision , MergePolitics politics, double coff , bool noAuto)
{
    Q_D(SimpleExecutiveReply);
    d->m_decisions.addDecision(decision, politics, coff, noAuto);
    return;
}

void NW::SimpleExecutiveReply::addDecision(const DecisionCreator & decisionCreator , MergePolitics politics, double coff , bool noAuto)
{
    addDecision(decisionCreator.data(),politics,coff,noAuto);
}

double NW::SimpleExecutiveReply::approximateThreshold() const
{
    Q_D(const SimpleExecutiveReply);
    return d->m_factory->approximateThreshold();
}
