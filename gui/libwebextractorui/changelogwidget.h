/*
   Copyright (C) 2010 by Serebriyskiy Artem <v.for.vandal at gmail.com>

   This library is free software; you can redistribute it and/or modify
   it under the terms of the GNU Library General Public License as published by
   the Free Software Foundation; either version 3, or (at your option)
   any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#ifndef __webextractor_ui_changelog_widget_h_
#define __webextractor_ui_changelog_widget_h_

#include <QWidget>
#include "changelog.h"
#include "webextractorui_export.h"

class WEBEXTRACTORUI_EXPORT ChangeLogWidget : public QWidget
{
    Q_OBJECT;
    public:
	ChangeLogWidget(QWidget * parent = 0);
	ChangeLogWidget(const Nepomuk::Sync::ChangeLog & log, QWidget * parent = 0);
	~ChangeLogWidget();
	void setLog( const Nepomuk::Sync::ChangeLog & log);
    public Q_SLOTS:
	void clear();
    private:
	class Private;
	Private * d;
};

#endif
