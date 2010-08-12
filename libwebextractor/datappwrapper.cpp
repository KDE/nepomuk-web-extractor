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

#include "datappwrapper.h"

namespace NW=Nepomuk::WebExtractor;

NW::DataPPWrapper::DataPPWrapper(DataPP * dpp,const QString & name, double rank, double scaleCoff)
{
    Q_CHECK_PTR(dpp);
    m_data = dpp;
    dpp->setPluginName(name);
    m_rank = rank;
    m_scaleCoff = scaleCoff;
}

NW::DataPPReply * NW::DataPPWrapper::DataPPWrapper::requestDecisions(const DecisionFactory * factory, const Nepomuk::Resource & res) const
{
    DataPPReply * answer = m_data->requestDecisions(factory,res);
    /*
    if (!answer)
	return answer;

    answer->setPluginName(d->pluginName());
    */
    return answer;
}