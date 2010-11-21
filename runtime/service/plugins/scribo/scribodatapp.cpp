/*
   Copyright (C) 2010 by Sebastian Trueg <trueg@kde.org>

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

#include "scribodatapp.h"
#include "scribodatappreply.h"

#include <webextractor/datappreply.h>

#include <Nepomuk/File>
#include <Nepomuk/Variant>
#include <Nepomuk/Vocabulary/NIE>

Nepomuk::ScriboDataPP::ScriboDataPP()
    : DataPP(1)
{
}


Nepomuk::WebExtractor::DataPPReply* Nepomuk::ScriboDataPP::requestDecisions(const WebExtractor::DecisionFactory* factory, const Nepomuk::Resource & res)
{
    QString text = res.property( Nepomuk::Vocabulary::NIE::plainTextContent() ).toString();
    if( text.isEmpty() ) {
        kDebug() << "No plainTextContent";
        return 0;
    }
    else {
        return new ScriboReply(this, factory, res, text);
    }
}

Nepomuk::ScriboDataPP::~ScriboDataPP()
{
}

#include "scribodatapp.moc"
