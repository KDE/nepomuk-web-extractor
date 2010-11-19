/*
    This file is part of the Nepomuk KDE project.
    Copyright (C) 2010  Vishesh Handa <handa.vish@gmail.com>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along
    with this program; if not, write to the Free Software Foundation, Inc.,
    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

*/

#include "changelog.h"
#include "changelogrecord.h"

#include <QtCore/QFile>
#include <QtCore/QTextStream>
#include <QtCore/QFile>
#include <QtCore/QList>
#include <QtCore/QUrl>
#include <QtCore/QMutableListIterator>
#include <QtCore/QDir>
#include <QtCore/QSet>
#include <QtCore/QDebug>

#include <Soprano/Statement>
#include <Soprano/Model>

// trueg: why not use QSharedData and avoid all the hassle or returning references in SyncFile?
class Nepomuk::Sync::ChangeLog::Private {
public:
    // vHanda: Maybe I should use a QMap<ChangeLogRecord, QHashDummyValue> ?
    // That way the records will be ordered.
    // But then the ordering is only required while syncing ( not merging )
    QList<ChangeLogRecord> m_records;
};

Nepomuk::Sync::ChangeLog::ChangeLog()
    : d( new Nepomuk::Sync::ChangeLog::Private )
{
}

Nepomuk::Sync::ChangeLog::ChangeLog(const QList<Soprano::Statement>& st)
    : d( new Nepomuk::Sync::ChangeLog::Private )
{
    d->m_records = ChangeLogRecord::toRecordList( st );
}

Nepomuk::Sync::ChangeLog::ChangeLog( const QUrl & graphUrl, Soprano::Model * model)
    : d( new Nepomuk::Sync::ChangeLog::Private )
{
    d->m_records = ChangeLogRecord::toRecordList(graphUrl, model);
}

Nepomuk::Sync::ChangeLog::ChangeLog( const QList<QUrl> & graphUrlList, Soprano::Model * model)
    : d( new Nepomuk::Sync::ChangeLog::Private )
{
    d->m_records = ChangeLogRecord::toRecordList(graphUrlList,model);
}


Nepomuk::Sync::ChangeLog::ChangeLog(const QList<Nepomuk::Sync::ChangeLogRecord>& records)
    : d( new Nepomuk::Sync::ChangeLog::Private )
{
    d->m_records = records;
}

Nepomuk::Sync::ChangeLog::ChangeLog(const Nepomuk::Sync::ChangeLog& rhs)
    : d( new Nepomuk::Sync::ChangeLog::Private )
{
    this->operator=( rhs );
}

Nepomuk::Sync::ChangeLog::~ChangeLog()
{
    delete d;
}

Nepomuk::Sync::ChangeLog& Nepomuk::Sync::ChangeLog::operator=(const Nepomuk::Sync::ChangeLog& rhs)
{
    (*d) = (*rhs.d);
    return *this;
}


int Nepomuk::Sync::ChangeLog::size() const
{
    return d->m_records.size();
}


bool Nepomuk::Sync::ChangeLog::empty() const
{
    return d->m_records.empty();
}


void Nepomuk::Sync::ChangeLog::clear()
{
    d->m_records.clear();
}


void Nepomuk::Sync::ChangeLog::add(const Nepomuk::Sync::ChangeLogRecord& r)
{
    d->m_records.append( r );
}


QList<Nepomuk::Sync::ChangeLogRecord> Nepomuk::Sync::ChangeLog::toList() const
{
    return d->m_records;
}


// trueg: in case you change ChangeLog to useing QSharedData this method should be
//        const and return a sorted version. Then if there is only the one instance
//        of the changelog the performance is the same as no copying takes place
//        but the API would be much cleaner or at least more in sync with the Qt/KDE
//        API style
void Nepomuk::Sync::ChangeLog::sort()
{
    qSort( d->m_records );
}


// trueg: here the same as above applies.
void Nepomuk::Sync::ChangeLog::invert()
{
    QMutableListIterator<ChangeLogRecord> it( d->m_records );
    while( it.hasNext() ) {
        ChangeLogRecord & record = it.next();
        record.setAdded( !record.added() );
    }
}


