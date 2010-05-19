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
#include <webextractor/decision.h>
#include <QtCore/QSharedData>
#include <QtCore/QList>
#include <QtCore/QMultiMap>
#include <KDebug>

class Nepomuk::WebExtractor::Decision::Private : public QSharedData
{
    public:
	double rank;
	QMultiMap< double, QList<Soprano::Statement> > data;
	static inline double truncateRank(double );
};


double Nepomuk::WebExtractor::Decision::Private::truncateRank( double rank )
{
    if (rank >= 1)
	rank = 0.99;
    if (rank < 0 )
	rank = 0;
    return rank;
}

double Nepomuk::WebExtractor::Decision::rank() const
{ return d->rank; }

Nepomuk::WebExtractor::Decision::Decision()
{
    this->d = QSharedDataPointer<Private>( 
	    new Nepomuk::WebExtractor::Decision::Private()
	    );
}

Nepomuk::WebExtractor::Decision::~Decision()
{;}

Nepomuk::WebExtractor::Decision::Decision( const Decision & rhs)
{
    d = rhs.d;
}

const Nepomuk::WebExtractor::Decision & Nepomuk::WebExtractor::Decision::operator=( const Decision & rhs)
{
    d = rhs.d;
    return *this;
}

void Nepomuk::WebExtractor::Decision::setRank(double rank)
{
    rank = Private::truncateRank(rank);

    d->rank = rank;
}

void Nepomuk::WebExtractor::Decision::addStatement(const Soprano::Statement & statement, double rank)
{
    rank = Private::truncateRank(rank);
    QList<Soprano::Statement> lst;
    lst.push_back(statement);
    d->data.insert(rank,lst);
}

void Nepomuk::WebExtractor::Decision::addStatementGroup( const QList<Soprano::Statement> & statements, double rank)
{
    rank = Private::truncateRank(rank);
    d->data.insert(rank,statements);
}

void Nepomuk::WebExtractor::Decision::apply()
{
    kDebug() << "Write statements to storage";
    foreach ( const QList< Soprano::Statement > & lst, d->data )
    {
	foreach( const Soprano::Statement  & st, lst )
	{
	    kDebug() << st;
	}
    }
}

void Nepomuk::WebExtractor::Decision::addToUserDiscretion()
{
    kDebug() << "Write Decision to user discretion list";
    foreach ( const QList< Soprano::Statement >  &  lst, d->data )
    {
	foreach( const Soprano::Statement  & st, lst )
	{
	    kDebug() << st;
	}
    }
}
