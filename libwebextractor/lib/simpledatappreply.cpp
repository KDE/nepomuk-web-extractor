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

#include <webextractor/simpledatappreply.h>
#include <webextractor/decisionfactory.h>
#include "simpledatappreply_p.h"

namespace NW = Nepomuk::WebExtractor;

NW::SimpleDataPPReply::SimpleDataPPReply(DataPP * parent, const DecisionFactory * factory, const Nepomuk::Resource & res):
    DataPPReply(*new SimpleDataPPReplyPrivate(factory->newDecisionList(parent)), parent)
{
    Q_D(SimpleDataPPReply);
    d->m_factory = factory;
    d->m_res = res;
}

NW::SimpleDataPPReply::SimpleDataPPReply(DataPP * parent, const DecisionFactory * factory, const Nepomuk::Resource & res , SimpleDataPPReplyPrivate & p):
    DataPPReply(p, parent)
{
    Q_D(SimpleDataPPReply);
    d->m_res = res;
    d->m_factory = factory;
}

NW::Decision NW::SimpleDataPPReply::newDecision()
{
    Q_D(SimpleDataPPReply);
    return d->m_factory->newDecision(parentDataPP());
}

NW::DecisionList NW::SimpleDataPPReply::newDecisionList()
{
    Q_D(SimpleDataPPReply);
    return d->m_factory->newDecisionList(parentDataPP());
}

NW::DecisionList NW::SimpleDataPPReply::decisions() const
{
    Q_D(const SimpleDataPPReply);
    return d->m_decisions;
}

const Nepomuk::Resource & NW::SimpleDataPPReply::resource()
{
    Q_D(SimpleDataPPReply);
    return d->m_res;
}

NW::DecisionList & NW::SimpleDataPPReply::mainDecisionList()
{
    Q_D(SimpleDataPPReply);
    return d->m_decisions;
}
