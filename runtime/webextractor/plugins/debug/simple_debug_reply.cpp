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

#include "simple_debug_reply.h"
#include "executive.h"
#include "simplenetworkexecutive.h"
#include "simplenetworkrequest.h"
#include <decision/decisionfactory.h>
#include <KDebug>
#include <stdint.h>

namespace NW = Nepomuk::WebExtractor;
namespace ND = Nepomuk::Decision;

Nepomuk::SimpleDebugReply::SimpleDebugReply(NW::SimpleNetworkExecutive * parent , const ND::DecisionFactory * factory, const Nepomuk::Resource & res):
    SimpleNetworkExecutiveReply(parent, factory, res),
    m_state(0),
    m_decisions(factory->newDecisionList(parent))
{
}

void Nepomuk::SimpleDebugReply::start()
{
    step();
}

void Nepomuk::SimpleDebugReply::requestFinished()
{
    if(m_state == 10) {
        kDebug() << "SimpleDebugReply " << uintptr_t(this) << " finished";
        emit finished();
    } else {
        step();
    }

    // Anyway delete sender
    sender()->deleteLater();
}

void Nepomuk::SimpleDebugReply::step()
{
    // Create request and connect it
    NW::SimpleNetworkExecutiveRequest * req = new NW::SimpleNetworkExecutiveRequest("");
    connect(req, SIGNAL(finished()), this, SLOT(requestFinished()));
    connect(req, SIGNAL(error()), this, SLOT(requestError()));

    NW::SimpleNetworkExecutive * dpp = qobject_cast<NW::SimpleNetworkExecutive*>(this->parentExecutive());
    dpp->get(req);
    kDebug() << "SimpleDebugReply: " << uintptr_t(this) << " Step number: " << m_state;
    m_state++;
}

void Nepomuk::SimpleDebugReply::requestError()
{
    // Stop at this step
    kDebug() << "SimpleDebugReply " << uintptr_t(this) << " finished with error";
    sender()->deleteLater();
    emit error();
}
