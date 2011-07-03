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


#include <QComboBox>
#include "webexsettings_global.h"
#include <decision/global.h>

class MergePoliticsSelector: public QComboBox
{
        Q_OBJECT;
    public:
        MergePoliticsSelector(QWidget * parent = 0);
        Nepomuk::Decision::MergePolitics politics() const;
    private:
        // Only defenition
        // addItem();
};
