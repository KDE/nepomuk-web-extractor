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

#include "executive.h"
#include <Nepomuk/ResourceManager>
#include <Nepomuk/Variant>
#include <Nepomuk/Query/Query>
#include <Nepomuk/Query/ComparisonTerm>
#include <Nepomuk/Query/Term>
#include <Nepomuk/Query/NegationTerm>
#include <Nepomuk/Query/LiteralTerm>
#include <Nepomuk/Query/AndTerm>
#include <Soprano/QueryResultIterator>
#include <Soprano/Vocabulary/NAO>
#include <Soprano/Vocabulary/RDFS>
#include <Soprano/Vocabulary/RDF>
#include <Soprano/Model>
#include <Soprano/Node>
#include <Soprano/BindingSet>
#include "ndco.h"

#include <QtDebug>


namespace NQ = Nepomuk::Query;
namespace NW = Nepomuk::WebExtractor;

namespace {
    class FilterRecord
    {
        public:
            QUrl typeUrl;
            NW::Executive::FilterClassType matchType;
            bool allowed;
            /*
            static QString* templateTable() 
            {
                static bool init = false;
                static QString[NW::Executive::Match_MAX+1] table; 
                if (!init) {
                    init = true;
                    table[NW::Executive::ExactMatch] = QString("%1 %2 %3 .").arg("%1",
                           Soprano::resourceToN3(Soprano::Vocabulary::RDF::type())
                           ); 
                    table[NW::Executive::ExactMatch] = QString("%1 %2 ? .").arg("%1",
                           Soprano::resourceToN3(Soprano::Vocabulary::RDFS::?())
                           ); 
                }
                return table;
            }
            */
#if 0
            QString queryString(const QUrl & resourceUrl, int uid)
            {
                static QString exactMatchString = QString("{%1 %2 %3 }").arg("%1",
                           Soprano::Node::resourceToN3(Soprano::Vocabulary::RDF::type())
                           ); 

                static QString subclassMatchString = QString("{%1 %2 %3. %3  %4 %5 }").arg(
                        /*%1*/ "%1",
                         /*%2*/Soprano::Node::resourceToN3(Soprano::Vocabulary::RDF::type()),
                         /*%3 - template for blank node*/"%2",
                         /*%4*/Soprano::Node::resourceToN3(Soprano::Vocabulary::RDFS::subClassOf()),
                         /*%5*/"%3"
                        );

                qDebug() << "Subclass match string" << subclassMatchString;

                QString stage1;
                switch( this->matchType )
                {
                    case NW::Executive::ExactMatch: {
                               stage1 = exactMatchString.arg(
                                      Soprano::Node::resourceToN3(resourceUrl),
                                      Soprano::Node::resourceToN3(typeUrl)
                                      );
                               break;
                                                    } 
                    case NW::Executive::SubclassMatch: {
                               stage1 = subclassMatchString.arg(
                                      Soprano::Node::resourceToN3(resourceUrl),
                                      QString("_:a%1").arg(QString::number(uid)),
                                      Soprano::Node::resourceToN3(typeUrl)
                                      );
                               break;
                                                    } 
                    default : { qCritical() << "Unknow type match type in filter"; }
                }

                qDebug() << "Query after stage1 " << stage1;

                static QString notExistsString = QString(" NOT EXISTS { %1 } ");
                if ( this->allowed ) {
                    return stage1;
                }
                else {
                    return notExistsString.arg(stage1);
                }
                
                /* Function end */
            }
#endif
            static QString queryString(NW::Executive::FilterClassType matchType, const QUrl & typeUrl ) 
            {
                static QString exactMatchString = QString(" { ?t a %1 }" );
                static QString subclassMatchString = QString(" { ?s a %1 } ");
                QString stage1;
                switch( matchType )
                {

                    case NW::Executive::ExactMatch: {
                               stage1 = exactMatchString;
                               break;
                                                    } 
                    case NW::Executive::SubclassMatch: {
                               stage1 = subclassMatchString;                               break;
                                                    } 
                                                              
                    default : { qCritical() << "Unhandled match type passed to query constructor";return QString(); }
                }


                return stage1.arg(
                        Soprano::Node::resourceToN3(typeUrl)
                        );
            }
    /* Class end */
    };


