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

#include "executivewrapper.h"
#include "executive.h"

#include <QtCore/QSharedData>


namespace NW = Nepomuk::WebExtractor;
namespace ND = Nepomuk::Decision;

class NW::ExecutiveWrapper::Private : public QSharedData
{
    public:
        Executive * data;
        double rank;
        double scaleCoff;
};

NW::ExecutiveWrapper::ExecutiveWrapper(Executive * dpp, const QString & name, double rank, double scaleCoff):
    d(new Private())
{
    Q_CHECK_PTR(dpp);
    d->data = dpp;
    dpp->setName(name);
    d->rank = rank;
    d->scaleCoff = scaleCoff;
}

NW::ExecutiveWrapper::ExecutiveWrapper(const ExecutiveWrapper & rhs):
    d(rhs.d)
{
    ;
}

NW::ExecutiveWrapper & NW::ExecutiveWrapper::operator=(const ExecutiveWrapper & rhs)
{
    this->d = rhs.d;
    return *this;
}

NW::ExecutiveWrapper::~ExecutiveWrapper()
{
    ;
}

NW::ExecutiveReply * NW::ExecutiveWrapper::ExecutiveWrapper::requestDecisions(const ND::DecisionFactory * factory, const Nepomuk::Resource & res) const
{
    ExecutiveReply * answer = d->data->requestDecisions(factory, res);
    /*
    if (!answer)
    return answer;

    answer->setPluginName(d->pluginName());
    */
    return answer;
}
NW::Executive * NW::ExecutiveWrapper::data() const
{
    return d->data;
}
double NW::ExecutiveWrapper::rank() const
{
    return d->rank;
}
double NW::ExecutiveWrapper::coff() const
{
    return d->scaleCoff;
}
void NW::ExecutiveWrapper::setRank(double val)
{
    d->rank = val;
}
QString  NW::ExecutiveWrapper::name() const
{
    return d->data->name();
}
int  NW::ExecutiveWrapper::version() const
{
    return d->data->version();
}
