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
#include "webexqueries.h"
#include <Soprano/Model>
#include <Soprano/Vocabulary/NAO>
#include <Soprano/Vocabulary/NRL>
#include <Soprano/Node>

const QString & Nepomuk::WebExtractorQueries::ask_template() 
{
    static QString _templ = QString(
	"PREFIX nrl: %1 \
	 PREFIX nao: %2 \
	 %3 \
	 ASK {%4}"
	 ).arg(
	    Soprano::Node::resourceToN3(Soprano::Vocabulary::NRL::nrlNamespace()),
	    Soprano::Node::resourceToN3(Soprano::Vocabulary::NAO::naoNamespace())
	    );
    return _templ;
;
};

QString Nepomuk::WebExtractorQueries::ask_query(const QString & prefix, const QString & query)
{
    return ask_template().arg(
		prefix,
		query
		);
}

const QString & Nepomuk::WebExtractorQueries::select_template() 
{
    static QString _templ = QString(
	"PREFIX nrl: %1 \
	 PREFIX nao: %2 \
	 %3 \
	 SELECT ?r WHERE {%4}"
	 ).arg(
	    Soprano::Node::resourceToN3(Soprano::Vocabulary::NRL::nrlNamespace()),
	    Soprano::Node::resourceToN3(Soprano::Vocabulary::NAO::naoNamespace())
	    );
    return _templ;
}

QString Nepomuk::WebExtractorQueries::select_query(const QString & prefix, const QString & query)
{
    return select_template().arg(
		prefix,
		query
		);
}

const QString & Nepomuk::WebExtractorQueries::resourceVariableName()
{
    static QString _n = QString("r");
    return _n;
}