    QString buildFilterQuery(const QUrl & resourceUri, QStringList & queryExactList, QStringList & querySubclassList )
    {
        static QString subclassPrependString = QString(" { ?t %1 ?s { ").arg(
                Soprano::Node::resourceToN3(Soprano::Vocabulary::RDFS::subClassOf()));
        static QString exactPrefixString = QString(" ASK { %1 %2 ?t. { ").arg("%1",
                Soprano::Node::resourceToN3(Soprano::Vocabulary::RDF::type()));
        static QString postfixString = QString( " } } ");
        static QString unionString = QString( "UNION" );

        // First create a exact match string
        if ( queryExactList.size() == 0 )
            queryExactList << "{}";
        else {
            Q_ASSERT(queryExactList.size() % 2 == 0 );
            // Remove last union string
            queryExactList.removeLast();
        }

        queryExactList.prepend(exactPrefixString.arg(Soprano::Node::resourceToN3(resourceUri)));
        // Call a query and clear
        if ( querySubclassList.size() ) {
            Q_ASSERT(querySubclassList.size() % 2 == 0 );
            if (  querySubclassList.size() ) {
                querySubclassList.removeLast();
            }
            queryExactList << unionString;
            queryExactList << subclassPrependString;
            queryExactList << querySubclassList ;
            queryExactList << postfixString;
        }
        queryExactList << postfixString;

        // Make a query
        QString query = queryExactList.join(QString());
        qDebug() << "Filter query: " <<  query;

        return query;
    }
/* Namespace end */
}

class Nepomuk::WebExtractor::Executive::Private
{
    public:
        int m_version;
        QString m_name;
        QList<FilterRecord> typeFilters;
        bool defaultTypeRule;
};

Nepomuk::WebExtractor::Executive::~Executive()
{
    ;
}

Nepomuk::WebExtractor::Executive::Executive(int  dataPPVersion)
{
    d = new Private();
    d->m_version = dataPPVersion;
}

void Nepomuk::WebExtractor::Executive::setName(const QString & name)
{
    d->m_name = name;
}
QString Nepomuk::WebExtractor::Executive::name() const
{
    return d->m_name;
}

int Nepomuk::WebExtractor::Executive::version() const
{
    return d->m_version;
}

/* ======= Decisions section ========= */

NW::ExecutiveReply * NW::Executive::decisions( const Decision::DecisionFactory * factory, const Nepomuk::Resource & res )
{
    if ( ! canProcess( res ) )
        return 0;

    return this->requestDecisions(factory,res);
}

/* ======= Filtering section ========= */
void NW::Executive::addTypeFilter( const QUrl & typeUrl, FilterClassType matchType, bool allowed)
{
    // Create new record
    FilterRecord record;
    record.typeUrl = typeUrl;
    record.matchType = matchType;
    record.allowed = allowed;

    // Add record to the list
    d->typeFilters << record;
}

bool NW::Executive::canProcess( const Nepomuk::Resource & res) const
{
    // If no filter set up, then no filter is necessary
    if ( d->typeFilters.isEmpty() )
        return true;

    // Create a query
    /*
    for( int i = 0; i < d->typeFilters; i++)
    {
    }*/
    static QString unionString = QString( "UNION" );
    QStringList queryExactList;
    QStringList querySubclassList;
    bool currentType = d->typeFilters[0].allowed;

            
    /* General idea - unite consiquent records with same .allowed field into
     * one group, and  when prepare to switch to another group - create a query
     * from accumulated strings, ask it and determine the result. 
     * Reaching the end of filter list is treated as switching to another group
     */
    foreach( const FilterRecord & record, d->typeFilters)
    {
        if ( record.allowed != currentType ) {

            // After this function call, queryExactList will change!
            QString query = buildFilterQuery(res.resourceUri(), queryExactList, querySubclassList);


            Soprano::QueryResultIterator it = res.manager()->mainModel()->executeQuery(query, Soprano::Query::QueryLanguageSparql );
            if ( it.next() ) {
                // Has answer
                if ( it.boolValue() ) {
                    return currentType;
                }
            }
            else {
                // Wow. Ask query doesn't return an answer. May be it is incorrect ?
                qCritical() << "Filter query is incorrect. Report a BUG please";
                return false;
            }

            // Clear
            queryExactList.clear();
            querySubclassList.clear();

            // Switch current type
            currentType = record.allowed;
        }

        // Accumulate to the group
        if ( record.matchType == ExactMatch or record.matchType == SubclassOrExactMatch ) {
            queryExactList << FilterRecord::queryString(ExactMatch, record.typeUrl) << unionString;
        }

        if ( record.matchType == SubclassOrExactMatch or record.matchType == SubclassMatch ) {
            querySubclassList << FilterRecord::queryString(SubclassMatch, record.typeUrl) << unionString;
        }
    }
    // Build for the last group
    QString query = buildFilterQuery(res.resourceUri(), queryExactList, querySubclassList);


    Soprano::QueryResultIterator it = res.manager()->mainModel()->executeQuery(query, Soprano::Query::QueryLanguageSparql );
    if ( it.isBool() ) {
        // Has answer
        if ( it.boolValue() ) {
            return currentType;
        }
        else {
            // return default rule
            return d->defaultTypeRule;
        }
    }
    else {
        // Wow. Ask query doesn't return an answer. May be it is incorrect ?
        qCritical() << "Filter query is incorrect. Report a BUG please";
        return false;
    }


}

void NW::Executive::setDefaultFilter( bool allowed )
{
    d->defaultTypeRule = allowed;
}
