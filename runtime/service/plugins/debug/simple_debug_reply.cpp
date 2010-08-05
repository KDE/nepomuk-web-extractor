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
#include "decisionfactory.h"
#include "datapp.h"
#include "simplenetworkdatapp.h"
#include "simplenetworkrequest.h"
#include <KDebug>
#include <stdint.h>

namespace NW = Nepomuk::WebExtractor;

Nepomuk::SimpleDebugReply::SimpleDebugReply(NW::SimpleNetworkDataPP * parent , const NW::DecisionFactory * factory, const Nepomuk::Resource & res):
    SimpleNetworkDataPPReply(parent, factory, res),
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
    NW::SimpleNetworkDataPPRequest * req = new NW::SimpleNetworkDataPPRequest("");
    connect(req, SIGNAL(finished()), this, SLOT(requestFinished()));
    connect(req, SIGNAL(error()), this, SLOT(requestError()));

    NW::SimpleNetworkDataPP * dpp = qobject_cast<NW::SimpleNetworkDataPP*>(this->parentDataPP());
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
