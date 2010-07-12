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


#ifndef __WEBEXTRACTOR_EXPORT_H_
#define __WEBEXTRACTOR_EXPORT_H_

/* needed for KDE_EXPORT and KDE_IMPORT macros */
#include <kdemacros.h>

#ifndef WEBEXTRACTOR_EXPORT
# if defined(MAKE_WEBEXTRACTOR_LIB)
   /* We are building this library */
#  define WEBEXTRACTOR_EXPORT KDE_EXPORT
# else
   /* We are using this library */
#  define WEBEXTRACTOR_EXPORT KDE_IMPORT
# endif
#endif


#endif
