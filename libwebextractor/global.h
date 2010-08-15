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

#ifndef _WEBEXTRCT_GLOBALS
#define _WEBEXTRCT_GLOBALS

#include "webextractor_export.h"
#include <Soprano/Model>

namespace Nepomuk
{
    namespace WebExtractor
    {
        enum MergePolitics {
            Lowest = 1,// Result decision has rank == min(r1,r2)
            Average = 2, // Result decision has rank == (r1+r2)/2
            Highest = 3,  // Result decision has rank == max(r1,r2)
            Adjust = 4, // Result decision has rank r1**(1-r2)
            MergePolitics_MAX = 4, MergePolitics_MIN = 1
        };

        enum LaunchPolitics {
            All = 1,
            StepWise = 2,
            LaunchPolitics_MIN = 1, LaunchPolitics_MAX = 2
        };

        WEBEXTRACTOR_EXPORT double minACrit();
        WEBEXTRACTOR_EXPORT double maxACrit();

        WEBEXTRACTOR_EXPORT double minUCrit();
        WEBEXTRACTOR_EXPORT double maxUCrit();

        WEBEXTRACTOR_EXPORT double minRank();
        WEBEXTRACTOR_EXPORT double maxRank();
        WEBEXTRACTOR_EXPORT double boundRank(double);

        WEBEXTRACTOR_EXPORT double minScaleCoff();
        WEBEXTRACTOR_EXPORT double maxScaleCoff();
        WEBEXTRACTOR_EXPORT double boundScaleCoff(double);

        WEBEXTRACTOR_EXPORT double boundACrit(double val);
        WEBEXTRACTOR_EXPORT double boundUCrit(double val);

    }
}
#endif
