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

#ifndef _kcm_plugins_h_
#define _kcm_plugins_h_

#include "datappconfig.h"
#include <QStringList> 

class Plugins: public QObject
{
    public:
	Plugins();
	void init();
	static const QStringList &  plugins();
	static const Plugins * self() { return m_self; }
    Q_SIGNALS:
	void pluginsChanged();
    private:
	QStringList m_plugins;
	static Plugins * m_self;
};

#endif
