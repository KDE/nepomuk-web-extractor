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

Nepomuk::WebExtractor::DataPP::DataPP(int  dataPPVersion)
{
    d = new Private();
    d->m_version = dataPPVersion;
}

void Nepomuk::WebExtractor::DataPP::setName(const QString & name)
{
    d->m_name = name;
}
QString Nepomuk::WebExtractor::DataPP::name() const
{
    return d->m_name;
}

int Nepomuk::WebExtractor::DataPP::version() const
{
    return d->m_version;
}

