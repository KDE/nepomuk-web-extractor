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
#include <webextractor/ndco.h>

namespace NQ = Nepomuk::Query;

QString Nepomuk::WebExtractorQueries::ask_template()
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

NQ::Query Nepomuk::WebExtractorQueries::ask_query(const NQ::Term & mainTerm)
{
    return NQ::Query(mainTerm);
    /*
    return ask_template().arg(
        prefix,
        query
        );
        */
}

QString Nepomuk::WebExtractorQueries::select_template()
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

NQ::Query Nepomuk::WebExtractorQueries::select_query(const NQ::Term & mainTerm)
{
    return NQ::Query(mainTerm);
    /*
    return select_template().arg(
               prefix,
               query
           );
       */
}

#if 0
Nepomuk::Query::Query Nepomuk::WebExtractorQueries::ask_nepomuk_optimized_query(const Nepomuk::Query::Term & mainTerm, const QMap<QString, float> & assignedDataPP)
{
}
QString ask_nepomuk_optimized_query(const QString & prefix, const QString & query, const QMap<QString, float> & assignedDataPP)
{
    // Query is like
    // ask distinct ?r where {
    // { user_defined_query }
    // { ?r hasDataPP ?_Zdp1; ?_Zdp1 ?rdfs:label "Name1"; ?_Zdp1 ?ndco:version ?_Zdp1v; ?_Zdp1v < "Name1Vesrion} UNION { NOT EXIST {?r hasDataPP ?_Zdp1; _Zdp1 rdfs:label "Name1"}
    QString queryStringAnswer;

    QStringList queryLst;
    // Set prefix
    static QString _prefix = QString(
                                 "PREFIX nrl: %1 \
	 PREFIX nao: %2 \
	 PREFIX ndco: %3 "
                             ).arg(
                                 Soprano::Node::resourceToN3(Soprano::Vocabulary::NRL::nrlNamespace()),
                                 Soprano::Node::resourceToN3(Soprano::Vocabulary::NAO::naoNamespace()),
                                 Soprano::Node::resourceToN3(NW::Vocabulary::NDCO::ndcoNamespace())
                             );

    queryList << _prefix << prefix;

    // Set query begining
    static QString _header = QString("ASK { ");
    queryList << _header;

    // Add main user query
    queryList << query;

    // Add DataPP restrictions



}
#endif

QString Nepomuk::WebExtractorQueries::resourceVariableName()
{
    static QString _n = QString("r");
    return _n;
}

