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

#include "datapp.h"
#include <Nepomuk/ResourceManager>
#include <Nepomuk/Variant>
#include <Nepomuk/Query/Query>
#include <Nepomuk/Query/ComparisonTerm>
#include <Nepomuk/Query/Term>
#include <Nepomuk/Query/NegationTerm>
#include <Nepomuk/Query/LiteralTerm>
#include <Nepomuk/Query/AndTerm>
#include <Soprano/QueryResultIterator>
#include <Soprano/Vocabulary/NAO>
#include <Soprano/Vocabulary/RDFS>
#include <Soprano/BindingSet>
#include "ndco.h"


namespace NQ = Nepomuk::Query;
namespace NW = Nepomuk::WebExtractor;


class Nepomuk::WebExtractor::DataPP::Private
{
    public:
        int m_version;
        QString m_name;
};
Nepomuk::WebExtractor::DataPP::~DataPP()
{
    ;
}

Nepomuk::WebExtractor::DataPP::DataPP(int  pluginVersion)
{
    d = new Private();
    d->m_version = pluginVersion;
}

void Nepomuk::WebExtractor::DataPP::setPluginName(const QString & name)
{
    d->m_name = name;
}
QString Nepomuk::WebExtractor::DataPP::pluginName() const
{
    return d->m_name;
}

int Nepomuk::WebExtractor::DataPP::pluginVersion() const
{
    return d->m_version;
}

