/*
   Copyright (C) 2010 by Sebastian Trueg <trueg@kde.org>

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public License as
   published by the Free Software Foundation; either version 2 of
   the License or (at your option) version 3 or any later version
   accepted by the membership of KDE e.V. (or its successor approved
   by the membership of KDE e.V.), which shall act as a proxy
   defined in Section 14 of version 3 of the license.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "tvshowfilenameanalyzer.h"

#include <QtCore/QList>
#include <QtCore/QRegExp>
#include <QtCore/QFileInfo>
#include <QtCore/QMutexLocker>

#include <KDebug>

TVShowFilenameAnalyzer::TVShowFilenameAnalyzer()
{
    // Regular expressions to parse file based on the ones from tvnamer.py
    // Copyright dbr/Ben
    // Regex's to parse filenames with. Must have 3 groups, seriesname, season number
    // and episode number. Use (?: optional) non-capturing groups if you need others.

    // foo_[s01]_[e01]
    m_filenameRegExps.append(
        QRegExp( QLatin1String( "(.+)[ \\._\\-]\\[s([0-9]+)\\]_\\[e([0-9]+)\\]?[^\\\\/]*" ),
                 Qt::CaseInsensitive, QRegExp::RegExp2 ) );

    // foo.1x09*
    m_filenameRegExps.append(
        QRegExp( QLatin1String( "(.+)[ \\._\\-]\\[?([0-9]+)x([0-9]+)[^\\d]?[^\\\\/]*" ),
                 Qt::CaseInsensitive, QRegExp::RegExp2 ) );

    // foo.s01.e01, foo.s01_e01
    m_filenameRegExps.append(
        QRegExp( QLatin1String( "(.+)[ \\._\\-]s([0-9]+)[\\._\\- ]?e([0-9]+)[^\\\\/]*" ),
                 Qt::CaseInsensitive, QRegExp::RegExp2 ) );

    // foo.103* (the strange part at the end is used to 1. prevent another digit and 2. allow the name to end)
    m_filenameRegExps.append(
        QRegExp( QLatin1String( "(.+)[ \\._\\-]([0-9]{1})([0-9]{2})(?:[^\\d][^\\\\/]*)?" ),
                 Qt::CaseInsensitive, QRegExp::RegExp2 ) );

    // foo.0103* (the strange part at the end is used to 1. prevent another digit and 2. allow the name to end)
    m_filenameRegExps.append(
        QRegExp( QLatin1String( "(.+)[ \\._\\-]([0-9]{2})([0-9]{2,3})(?:[^\\d][^\\\\/]*)?" ),
                 Qt::CaseInsensitive, QRegExp::RegExp2 ) );
}


TVShowFilenameAnalyzer::~TVShowFilenameAnalyzer()
{
}


TVShowFilenameAnalyzer::AnalysisResult TVShowFilenameAnalyzer::analyzeFilename( const QString& path )
{
    QMutexLocker lock( &m_mutex );

    // 1. extract base name of the file
    // TODO: analyze the path, too. In case we have something like: "foobar - Season 2/02x12 - blabla.avi"
    QFileInfo fi( path );
//    const QString dir = fi.absolutePath();
    QString name = fi.completeBaseName();

    kDebug() << name;

    // 2. run the base name through all regular expressions we have
    for ( int i = 0; i < m_filenameRegExps.count(); ++i ) {
        QRegExp& exp = m_filenameRegExps[i];
        if ( exp.exactMatch( name ) ) {
            AnalysisResult result;

            kDebug() << "Regexp matched:"<<i;
            result.name = exp.cap( 1 ).simplified();
            result.season = exp.cap( 2 ).toInt();
            result.episode = exp.cap( 3 ).toInt();

            // 3. clean up tv show name
            result.name.replace( '.', ' ' );
            result.name.replace( '_', ' ' );
            if ( result.name.endsWith( '-' ) )
                result.name.truncate( result.name.length()-1 );
            result.name = result.name.simplified();

            return result;
        }
    }

    return AnalysisResult();
}
