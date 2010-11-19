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

#ifndef _webexgui_console_h_
#define _webexgui_console_h_

#include "ui_mainWindow.h"
#include <QMainWindow>
#include <KXmlGuiWindow>

namespace Nepomuk {
    class WebExtractorPluginKCM;
}

class LaunchPage;
class SettingsPage;
class InfoPage;

class ConsoleMainWindow : public KXmlGuiWindow , public Ui_MainWindow
{
        Q_OBJECT;
    public:
        ConsoleMainWindow(
            const QString & uri = QString(),
            const QStringList & datapps = QStringList(),
            bool autostart = false,
            QWidget * parent = 0);
        ~ConsoleMainWindow();
	//void kcmButtonClicked();
    private:
        LaunchPage * lp;
        SettingsPage * sp;
        InfoPage * ip;



};

#endif

