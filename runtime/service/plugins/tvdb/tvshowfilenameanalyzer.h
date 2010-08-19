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

#ifndef _TVSHOW_FILENAME_ANALYZER_H_
#define _TVSHOW_FILENAME_ANALYZER_H_

#include <QtCore/QList>
#include <QtCore/QRegExp>
#include <QtCore/QMutex>

/**
 * The filename analyzis is wrapped in a class
 * to prevent re-parsing of regular expressions
 * on each call.
 */
class TVShowFilenameAnalyzer
{
public:
    TVShowFilenameAnalyzer();
    ~TVShowFilenameAnalyzer();

    struct AnalysisResult {
        AnalysisResult()
            : season( -1 ),
              episode( -1 ) {
        }

        bool isValid() const {
            return( !name.isEmpty() && season >= 0 && episode >= 0 );
        }

        QString name;
        int season;
        int episode;
    };

    AnalysisResult analyzeFilename( const QString& name );

private:
    QList<QRegExp> m_filenameRegExps;

    QMutex m_mutex;
};

#endif
