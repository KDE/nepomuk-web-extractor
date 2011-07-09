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

#ifndef DECISION_MANAGER_MAINWINDOW_H
#define DECISION_MANAGER_MAINWINDOW_H

#include "ui_mainWindow.h"

#include <KXmlGuiWindow>

namespace Nepomuk {
    class DecisionManagementClient;
    class DecisionModel;
}

class MainWindow : public KXmlGuiWindow, public Ui_MainWindow
{
    Q_OBJECT;
    public:
        MainWindow(Nepomuk::DecisionManagementClient * client, QWidget * parent = 0);
    private:
        Nepomuk::DecisionManagementClient * m_client;
        Nepomuk::DecisionModel * m_model;

    private Q_SLOTS:
        void onUrlSelected( const KUrl & );
};

#endif
