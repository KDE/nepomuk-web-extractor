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

#ifndef __webexgui_console_info_page_h_
#define __webexgui_console_info_page_h_

#include "ui_infoPage.h"
#include <QWidget>

class InfoPage : public QWidget, public Ui_infoPage
{
    Q_OBJECT;
    public:
        InfoPage(QWidget * parent = 0);
    public Q_SLOTS:
        void onMarkedExamined(const QString & name, int version);

        void onClearExamined();
        void onClearAllExamined();
        void onSelectedResourceChanged();
        void updateExaminedInfo(const Nepomuk::Resource &);
        void updateServiceInfo(const Nepomuk::Resource &);
        void onUriResourceInfoChanged(const QString & text);
        void updateUriResourceInfo(const Nepomuk::Resource &);

};
#endif

