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

#ifndef __webexgui_console_settings_page_h_
#define __webexgui_console_settings_page_h_

#include "ui_settingsPage.h"
#include "webextractor_kcm.h"

class SettingsPage : public QWidget, public Ui_settingsPage
{
    Q_OBJECT;
    public:
        SettingsPage(QWidget * parent = 0);
    private Q_SLOTS:
        void applySettings();
        void resetSettings();
        void defaultSettings();
        void dataPPClicked(QModelIndex);
        void dataPPSettingsChanged(bool);

    private:
        /* KCM subsection */
        Nepomuk::WebExtractorPluginKCM::Ptr m_currentKcm;
        bool m_kcmChanged;
        QAbstractButton * m_kcmApplyButton;
        QAbstractButton * m_kcmResetButton;
        QAbstractButton * m_kcmDefaultButton;
};

#endif