// trueg: and once more as above - maybe the methods could then be named "sorted()", "inverted()", and "filtered()"
void Nepomuk::Sync::ChangeLog::filter(const QSet< QUrl >& nepomukUris)
{
    QMutableListIterator<ChangeLogRecord> it( d->m_records );
    while( it.hasNext() ) {
        const ChangeLogRecord & r = it.next();
        if( !nepomukUris.contains( r.st().subject().uri() ) ) {
            it.remove();
        }
    }
}


Nepomuk::Sync::ChangeLog& Nepomuk::Sync::ChangeLog::operator<<(const Nepomuk::Sync::ChangeLog& log)
{
    d->m_records << log.d->m_records;
    return *this;
}


Nepomuk::Sync::ChangeLog Nepomuk::Sync::ChangeLog::fromUrl(const QUrl& url)
{
    ChangeLog log;
    log.d->m_records = ChangeLogRecord::loadRecords( url );
    return log;
}


Nepomuk::Sync::ChangeLog Nepomuk::Sync::ChangeLog::fromUrl(const QUrl& url, const QDateTime& min)
{
    ChangeLog log;
    log.d->m_records = ChangeLogRecord::loadRecords( url, min );
    return log;
}


bool Nepomuk::Sync::ChangeLog::save(const QUrl& url) const
{
    return ChangeLogRecord::saveRecords( d->m_records, url );
}


// trueg: removeRecordsAfter
void Nepomuk::Sync::ChangeLog::removeRecordsGreaterThan(const QDateTime& dt)
{
    QMutableListIterator<ChangeLogRecord> it( d->m_records );
    while( it.hasNext() ) {
        it.next();

        if( it.value().dateTime() >= dt )
            it.remove();
    }
}

// trueg: removeRecordBefore
void Nepomuk::Sync::ChangeLog::removeRecordsLessThan(const QDateTime& dt)
{
    QMutableListIterator<ChangeLogRecord> it( d->m_records );
    while( it.hasNext() ) {
        it.next();

        if( dt <= it.value().dateTime() )
            it.remove();
    }
}

QSet<QUrl> Nepomuk::Sync::ChangeLog::resources() const
{
    QSet<QUrl> uniqueUris;

    foreach(const ChangeLogRecord & r, d->m_records) {
        QUrl sub = r.subject().uri();
        uniqueUris.insert(sub);

        // If the Object is a resource, then it has to be identified as well.
        const Soprano::Node obj = r.object();
        if(obj.isResource()) {
            QUrl uri = obj.uri();
            //if(uri.scheme() == QLatin1String("nepomuk"))
                uniqueUris.insert(uri);
        }
    }

    return uniqueUris;
}

QSet<QUrl> Nepomuk::Sync::ChangeLog::subjects() const
{
    QSet<QUrl> uniqueUris;

    foreach(const ChangeLogRecord & r, d->m_records) {
        QUrl sub = r.subject().uri();
        uniqueUris.insert(sub);
    }

    return uniqueUris;
}

QSet<QUrl> Nepomuk::Sync::ChangeLog::objects() const
{
    QSet<QUrl> uniqueUris;

    foreach(const ChangeLogRecord & r, d->m_records) {
        const Soprano::Node obj = r.object();
        if(obj.isResource()) {
            QUrl uri = obj.uri();
            //if(uri.scheme() == QLatin1String("nepomuk"))
                uniqueUris.insert(uri);
        }
    }

    return uniqueUris;
}

bool Nepomuk::Sync::ChangeLog::hasSubject( const QUrl & url ) const
{
    foreach(const ChangeLogRecord & r, d->m_records) {
        if ( url == r.subject().uri() )
	    return true;
    }
    return false;

}

Nepomuk::Sync::ChangeLog& Nepomuk::Sync::ChangeLog::operator<<(const Nepomuk::Sync::ChangeLogRecord& record)
{
    add( record );
    return *this;
}


QTextStream& Nepomuk::Sync::operator<<(QTextStream& ts, const Nepomuk::Sync::ChangeLog& log)
{
    foreach( const ChangeLogRecord & record, log.toList() )
        ts << record << "\n";
    return ts;
}


QString Nepomuk::Sync::ChangeLog::dateTimeFormat()
{
    return ChangeLogRecord::dateTimeFormat();
}

QDebug operator<<( QDebug debug, const Nepomuk::Sync::ChangeLog & log )
{
    foreach( const Nepomuk::Sync::ChangeLogRecord & rec, log.toList() )
        debug << rec.toString();
    return debug;
}
