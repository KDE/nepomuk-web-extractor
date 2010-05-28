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

#ifndef _kcm_categories_h_
#define _kcm_categories_h_

#include "datappconfig.h"
#include <QStringList> 

class Categories: public QObject
{
    public:
	Categories();
	void init();
	static const QStringList &  categories();
	static const Categories * self() { return m_self; }
    Q_SIGNALS:
	void categoriesChanged();
    private:
	QStringList m_categories;
	static Categories * m_self;
};

#endif
