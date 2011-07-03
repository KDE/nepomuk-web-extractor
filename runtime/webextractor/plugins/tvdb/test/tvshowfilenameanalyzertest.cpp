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

#include "tvshowfilenameanalyzertest.h"
#include "tvshowfilenameanalyzer.h"

#include <QtTest/QtTest>
#include <qtest_kde.h>

void TVShowFilenameAnalyzerTest::testAnalyzeFilename_data()
{
    QTest::addColumn<QString>( "filename" );
    QTest::addColumn<QString>( "name" );
    QTest::addColumn<int>( "season" );
    QTest::addColumn<int>( "episode" );

    // 1. SAA EBB
    QTest::newRow("foobar1") << QString::fromLatin1("foobar.s2.e3.avi") << QString::fromLatin1( "foobar" ) << 2 << 3;
    QTest::newRow("foobar2") << QString::fromLatin1("foobar.s2.e3.hello.avi") << QString::fromLatin1( "foobar" ) << 2 << 3;
    QTest::newRow("foobar3") << QString::fromLatin1("foobar.S2.E3.avi") << QString::fromLatin1( "foobar" ) << 2 << 3;
    QTest::newRow("foobar4") << QString::fromLatin1("foobar_s2.e3 hello.avi") << QString::fromLatin1( "foobar" ) << 2 << 3;
    QTest::newRow("foobar5") << QString::fromLatin1("foobar_s2_e3_hello.avi") << QString::fromLatin1( "foobar" ) << 2 << 3;
    QTest::newRow("foobar6") << QString::fromLatin1("foobar s2 e3 hello.avi") << QString::fromLatin1( "foobar" ) << 2 << 3;
    QTest::newRow("foobar7") << QString::fromLatin1("foobar s02 e03 hello.avi") << QString::fromLatin1( "foobar" ) << 2 << 3;
    QTest::newRow("foobar8") << QString::fromLatin1("foobar-s02-e03-hello.avi") << QString::fromLatin1( "foobar" ) << 2 << 3;
    QTest::newRow("foobar9") << QString::fromLatin1("foobar - s02 e03 - hello.avi") << QString::fromLatin1( "foobar" ) << 2 << 3;

    // 2. AAxBB
    QTest::newRow("foobar10") << QString::fromLatin1("foobar.2x3.avi") << QString::fromLatin1( "foobar" ) << 2 << 3;
    QTest::newRow("foobar11") << QString::fromLatin1("foobar_2x3.avi") << QString::fromLatin1( "foobar" ) << 2 << 3;
    QTest::newRow("foobar12") << QString::fromLatin1("foobar-2x3.avi") << QString::fromLatin1( "foobar" ) << 2 << 3;
    QTest::newRow("foobar13") << QString::fromLatin1("foobar 2x3.avi") << QString::fromLatin1( "foobar" ) << 2 << 3;

    QTest::newRow("foobar14") << QString::fromLatin1("foobar.2x3.hello.avi") << QString::fromLatin1( "foobar" ) << 2 << 3;
    QTest::newRow("foobar15") << QString::fromLatin1("foobar_2x3_hello.avi") << QString::fromLatin1( "foobar" ) << 2 << 3;
    QTest::newRow("foobar16") << QString::fromLatin1("foobar-2x3-hello.avi") << QString::fromLatin1( "foobar" ) << 2 << 3;
    QTest::newRow("foobar17") << QString::fromLatin1("foobar 2x3 hello.avi") << QString::fromLatin1( "foobar" ) << 2 << 3;

    QTest::newRow("foobar18") << QString::fromLatin1("foobar.02x03.avi") << QString::fromLatin1( "foobar" ) << 2 << 3;
    QTest::newRow("foobar19") << QString::fromLatin1("foobar_02x03.avi") << QString::fromLatin1( "foobar" ) << 2 << 3;
    QTest::newRow("foobar20") << QString::fromLatin1("foobar-02x03.avi") << QString::fromLatin1( "foobar" ) << 2 << 3;
    QTest::newRow("foobar21") << QString::fromLatin1("foobar 02x03.avi") << QString::fromLatin1( "foobar" ) << 2 << 3;

    QTest::newRow("foobar22") << QString::fromLatin1("foobar.02x03.hello.avi") << QString::fromLatin1( "foobar" ) << 2 << 3;
    QTest::newRow("foobar23") << QString::fromLatin1("foobar_02x03_hello.avi") << QString::fromLatin1( "foobar" ) << 2 << 3;
    QTest::newRow("foobar24") << QString::fromLatin1("foobar-02x03-hello.avi") << QString::fromLatin1( "foobar" ) << 2 << 3;
    QTest::newRow("foobar25") << QString::fromLatin1("foobar 02x03 hello.avi") << QString::fromLatin1( "foobar" ) << 2 << 3;

    QTest::newRow("foobar26") << QString::fromLatin1("foobar.002x003.avi") << QString::fromLatin1( "foobar" ) << 2 << 3;
    QTest::newRow("foobar27") << QString::fromLatin1("foobar_002x003.avi") << QString::fromLatin1( "foobar" ) << 2 << 3;
    QTest::newRow("foobar28") << QString::fromLatin1("foobar-002x003.avi") << QString::fromLatin1( "foobar" ) << 2 << 3;
    QTest::newRow("foobar29") << QString::fromLatin1("foobar 002x003.avi") << QString::fromLatin1( "foobar" ) << 2 << 3;

    QTest::newRow("foobar30") << QString::fromLatin1("foobar.002x003.hello.avi") << QString::fromLatin1( "foobar" ) << 2 << 3;
    QTest::newRow("foobar31") << QString::fromLatin1("foobar_002x003_hello.avi") << QString::fromLatin1( "foobar" ) << 2 << 3;
    QTest::newRow("foobar32") << QString::fromLatin1("foobar-002x003-hello.avi") << QString::fromLatin1( "foobar" ) << 2 << 3;
    QTest::newRow("foobar33") << QString::fromLatin1("foobar 002x003 hello.avi") << QString::fromLatin1( "foobar" ) << 2 << 3;

    // ABB
    QTest::newRow("foobar34") << QString::fromLatin1("foobar.203.avi") << QString::fromLatin1( "foobar" ) << 2 << 3;
    QTest::newRow("foobar35") << QString::fromLatin1("foobar_203.avi") << QString::fromLatin1( "foobar" ) << 2 << 3;
    QTest::newRow("foobar36") << QString::fromLatin1("foobar-203.avi") << QString::fromLatin1( "foobar" ) << 2 << 3;
    QTest::newRow("foobar37") << QString::fromLatin1("foobar 203.avi") << QString::fromLatin1( "foobar" ) << 2 << 3;

    QTest::newRow("foobar38") << QString::fromLatin1("foobar.203.hello.avi") << QString::fromLatin1( "foobar" ) << 2 << 3;
    QTest::newRow("foobar39") << QString::fromLatin1("foobar_203_hello.avi") << QString::fromLatin1( "foobar" ) << 2 << 3;
    QTest::newRow("foobar40") << QString::fromLatin1("foobar-203-hello.avi") << QString::fromLatin1( "foobar" ) << 2 << 3;
    QTest::newRow("foobar41") << QString::fromLatin1("foobar 203 hello.avi") << QString::fromLatin1( "foobar" ) << 2 << 3;

    // AABB
    QTest::newRow("foobar42") << QString::fromLatin1("foobar.0203.avi") << QString::fromLatin1( "foobar" ) << 2 << 3;
    QTest::newRow("foobar43") << QString::fromLatin1("foobar_0203.avi") << QString::fromLatin1( "foobar" ) << 2 << 3;
    QTest::newRow("foobar44") << QString::fromLatin1("foobar-0203.avi") << QString::fromLatin1( "foobar" ) << 2 << 3;
    QTest::newRow("foobar45") << QString::fromLatin1("foobar 0203.avi") << QString::fromLatin1( "foobar" ) << 2 << 3;

    QTest::newRow("foobar46") << QString::fromLatin1("foobar.0203.hello.avi") << QString::fromLatin1( "foobar" ) << 2 << 3;
    QTest::newRow("foobar47") << QString::fromLatin1("foobar_0203_hello.avi") << QString::fromLatin1( "foobar" ) << 2 << 3;
    QTest::newRow("foobar48") << QString::fromLatin1("foobar-0203-hello.avi") << QString::fromLatin1( "foobar" ) << 2 << 3;
    QTest::newRow("foobar49") << QString::fromLatin1("foobar 0203 hello.avi") << QString::fromLatin1( "foobar" ) << 2 << 3;

    // misc
    QTest::newRow("misc1") << QString::fromLatin1("How I Met Your Mother - 01x01 - Pilot.avi") << QString::fromLatin1( "How I Met Your Mother" ) << 1 << 1;
}


void TVShowFilenameAnalyzerTest::testAnalyzeFilename()
{
    QFETCH( QString, filename );
    QFETCH( QString, name );
    QFETCH( int, season );
    QFETCH( int, episode );

    TVShowFilenameAnalyzer tva;
    TVShowFilenameAnalyzer::AnalysisResult result = tva.analyzeFilename( filename );
    QVERIFY( result.isValid() );
    QCOMPARE( result.name, name );
    QCOMPARE( result.season, season );
    QCOMPARE( result.episode, episode );
}

QTEST_KDEMAIN_CORE( TVShowFilenameAnalyzerTest )

#include "tvshowfilenameanalyzertest.moc"
