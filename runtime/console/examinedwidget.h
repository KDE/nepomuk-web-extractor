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

#ifndef _webexgui_examined_model_h_
#define _webexgui_examined_model_h_

#include <QTableWidget>
#include <QDragMoveEvent>
#include <QDragEnterEvent>
#include <QDropEvent>

class ExaminedWidget : public QTableWidget
{
        Q_OBJECT;
    public:
        ExaminedWidget(QWidget * parent);
    Q_SIGNALS:
        void setExaminedDataPP(const QString & name, float version);
    protected:
        //virtual bool dropMimeData(int row, int column, const QMimeData * data, Qt::DropAction action );
        //virtual Qt::DropActions supportedDropActions() const;
        void dropEvent(QDropEvent *event);
        void dragEnterEvent(QDragEnterEvent *event);
        void dragMoveEvent(QDragMoveEvent *event);
};

#endif


