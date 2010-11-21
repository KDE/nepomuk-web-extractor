/*
   Copyright (C) 2010 by Sebastian Trueg <trueg@kde.org>

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

#include "scribodatappreply.h"
#include "scribodatapp.h"

#include <scribo/textmatcher.h>
#include <scribo/textmatch.h>
#include <scribo/textoccurrence.h>
#include <scribo/entity.h>

#include "decisionfactory.h"
#include "decisioncreator.h"

#include <Nepomuk/Resource>
#include <Nepomuk/Variant>
#include <KDebug>
#include <KLocale>

#include <Nepomuk/Query/ComparisonTerm>
#include <Nepomuk/Query/LiteralTerm>
#include <Nepomuk/Query/AndTerm>
#include <Nepomuk/Query/OrTerm>
#include <Nepomuk/Query/ResourceTypeTerm>
#include <Nepomuk/Query/Query>

#include <Nepomuk/Vocabulary/PIMO>
#include <Soprano/Vocabulary/NAO>
#include <Soprano/QueryResultIterator>


Nepomuk::ScriboReply::ScriboReply( ScriboDataPP* parent,
                               const WebExtractor::DecisionFactory* factory,
                               const Nepomuk::Resource& res,
                               const QString& text )
    : SimpleDataPPReply(parent, factory, res)
{
    Scribo::TextMatcher* matcher = new Scribo::TextMatcher( this );
    connect( matcher, SIGNAL( newMatch( Scribo::TextMatch ) ),
             this, SLOT( slotNewMatch( Scribo::TextMatch ) ) );
    connect( matcher, SIGNAL( finished() ),
             this, SLOT( slotMatchingFinished() ) );
    matcher->getPossibleMatches( text );
}


bool Nepomuk::ScriboReply::isValid() const
{
    return true;
}


void Nepomuk::ScriboReply::abort()
{
    kError() <<  "ATTENTION: Not implemented";
}


void Nepomuk::ScriboReply::slotNewMatch( const Scribo::TextMatch& match )
{
    kDebug();
    if ( match.isEntity() ) {
        Scribo::Entity entity = match.toEntity();

        if ( entity.localResource().isValid() ) {
            createAnnotationForResource( entity, entity.localResource(), 1.0 );
        }
        else {
            // TODO: use the entity type

            Query::Query query(
                Query::AndTerm(
                    Query::ComparisonTerm(
                        Soprano::Vocabulary::NAO::prefLabel(),
                        Query::LiteralTerm( entity.label() )
                        ),
                    Query::OrTerm(
                        Query::ResourceTypeTerm( Soprano::Vocabulary::NAO::Tag() ),
                        Query::ResourceTypeTerm( Vocabulary::PIMO::Thing() )
                        )
                    )
                );

            // although this might slow down queries considerably it will give us better results
            query.setFullTextScoringEnabled( true );

            Soprano::QueryResultIterator it
                = ResourceManager::instance()->mainModel()->executeQuery( query.toSparqlQuery(),
                                                                          Soprano::Query::QueryLanguageSparql );
            while ( it.next() ) {
                Nepomuk::Resource res = Resource::fromResourceUri( it[0].uri() );
                double score = it[1].literal().toDouble();

                kDebug() << res.resourceUri() << score;
                createAnnotationForResource( entity, res, score );
            }
        }
    }
}


void Nepomuk::ScriboReply::createAnnotationForResource( const Scribo::Entity& entity, const Nepomuk::Resource& res, double score )
{
    const QUrl type = res.resourceType();

    // create a decision
    WebExtractor::DecisionCreator d = newDecision();

    Resource resourceProxy = d.proxyResource( resource() );
    resourceProxy.addProperty( type == Soprano::Vocabulary::NAO::Tag()
                               ? Soprano::Vocabulary::NAO::hasTag()
                               : Nepomuk::Vocabulary::PIMO::isRelated(),
                               d.proxyResource( res ) );

    // calculate the probability of the match the dumb way
    d.setRank( score );

    // set a user-readable description
    d.setDescription(i18n("%1 mentions '%2'",
                          resource().genericLabel(),
                          entity.label()));

    // add the decision to the pool of applicable ones
    addDecision( d );
}


void Nepomuk::ScriboReply::slotMatchingFinished()
{
    kDebug();
    finish();
}

#include "scribodatappreply.moc"
