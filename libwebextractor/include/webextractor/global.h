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

namespace Nepomuk {
    namespace WebExtractor {
	class WE 
	{
	    public:
		enum MergePolitics { 
		    Lowest = 1, 
		    Average = 2, 
		    Highest = 3, 
		    Adjust = 4, 
		    MergePolitics_MAX = 4, MergePolitics_MIN = 1 
		};

		enum LaunchPolitics {
		    All = 1, 
		    StepWise = 2, 
		    LaunchPolitics_MIN = 1, LaunchPolitics_MAX = 2
		};
	};
    }
}
#endif
