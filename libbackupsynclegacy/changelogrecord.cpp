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

#include "changelogrecord.h"

#include <QtCore/QRegExp>
#include <QtCore/QFile>

#include <Soprano/Node>
#include <Soprano/PluginManager>
#include <Soprano/Parser>
#include <Soprano/Serializer>
#include <Soprano/Util/SimpleStatementIterator>

#include <KDebug>

#include <QtCore/QSharedData>

class Nepomuk::Sync::ChangeLogRecord::Private : public QSharedData {
public:
    QDateTime dateTime;
    bool added;
    Soprano::Statement st;

    const static Soprano::Parser * parser;
    const static Soprano::Serializer * serializer;

    const static QString s_dateTimeFormat;
};

const QString Nepomuk::Sync::ChangeLogRecord::Private::s_dateTimeFormat = QString::fromLatin1("yyyy-MM-ddThh:mm:ss.zzz");

//
// Create the serializer and parser
//
const Soprano::Parser* Nepomuk::Sync::ChangeLogRecord::Private::parser = Soprano::PluginManager::instance()->discoverParserForSerialization( Soprano::SerializationNQuads );

const Soprano::Serializer* Nepomuk::Sync::ChangeLogRecord::Private::serializer = Soprano::PluginManager::instance()->discoverSerializerForSerialization( Soprano::SerializationNQuads );


Nepomuk::Sync::ChangeLogRecord::ChangeLogRecord()
    : d( new Nepomuk::Sync::ChangeLogRecord::Private )
{
    //Nothing to do
}

Nepomuk::Sync::ChangeLogRecord::~ChangeLogRecord()
{
}


Nepomuk::Sync::ChangeLogRecord::ChangeLogRecord(const QDateTime& dt, bool add, const Soprano::Statement& statement)
    : d( new Nepomuk::Sync::ChangeLogRecord::Private )
{
    d->dateTime = dt;
    d->added = add;
    d->st = statement;
}


Nepomuk::Sync::ChangeLogRecord::ChangeLogRecord(const Soprano::Statement& statement)
    : d( new Nepomuk::Sync::ChangeLogRecord::Private )
{
    d->dateTime = QDateTime::currentDateTime();
    d->added = true;
    d->st = statement;
}


Nepomuk::Sync::ChangeLogRecord::ChangeLogRecord(const Nepomuk::Sync::ChangeLogRecord& rhs)
    : d( rhs.d )
{
}


Nepomuk::Sync::ChangeLogRecord::ChangeLogRecord( QString& string )
    : d( new Nepomuk::Sync::ChangeLogRecord::Private )
{
    QTextStream ts( &string );

    QString dt, ad;
    ts >> dt >> ad ;

    d->dateTime = QDateTime::fromString(dt, Private::s_dateTimeFormat);
    d->added = (ad == "+") ? true : false;

    QList<Soprano::Statement> statements = Private::parser->parseStream( ts, QUrl(), Soprano::SerializationNQuads ).allElements();

    Q_ASSERT( statements.size() == 1 );
    d->st = statements.first();
}


Nepomuk::Sync::ChangeLogRecord& Nepomuk::Sync::ChangeLogRecord::operator=(const Nepomuk::Sync::ChangeLogRecord& rhs)
{
    this->d = rhs.d;
    return *this;
}


QString Nepomuk::Sync::ChangeLogRecord::toString() const
{
    QString s;
    s += d->dateTime.toString( Private::s_dateTimeFormat ) + ' ';
    s += d->added ? '+' : '-';
    s += " ";

    Soprano::Util::SimpleStatementIterator it( QList<Soprano::Statement>() << d->st );

    QString statement;
    QTextStream ts( &statement );

    Private::serializer->serialize( it, ts, Soprano::SerializationNQuads);

    return s + statement;
}


bool Nepomuk::Sync::ChangeLogRecord::operator < (const Nepomuk::Sync::ChangeLogRecord & rhs) const
{
    return d->dateTime < rhs.d->dateTime;
}

bool Nepomuk::Sync::ChangeLogRecord::operator > (const Nepomuk::Sync::ChangeLogRecord & rhs) const
{
    return d->dateTime > rhs.d->dateTime;
}

bool Nepomuk::Sync::ChangeLogRecord::operator==(const Nepomuk::Sync::ChangeLogRecord& rhs) const
{
    return d->dateTime == rhs.d->dateTime && d->st == rhs.d->st && d->added == rhs.d->added;
}


//static
QList<Nepomuk::Sync::ChangeLogRecord> Nepomuk::Sync::ChangeLogRecord::toRecordList(const QList<Soprano::Statement>& stList)
{
    QList<ChangeLogRecord> list;
    foreach( const Soprano::Statement & st, stList ) {
        list << ChangeLogRecord( st );
    }
    return list;
}


