/*
    This file is part of the Nepomuk KDE project.
    Copyright (C) 2010  Vishesh Handa <handa.vish@gmail.com>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/


#include "simpleresource.h"
#include <nepomuk/nfo.h>
#include <nepomuk/nie.h>

#include <Soprano/Vocabulary/RDF>
#include <Soprano/Statement>

#include <QtCore/QSharedData>

class Nepomuk::Sync::SimpleResource::Private : public QSharedData {
public:
    QUrl uri;
};


Nepomuk::Sync::SimpleResource::SimpleResource()
    : d( new Nepomuk::Sync::SimpleResource::Private )
{
}

Nepomuk::Sync::SimpleResource::SimpleResource(const Nepomuk::Sync::SimpleResource& rhs)
    : QMultiHash< QUrl, Soprano::Node >(rhs),
      d( rhs.d )
{
}

Nepomuk::Sync::SimpleResource::~SimpleResource()
{
}

Nepomuk::Sync::SimpleResource& Nepomuk::Sync::SimpleResource::operator=(const Nepomuk::Sync::SimpleResource& rhs)
{
    d = rhs.d;
    return *this;
}

QList< Soprano::Statement > Nepomuk::Sync::SimpleResource::toStatementList() const
{
    QList<Soprano::Statement> list;
    const QList<QUrl> & keys = uniqueKeys();
    foreach( const QUrl & key, keys ) {
        Soprano::Statement st;
        st.setSubject( Soprano::Node( d->uri ) );
        st.setPredicate( Soprano::Node( key ) );

        const QList<Soprano::Node>& objects = values( key );
        foreach( const Soprano::Node & node, objects ) {
            st.setObject( node );
            list.append( st );
        }
    }
    return list;
}


bool Nepomuk::Sync::SimpleResource::isFolder() const
{
    return values( Soprano::Vocabulary::RDF::type() ).contains( Soprano::Node( Nepomuk::Vocabulary::NFO::Folder() ) );
}


bool Nepomuk::Sync::SimpleResource::isFileDataObject() const
{
    return values( Soprano::Vocabulary::RDF::type() ).contains( Soprano::Node( Nepomuk::Vocabulary::NFO::FileDataObject() ) );
}


QUrl Nepomuk::Sync::SimpleResource::nieUrl() const
{
    const QHash<QUrl, Soprano::Node>::const_iterator it = constFind( Nepomuk::Vocabulary::NIE::url() );
    if( it == constEnd() )
        return QUrl();
    else
        return it.value().uri();
}


void Nepomuk::Sync::SimpleResource::setUri(const QUrl& newUri)
{
    d->uri = newUri;
}

QUrl Nepomuk::Sync::SimpleResource::uri() const
{
    return d->uri;
}

QList< Soprano::Node > Nepomuk::Sync::SimpleResource::property(const QUrl& url) const
{
    return values(url);
}
