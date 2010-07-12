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

namespace Nepomuk
{
    namespace WebExtractor
    {
        class WEBEXTRACTOR_EXPORT WE
        {
            public:
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

                static double minACrit();
                static double maxACrit();

                static double minUCrit();
                static double maxUCrit();

                static double minRank();
                static double maxRank();
                static double boundRank(double);

                static double minScaleCoff();
                static double maxScaleCoff();
                static double boundScaleCoff(double);

                static double boundACrit(double val);
                static double boundUCrit(double val);

        };
    }
}
#endif
