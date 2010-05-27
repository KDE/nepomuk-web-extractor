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


#ifndef __pluginItemDelegate_h_
#define __pluginItemDelegate_h_

#include <kwidgetitemdelegate.h>

class CategoryPluginItemDelegate : public KWidgetItemDelegate
{
    Q_OBJECT;
    public:
        CategoryPluginItemDelegate(QAbstractItemView *itemView, QObject *parent = 0):
	    KWidgetItemDelegate(itemView, parent)
	  {;}
	virtual ~CategoryPluginItemDelegate() {;}
	QList<QWidget*> createItemWidgets() const;
	 virtual void updateItemWidgets(
				const QList<QWidget*> widgets,
				const QStyleOptionViewItem &option,
				const QPersistentModelIndex &index
				) const;
	  virtual void paint(
		  QPainter *painter,
		  const QStyleOptionViewItem &option,
		  const QModelIndex &index
		  ) const;

	  virtual QSize sizeHint(const QStyleOptionViewItem&, const QModelIndex&) const
	  {return QSize(1000, 200); }
};
#endif

