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

#include "debug_datapp.h"
#include <QtCore/QTimer>
#include <KDebug>
#include <stdint.h>
#include <stdlib.h>

namespace NW = Nepomuk::WebExtractor;

QString NW::DebugDataPP::name()
{
    static QString name = QString("debug");
    return name;
}

int  NW::DebugDataPP::version()
{
    //static QString version = QString("0.0.1");
    return 0.1;
}

NW::DebugDataPP::DebugDataPP():
    DataPP(DebugDataPP::version())
{
    kDebug() << "New DebugDataPP created. ID: " << uintptr_t(this);
}

NW::DataPPReply * NW::DebugDataPP::requestDecisions(const NW::DecisionFactory * factory, const Nepomuk::Resource & res)
{
    return new DebugDataPPReply(this, factory);
}

NW::DebugDataPPReply::DebugDataPPReply(DebugDataPP* parent, const DecisionFactory * factory):
    DataPPReply(parent),
    //m_fact(factory),
    m_decisions(factory->newDecisionList(parent))
{
    kDebug() << "New DebugDataPPReply created. ID: " << uintptr_t(this);
    m_timer = new QTimer(this);
    m_timer->setSingleShot(true);
    connect(m_timer, SIGNAL(timeout()), this, SLOT(ready()));
    for(double r = 0; r < 0.8; r += 0.1) {
        DecisionCreator d = factory->newDecision(parent);
        d.setRank(r);
        m_decisions.addDecision(d.data());
    }
    m_timer->start(2 * 1000);
}

void NW::DebugDataPPReply::abort()
{
    ;
}

void NW::DebugDataPPReply::ready()
{
    kDebug() << " DebugDataPPReply ready. ID: " << uintptr_t(this);
    emit finished();
}

bool NW::DebugDataPPReply::isValid() const
{
    return true;
}



NW::DebugDataPPReply::~DebugDataPPReply()
{
    delete m_timer;
}
