/*
   Copyright (C) 2011 by Serebriyskiy Artem <v.for.vandal at gmail.com>

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
#include <klocalizedstring.h>
#include <KApplication>
#include <KCmdLineArgs>
#include <QByteArray>
#include <KAboutData>
#include <KCmdLineOptions>
#include <kdebug.h>

#include "mainwindow.h"
#include "client.h"

int main(int argc, char ** argv)
{
    KAboutData about("nepomuk_decision_manager", 0,
                     ki18nc("@title", "Decision Manager"),
                     "1.5",
                     ki18nc("@title", "Decision Manager"),
                     KAboutData::License_GPL,
                     ki18nc("@info:credit", "(C) 2010 Artem Serebriyskiy"));

    KCmdLineArgs::init(argc, argv, &about);



    KApplication app;

    KCmdLineArgs * args = KCmdLineArgs::parsedArgs();


    MainWindow * w = new MainWindow(Nepomuk::DecisionManagementClient::instance());
    w->show();
    app.exec();

}
