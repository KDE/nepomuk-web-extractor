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

#include "olenaplugin_config.h"
#include "olenadatappreply.h"
#include "olenadatapp.h"

#include <webextractor/datappreply.h>

#include <Nepomuk/File>

Nepomuk::OlenaDataPP::OlenaDataPP( int pluginVersion )
    : DataPP(pluginVersion)
{
}


Nepomuk::OlenaDataPP::~OlenaDataPP()
{
}


Nepomuk::WebExtractor::DataPPReply* Nepomuk::OlenaDataPP::requestDecisions(const WebExtractor::DecisionFactory* factory, const Nepomuk::Resource& res)
{
    if( res.isFile() ) {
        const KUrl url = res.toFile().url();
        if( url.isLocalFile() ) {
            return new OlenaReply(this, factory, res);
        }
    }
    return 0;
}
