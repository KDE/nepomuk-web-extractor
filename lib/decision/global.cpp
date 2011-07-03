/*
   Copyright (C) 2011 by Serebriyskiy Artem <v.for.vandal at gmail.com>

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

#include <KDebug>

namespace ND = Nepomuk::Decision;

double ND::minACrit()
{
    static double val = 0.0;
    return val;
}

double ND::maxACrit()
{
    static double val = 1.0;
    return val;
}

double ND::minUCrit()
{
    return minACrit();
}

double ND::maxUCrit()
{
    static double val = 0.999;
    return val;
}

double ND::minRank()
{
    return 0.0;
}

double ND::maxRank()
{
    return 0.9999;
}


double ND::boundRank(double val)
{
    return qBound(minRank(), maxRank(), val);
}

double ND::noAutoRank()
{
    return NO_AUTO_RANK;
}

double ND::boundACrit(double val)
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

double ND::boundUCrit(double val)
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
