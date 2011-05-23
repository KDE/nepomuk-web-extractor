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

#include "syncfile.h"
#include "changelog.h"
#include "identificationset.h"

#include <QtCore/QFile>
#include <QtCore/QMutableListIterator>

#include <KTar>
#include <KTempDir>
#include <KDebug>
#include <Nepomuk/ResourceManager>


class Nepomuk::Sync::SyncFile::Private {
public :
    ChangeLog m_changeLog;
    IdentificationSet m_identificationSet;
};


Nepomuk::Sync::SyncFile::SyncFile()
    : d( new Nepomuk::Sync::SyncFile::Private() )
{
}


Nepomuk::Sync::SyncFile::SyncFile(const Nepomuk::Sync::SyncFile& rhs)
    : d( new Nepomuk::Sync::SyncFile::Private() )
{
    this->operator=( rhs );
}


Nepomuk::Sync::SyncFile::~SyncFile()
{
    delete d;
}


Nepomuk::Sync::SyncFile& Nepomuk::Sync::SyncFile::operator=(const Nepomuk::Sync::SyncFile& rhs)
{
    // trueg: why not use QSharedData if the data can be copied this easily anyway?
    // however, in that case the non-const methods below returning refs will call d->detach
    (*d) = (*rhs.d);
    return *this;
}


Nepomuk::Sync::SyncFile::SyncFile(const QUrl& url)
: d( new Nepomuk::Sync::SyncFile::Private() )
{
    load( url );
}


Nepomuk::Sync::SyncFile::SyncFile(const Nepomuk::Sync::ChangeLog& log, Soprano::Model* model)
: d( new Nepomuk::Sync::SyncFile::Private() )
{
    d->m_changeLog = log;
    d->m_identificationSet = IdentificationSet::fromChangeLog( log, model );
}


Nepomuk::Sync::SyncFile::SyncFile(const Nepomuk::Sync::ChangeLog& log, const Nepomuk::Sync::IdentificationSet& ident)
: d( new Nepomuk::Sync::SyncFile::Private() )
{
    d->m_changeLog = log;
    d->m_identificationSet = ident;
}


Nepomuk::Sync::SyncFile::SyncFile(const QList<Soprano::Statement>& stList, Soprano::Model* model)
: d( new Nepomuk::Sync::SyncFile::Private() )
{
    d->m_changeLog = ChangeLog( stList );
    d->m_identificationSet = IdentificationSet::fromChangeLog( d->m_changeLog, model );
}


bool Nepomuk::Sync::SyncFile::load(const QUrl& changeLogUrl, const QUrl& identFileUrl)
{
    d->m_identificationSet = IdentificationSet::fromUrl( identFileUrl );
    d->m_changeLog = ChangeLog::fromUrl( changeLogUrl );
    return true;
}


bool Nepomuk::Sync::SyncFile::load(const QUrl& syncFile)
{
    KTar tarFile( syncFile.toString(), QString::fromLatin1("application/x-gzip") );
    if( !tarFile.open( QIODevice::ReadOnly ) ) {
        kWarning() << "File could not be opened : " << syncFile.path();
        return false;
    }

    // trueg: no need to check (dir != 0)?
    const KArchiveDirectory * dir = tarFile.directory();
    Q_ASSERT(dir);
    KTempDir tempDir;
    dir->copyTo( tempDir.name() );

    QUrl logFileUrl( tempDir.name() + "changelog" );
    QUrl identFileUrl( tempDir.name() + "identificationset" );

    return load( logFileUrl, identFileUrl );
}


bool Nepomuk::Sync::SyncFile::save( const QUrl& outFile )
{
    KTempDir tempDir;

    QUrl logFileUrl( tempDir.name() + "changelog" );
    d->m_changeLog.save( logFileUrl );

    QUrl identFileUrl( tempDir.name() + "identificationset" );
    d->m_identificationSet.save( identFileUrl );

    KTar tarFile( outFile.toString(), QString::fromLatin1("application/x-gzip") );
    if( !tarFile.open( QIODevice::WriteOnly ) ) {
        kWarning() << "File could not be opened : " << outFile.path();
        return false;
    }

    tarFile.addLocalFile( logFileUrl.path(), "changelog" );
    tarFile.addLocalFile( identFileUrl.path(), "identificationset" );

    return true;
}


Nepomuk::Sync::ChangeLog& Nepomuk::Sync::SyncFile::changeLog()
{
    return d->m_changeLog;
}


Nepomuk::Sync::IdentificationSet& Nepomuk::Sync::SyncFile::identificationSet()
{
    return d->m_identificationSet;
}

const Nepomuk::Sync::ChangeLog& Nepomuk::Sync::SyncFile::changeLog() const
{
    return d->m_changeLog;
}

const Nepomuk::Sync::IdentificationSet& Nepomuk::Sync::SyncFile::identificationSet() const
{
    return d->m_identificationSet;
}
