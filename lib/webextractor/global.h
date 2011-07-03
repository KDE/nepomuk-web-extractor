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

#ifndef _WEBEXTRCT_GLOBALS
#define _WEBEXTRCT_GLOBALS

#include "webextractor_export.h"
#include <Soprano/Model>

namespace Nepomuk
{
    namespace WebExtractor
    {

        enum LaunchPolitics {
            All = 1,
            StepWise = 2,
            LaunchPolitics_MIN = 1, LaunchPolitics_MAX = 2
        };


        WEBEXTRACTOR_EXPORT double minScaleCoff();
        WEBEXTRACTOR_EXPORT double maxScaleCoff();
        WEBEXTRACTOR_EXPORT double boundScaleCoff(double);


        WEBEXTRACTOR_EXPORT int defaultExecutiveTimeout();

    }
}
#endif
