/*
   Copyright (C) 2011 by Serebriyskiy Artem <v.for.vandal at gmail.com>

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

#include "decision_proxy_data.h"
#include "client.h"

#include <QtDebug>

using namespace Nepomuk;

DecisionProxyData::DecisionProxyData(DecisionManagementClient * client,int id, bool checkExist):
    valid(false),
    m_metadataLoaded(false),
    m_client(client),
    m_id(id)
{
    Q_ASSERT(m_client);
    if ( checkExist )
        initialCheckExist();
}


void DecisionProxyData::loadMetadata()
{
    if (m_metadataLoaded)
        return;

    // client->request metadata
    m_metadataLoaded = true;
}

void DecisionProxyData::initialCheckExist()
{
    valid = m_client->existsDecision(m_id);
}

DecisionProxyData::~DecisionProxyData()
{;}
