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

#ifndef __nepomuk_webexsettingsui_decisionsmodelselector_h_
#define __nepomuk_webexsettingsui_decisionsmodelselector_h_

#include "webexconfigbase.h"
#include "ui_decisionsModelSelector.h"

class DecisionsModelSelector : public QWidget, public Ui_DecisionsModelSelectorWidget
{
    Q_OBJECT;
    public:
	DecisionsModelSelector(QWidget * parent = 0);
	Nepomuk::WebExConfigBase::EnumDecisionsModelScheme::type scheme() const;
	void setScheme( Nepomuk::WebExConfigBase::EnumDecisionsModelScheme::type scheme);
	void updateSchemeList();
    Q_SIGNALS:
	void valueChanged();


};
#endif
