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

#include "console.h"
#include <kdebug.h>
#include <QUrl>
#include "launchPage.h"
#include "infoPage.h"
#include "settingsPage.h"
//#include "modeltest.h"

using namespace Nepomuk;
namespace NW = Nepomuk::WebExtractor;
ConsoleMainWindow::ConsoleMainWindow(
    const QString & uri ,
    const QStringList & datapps ,
    bool autostart ,
    QWidget * parent):
    KXmlGuiWindow(parent)
{
    this->setupUi(this);
    
    sp = new SettingsPage(this);
    this->ktabwidget->insertTab(0,sp,i18n("Settings"));
    
    ip = new InfoPage(this); 
    this->ktabwidget->insertTab(0,ip,i18n("Info"));

    lp = new LaunchPage(uri,datapps,autostart,this);
    this->ktabwidget->insertTab(0,lp,i18n("Launch"));
}



ConsoleMainWindow::~ConsoleMainWindow()
{
    delete sp;
    delete ip;
    delete lp;
}









