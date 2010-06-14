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
#include <webextractor/decisionfactory.h>
#include <webextractor/datapp.h>
#include <webextractor/simple_datapp.h>
#include <KDebug>
#include <stdint.h>

namespace NW=Nepomuk::WebExtractor;

Nepomuk::SimpleDebugReply::SimpleDebugReply(NW::SimpleDataPP * parent , const NW::DecisionFactory * factory, const Nepomuk::Resource & res):
    SimpleDataPPReply(parent,factory, res),
    m_state(0),
    m_decisions(factory->newDecisionList(parent))
{
}

void Nepomuk::SimpleDebugReply::step()
{
    if (m_state == 10) {
	kDebug() << "SimpleDebugReply "<< uintptr_t(this) <<" finished";
	emit finished();
	return;
    }
    else {
	// Enqueue ourself again
	reinterpret_cast<NW::SimpleDataPP*>(this->parent())->enqueue(this);
	kDebug() << "SimpleDebugReply: " << uintptr_t(this) << " Step number: "<<m_state;
	m_state++;
    }
}
