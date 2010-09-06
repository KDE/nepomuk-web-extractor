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

#include "mergepoliticsselector.h"
#include <klocalizedstring.h>

MergePoliticsSelector::MergePoliticsSelector(QWidget * parent):
    QComboBox(parent)
{
    this->addItem(i18n("Lowest"), Nepomuk::WebExtractor::Lowest);
    this->addItem(i18n("Average"), Nepomuk::WebExtractor::Average);
    this->addItem(i18n("Highest"), Nepomuk::WebExtractor::Highest);
    this->addItem(i18n("Adjust"), Nepomuk::WebExtractor::Adjust);
}

Nepomuk::WebExtractor::MergePolitics MergePoliticsSelector::politics() const
{
    return Nepomuk::WebExtractor::MergePolitics(itemData(currentIndex()).toInt());
}
