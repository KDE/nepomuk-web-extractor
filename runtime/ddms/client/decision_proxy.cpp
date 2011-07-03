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

#include "decision_proxy.h"
#include "decision_proxy_data.h"
#include "client.h"

using namespace Nepomuk;

DecisionProxy::DecisionProxy( DecisionManagementClient * client,
        int id)
{
    this->d = client->getDecisionData(id,true);
}

DecisionProxy::DecisionProxy() 
{;}

bool DecisionProxy::isValid() const
{
    return (!d.isNull()) and (d->valid);
}

QString DecisionProxy::description() const
{
    d->loadMetadata();
    return d->description;
}


DecisionProxy::~DecisionProxy()
{;}

DecisionProxy::DecisionProxy(const DecisionProxy & rhs):
    d(rhs.d)
{
}

DecisionProxy & DecisionProxy::operator=(const DecisionProxy & rhs )
{
    this->d = rhs.d;
    return *this;
}
