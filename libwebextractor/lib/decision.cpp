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
#include <webextractor/propertiesgroup.h>
#include <webextractor/decision.h>
#include <webextractor/datapp.h>
#include <QtCore/QSharedData>
#include <QtCore/QTime>
#include <QtCore/QList>
#include <QtCore/QMultiMap>
#include <KDebug>

class Nepomuk::WebExtractor::Decision::Private : public QSharedData
{
    public:
	// TODO move rank to upper class because it it modificated frequently
	double rank;
	QSet<  PropertiesGroup > data;
	QSet<const DataPP*>  authorsData;
	unsigned int hash;
	QTime timeStamp;
	//QString pluginName;
	//QString pluginVersion;
};


double Nepomuk::WebExtractor::Decision::truncateRank( double rank )
{
    return WE::boundRank(rank);
}

double Nepomuk::WebExtractor::Decision::rank() const
{ return d->rank; }

const QString  & Nepomuk::WebExtractor::Decision::pluginVersion() const
{
   Q_ASSERT(!d->authorsData.isEmpty());
   return (*(d->authorsData.begin()))->pluginVersion();
}

const QString  & Nepomuk::WebExtractor::Decision::pluginName() const
{ 
   Q_ASSERT(!d->authorsData.isEmpty());
   return (*(d->authorsData.begin()))->pluginName();
}

Nepomuk::WebExtractor::Decision::Decision(
	const DataPP * parent
			)
{
    this->d = QSharedDataPointer<Private>( 
	    new Nepomuk::WebExtractor::Decision::Private()
	    );
    // If parent == 0 then this is invalid Decision
    if (parent)
	d->authorsData.insert(parent);

    d->timeStamp = QTime::currentTime();
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

bool Nepomuk::WebExtractor::Decision::operator==( const Decision & rhs) const
{
	return (d->data == rhs.d->data);
}

bool Nepomuk::WebExtractor::Decision::operator!=( const Decision & rhs)const
{
    return !(*this == rhs);
}

bool Nepomuk::WebExtractor::Decision::isEmpty() const
{
    return d->data.isEmpty();
}

bool Nepomuk::WebExtractor::Decision::isValid() const
{
    return !d->authorsData.isEmpty();
}

void Nepomuk::WebExtractor::Decision::setRank(double rank)
{
    rank = truncateRank(rank);

    d->rank = rank;
}

void Nepomuk::WebExtractor::Decision::addStatement(const Soprano::Statement & statement, double rank)
{
    PropertiesGroup grp;
    grp << statement;
    grp.setRank(rank);
    addGroup(grp);
}

void Nepomuk::WebExtractor::Decision::addGroup( const PropertiesGroup & grp)
{
    //rank = Private::truncateRank(rank);

    // Check that none of this statemnt's exist in model.
    // Those that's exist - ignore
    
    // Add statements
    //d->data.insert(rank,statements);
    d->data << grp;

    // Increase hash
    d->hash ^= qHash(grp);
}

void Nepomuk::WebExtractor::Decision::apply() const
{
    kDebug() << "Write statements to storage";
    foreach ( const PropertiesGroup & lst, d->data )
    {
	foreach( const Soprano::Statement  & st, lst.data() )
	{
	    kDebug() << st;
	}
    }
}

void Nepomuk::WebExtractor::Decision::addToUserDiscretion()
{
    kDebug() << "Write Decision to user discretion list";
    foreach ( const PropertiesGroup  &  lst, d->data )
    {
	foreach( const Soprano::Statement  & st, lst.data() )
	{
	    kDebug() << st;
	}
    }
}

void Nepomuk::WebExtractor::Decision::addAuthor(const DataPP * author)
{
    d->authorsData.insert(author);
}

unsigned int Nepomuk::WebExtractor::qHash( const Nepomuk::WebExtractor::Decision & des)
{
    return des.d->hash;
}
