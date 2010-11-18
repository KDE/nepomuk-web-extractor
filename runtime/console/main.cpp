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

#include <QApplication>
#include "console.h"
#include <klocalizedstring.h>
#include <KApplication>
#include <KCmdLineArgs>
#include <QByteArray>
#include <KAboutData>
#include <KCmdLineOptions>
#include <kdebug.h>

int main(int argc, char ** argv)
{
    KAboutData about("webextracor_console", 0,
                     ki18nc("@title", "WebExtractorConsole"),
                     "1.5",
                     ki18nc("@title", "WebExtracor Console"),
                     KAboutData::License_GPL,
                     ki18nc("@info:credit", "(C) 2010 Artem Serebriyskiy"));

    KCmdLineArgs::init(argc, argv, &about);


    KCmdLineOptions options;
    options.add(QByteArray("uri < nepomuk uri >"), ki18n("Nepomuk uri for extracting"));
    options.add("start", ki18n("Start extracting immediately"));
    options.add("!+datapp", ki18n("Desired set of datapps. Unexisting and invalid will be silently ignored"));

    KCmdLineArgs::addCmdLineOptions(options);

    KApplication app;

    KCmdLineArgs * args = KCmdLineArgs::parsedArgs();

    bool autostart = args->isSet("start");
    QString uri;
    if(args->isSet("uri"))
        uri = args->getOption("uri");

    QStringList datapps;
    for(int i = 0; i < args->count(); i++)
        datapps << args->arg(i);

    qDebug() << "Selected DataPP: " << datapps;


    ConsoleMainWindow * w = new ConsoleMainWindow(uri, datapps, autostart);
    w->show();
    app.exec();

}
