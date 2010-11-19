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

#ifndef __webextractor_datapp_olena 
#define __webextractor_datapp_olena 

#include "datapp.h"

namespace Nepomuk {
    class OlenaReply;

    class OlenaDataPP : public WebExtractor::DataPP 
    {
        Q_OBJECT

    public:
        WebExtractor::DataPPReply* requestDecisions(const WebExtractor::DecisionFactory* factory, const Nepomuk::Resource& res) ;
        OlenaDataPP( int pluginVersion );
        ~OlenaDataPP();
    };
}

#endif 

