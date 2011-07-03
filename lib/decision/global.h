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

#ifndef NEPOMUK_DECISION_GLOBALS_H
#define NEPOMUK_DECISION_GLOBALS_H

#include "decision_export.h"


namespace Nepomuk {
    namespace Decision {

        const int NO_AUTO_RANK = -1;
        
        enum MergePolitics {
            Lowest = 1,// Result decision has rank == min(r1,r2)
            Average = 2, // Result decision has rank == (r1+r2)/2
            Highest = 3,  // Result decision has rank == max(r1,r2)
            Adjust = 4, // Result decision has rank r1**(1-r2)
            MergePolitics_MAX = 4, MergePolitics_MIN = 1
        };

        DECISION_EXPORT double minACrit();
        DECISION_EXPORT double maxACrit();

        DECISION_EXPORT double minUCrit();
        DECISION_EXPORT double maxUCrit();

        DECISION_EXPORT double minRank();
        DECISION_EXPORT double maxRank();

        /*! \brief This function return the special value.
         * Decisions with this rank can not be applied automatically
         */
        DECISION_EXPORT double noAutoRank();

        DECISION_EXPORT double boundRank(double);
        DECISION_EXPORT double boundACrit(double val);
        DECISION_EXPORT double boundUCrit(double val);
    }
}

#endif
