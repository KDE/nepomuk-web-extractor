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

#include "global.h"
#include "config.h"
#include <KDebug>

namespace NW = Nepomuk::WebExtractor;

// Some static variables
int m_defaultExecutiveTimeout = DEFAULT_DATAPP_TIMEOUT;




double NW::minScaleCoff()
{
    return -0.9999;
}

double NW::maxScaleCoff()
{
    return 0.9999;
}


double NW::boundScaleCoff(double val)
{
    return qBound(minScaleCoff(), maxScaleCoff(), val);
}

int NW::defaultExecutiveTimeout()
{
    return m_defaultExecutiveTimeout;
}
/*
Soprano::Model * NW::decisionsModel()
{
    // Find necessary backend
    static Soprano::Backend * backend = Soprano::discoverBackendByFeatures(Soprano::BackendOptionStorageMemory);
    static Soprano::Model * val = backend->createModel(Soprano::BackendOptionStorageMemory);
    return val;
}

*/