//static
QList< Nepomuk::Sync::ChangeLogRecord> Nepomuk::Sync::ChangeLogRecord::toRecordList( const QUrl& contextUrl, Soprano::Model* model )
{
    QList<ChangeLogRecord> list;
    Soprano::StatementIterator it = model->listStatementsInContext(contextUrl);
    while( it.next() ) {
        list << ChangeLogRecord( QDateTime::currentDateTime(), true, it.current());
    }
    return list;
}


//static
QList< Nepomuk::Sync::ChangeLogRecord> Nepomuk::Sync::ChangeLogRecord::toRecordList( const QList< QUrl >& contextUrlList, Soprano::Model* model )
{
    QList<ChangeLogRecord> list;
    foreach( const QUrl & contextUrl, contextUrlList) {
        Soprano::StatementIterator it = model->listStatementsInContext(contextUrl);
        while( it.next() ) {
            list << ChangeLogRecord( QDateTime::currentDateTime(), true, it.current());
        }
    }
    return list;
}

// static
bool Nepomuk::Sync::ChangeLogRecord::saveRecords(const QList<Nepomuk::Sync::ChangeLogRecord>& records, const QUrl & url)
{
    if( records.empty() )
        return false;
    
    QFile file( url.toLocalFile() );
    if( !file.open( QIODevice::ReadWrite | QIODevice::Append | QIODevice::Text ) ) {
        kWarning() << "File couldn't be opened for saving : " << url;
        return false;
    }
    
    QTextStream out( &file );
    
    foreach( const ChangeLogRecord& r, records ) {
        out << r.toString();
    }
    return true;
}

// static
QList< Nepomuk::Sync::ChangeLogRecord > Nepomuk::Sync::ChangeLogRecord::loadRecords(const QUrl& url, const QDateTime & min)
{
    QFile file( url.path() );
    if ( !file.open( QIODevice::ReadOnly | QIODevice::Text) ) {
        kWarning() << "File could not be opened : " << url.path();
        return QList<ChangeLogRecord>();
    }
    
    QTextStream in( &file );

    QList<ChangeLogRecord> records;
    while( !in.atEnd() )
    {
        QString line = in.readLine();
        ChangeLogRecord r( line );
        
        if( min <= r.dateTime() ) {
            records.push_back( r );
        }
    }
    return records;
}


// static
QList< Nepomuk::Sync::ChangeLogRecord > Nepomuk::Sync::ChangeLogRecord::loadRecords(const QUrl& url)
{
    QDateTime min;
    min.setTime_t( 0 );
    
    return loadRecords( url, min );
}

bool Nepomuk::Sync::ChangeLogRecord::added() const
{
    return d->added;
}

QDateTime Nepomuk::Sync::ChangeLogRecord::dateTime() const
{
    return d->dateTime;
}

void Nepomuk::Sync::ChangeLogRecord::setAdded(bool add)
{
    d->added = add;
}

void Nepomuk::Sync::ChangeLogRecord::setRemoved()
{
    d->added = false;
}

void Nepomuk::Sync::ChangeLogRecord::setDateTime(const QDateTime& dt)
{
    d->dateTime = dt;
}

const Soprano::Statement& Nepomuk::Sync::ChangeLogRecord::st() const
{
    return d->st;
}

void Nepomuk::Sync::ChangeLogRecord::setSubject(const Soprano::Node& subject)
{
    d->st.setSubject( subject );
}


void Nepomuk::Sync::ChangeLogRecord::setPredicate(const Soprano::Node& predicate)
{
    d->st.setPredicate( predicate );
}

void Nepomuk::Sync::ChangeLogRecord::setObject(const Soprano::Node& object)
{
    d->st.setObject( object );
}


void Nepomuk::Sync::ChangeLogRecord::setContext(const Soprano::Node& context)
{
    d->st.setContext( context );
}

Soprano::Node Nepomuk::Sync::ChangeLogRecord::subject() const
{
    return d->st.subject();
}

Soprano::Node Nepomuk::Sync::ChangeLogRecord::predicate() const
{
    return d->st.predicate();
}

Soprano::Node Nepomuk::Sync::ChangeLogRecord::object() const
{
    return d->st.object();
}

QTextStream& Nepomuk::Sync::operator<<(QTextStream& ts, const Nepomuk::Sync::ChangeLogRecord& record)
{
    return ts << record.toString();
}

QString Nepomuk::Sync::ChangeLogRecord::dateTimeFormat()
{
    return Private::s_dateTimeFormat;
}
