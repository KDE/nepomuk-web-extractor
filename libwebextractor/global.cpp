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

#include "global.h"
#include "config.h"
#include <KDebug>

namespace NW = Nepomuk::WebExtractor;

// Some static variables
int m_defaultDataPPTimeout = DEFAULT_DATAPP_TIMEOUT;

double NW::minACrit()
{
    static double val = 0.0;
    return val;
}

double NW::maxACrit()
{
    static double val = 1.0;
    return val;
}

double NW::minUCrit()
{
    return minACrit();
}

double NW::maxUCrit()
{
    static double val = 0.999;
    return val;
}

double NW::boundACrit(double val)
{
    if(val < minACrit()) {
        kDebug() << "ACrit value is less than minimun. Fixing";
        return minACrit();
    } else if(val > maxACrit()) {
        kDebug() << "ACrit value is greater than maximum. Fixing";
        return maxACrit();
    }

    return val;

}

double NW::boundUCrit(double val)
{
    if(val < minUCrit()) {
        kDebug() << "UCrit value is less than minimun. Fixing";
        return minUCrit();
    } else if(val > maxUCrit()) {
        kDebug() << "UCrit value is greater than maximum. Fixing";
        return maxUCrit();
    }

    return val;

}

double NW::minRank()
{
    return 0.0;
}

double NW::maxRank()
{
    return 0.9999;
}


double NW::boundRank(double val)
{
    return qBound(minRank(), maxRank(), val);
}

double NW::noAutoRank()
{
    return NO_AUTO_RANK;
}

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

int NW::defaultDataPPTimeout()
{
    return m_defaultDataPPTimeout;
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
