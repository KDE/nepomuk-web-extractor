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
#include "datapp.h"

#include <QtCore/QSharedData>


namespace NW = Nepomuk::WebExtractor;

class NW::DataPPWrapper::Private : public QSharedData
{
    public:
        DataPP * data;
        double rank;
        double scaleCoff;
};

NW::DataPPWrapper::DataPPWrapper(DataPP * dpp, const QString & name, double rank, double scaleCoff):
    d(new Private())
{
    Q_CHECK_PTR(dpp);
    d->data = dpp;
    dpp->setName(name);
    d->rank = rank;
    d->scaleCoff = scaleCoff;
}

NW::DataPPWrapper::DataPPWrapper(const DataPPWrapper & rhs):
    d(rhs.d)
{
    ;
}

NW::DataPPWrapper & NW::DataPPWrapper::operator=(const DataPPWrapper & rhs)
{
    this->d = rhs.d;
}

NW::DataPPWrapper::~DataPPWrapper()
{
    ;
}

NW::DataPPReply * NW::DataPPWrapper::DataPPWrapper::requestDecisions(const DecisionFactory * factory, const Nepomuk::Resource & res) const
{
    DataPPReply * answer = d->data->requestDecisions(factory, res);
    /*
    if (!answer)
    return answer;

    answer->setPluginName(d->pluginName());
    */
    return answer;
}
NW::DataPP * NW::DataPPWrapper::data() const
{
    return d->data;
}
double NW::DataPPWrapper::rank() const
{
    return d->rank;
}
double NW::DataPPWrapper::coff() const
{
    return d->scaleCoff;
}
void NW::DataPPWrapper::setRank(double val)
{
    d->rank = val;
}
QString  NW::DataPPWrapper::name() const
{
    return d->data->name();
}
int  NW::DataPPWrapper::version() const
{
    return d->data->version();
}
