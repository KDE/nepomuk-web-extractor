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

#include <QtCore/QCoreApplication>
#include <QtCore/QTextStream>
#include <QtCore/QStringList>

#include <KComponentData>
#include <KDebug>

#include "../tvshowfilenameanalyzer.h"

#include "stdio.h"

int main( int argc, char **argv )
{
    QCoreApplication app( argc, argv );
    KComponentData comp( "filenameanalyzer" );

    QTextStream err( stderr );

    const QString path = app.arguments()[1];

    TVShowFilenameAnalyzer fna;
    if ( fna.analyzeFilename( path ) ) {
        err << "Extracted name: '" << fna.name() << "', Season: '" << fna.season() << "', Episode: '" << fna.episode() << "'" << endl;
    }
    else {
        err << "Failed to extract series name from " << path << endl;
    }
}
