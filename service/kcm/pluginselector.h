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

#ifndef __plugin_selector_h_
#define __plugin_selector_h_

#include "ui_pluginSelector.h"
#include "datappdescr.h"
#include "kcm_config.h"
#include <QStandardItemModel> 
#include <QStandardItem> 
class PluginSelector : public QWidget, public Ui_pluginSelector
{
    Q_OBJECT;
    public:
	PluginSelector(QWidget * parent);

	void addPlugin(const DataPPDescr & plugin, QString  category = QString(), bool selected=false);
	QStandardItemModel * selectedModel() const { return m_selected; }
	QStandardItemModel * availableModel() const { return m_available; }
	QAbstractItemView * selectedView() const { return m_selectedView; }
	QAbstractItemView * availableView() const { return m_availableView; }
	QList< DataPPDescr > selectedPlugins() const;
	void clear() { m_selected->clear(); m_available->clear(); }
    Q_SIGNALS:
	void added();
	void removed();
	void movedUp();
	void movedDown();
    private:
	void addItem(QStandardItem * item, const QString  & category, bool selected=false, double rank = 1.0);
	QStandardItemModel * m_selected;
	QStandardItemModel * m_available;
    private Q_SLOTS:
	void moveUp();
	void moveDown();
	void select();
	void deselect();


};

#endif
