/*
   Copyright (C) 2010 by Serebriyskiy Artem <v.for.vandal at gmail.com>
   Copyright (C) 2008 by Dario Freddi <drf@kde.org>                      

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


#ifndef GENERALPAGE_H
#define GENERALPAGE_H

#include <QWidget>

#include "ui_generalPage.h"
#include "webexconfigbase.h"

class GeneralPage : public QWidget, private Ui_generalPage
{
    Q_OBJECT

    public:
	GeneralPage(Nepomuk::WebExConfigBase*, QWidget *parent = 0 );
	~GeneralPage();

    public Q_SLOTS:
	virtual void load();
        void save();
        void defaults();
        void emitChanged();
    Q_SIGNALS:
	void changed(bool ch);

    private:
	Nepomuk::WebExConfigBase* m_config;
};

#endif
