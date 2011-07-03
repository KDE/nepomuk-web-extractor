/*
   Copyright (C) 2010 by Serebriyskiy Artem <v.for.vandal@gmail.com>

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

#ifndef _NEPOMUK_WEBEXTRCT_QUERIES_H_
#define _NEPOMUK_WEBEXTRCT_QUERIES_H_
#include <QtCore/QString>
#include <Nepomuk/Query/Query>
#include <Nepomuk/Query/Term>


namespace Nepomuk
{
    // Provide static members
    class WebExtractorQueries
    {
        public:
            static Nepomuk::Query::Query ask_query(const Nepomuk::Query::Term & mainTerm);
            static Nepomuk::Query::Query ask_nepomuk_optimized_query(const Nepomuk::Query::Term & mainTerm, const QMap<QString, float> & assignedDataPP);
            static Nepomuk::Query::Query select_query(const Nepomuk::Query::Term & mainTerm);
            static Nepomuk::Query::Query select_nepomuk_optimized_query(const Nepomuk::Query::Term & mainTerm, const QMap<QString, float> & assignedDataPP);
            static QString  resourceVariableName();
        private:
            static QString  ask_template() ;
            static QString  select_template() ;
    };
}

#endif
