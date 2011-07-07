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

#include "pluginselector.h"
#include <math.h>
#include <KDebug>
#include <KIcon>
#include <QItemSelectionModel>

PluginSelector::PluginSelector(QWidget * parent):
    QWidget(parent)
{
    this->setupUi(this);
    m_selected = new QStandardItemModel();
    m_selected->setHeaderData(0,Qt::Horizontal,i18n("Available plugins"),Qt::DisplayRole);
    m_available = new QStandardItemModel();
    m_selectedView->setModel(m_selected);
    m_availableView->setModel(m_available);
    this->upButton->setIcon( KIcon("go-up"));
    this->downButton->setIcon( KIcon("go-down"));
    this->selectButton->setIcon( KIcon(QApplication::isRightToLeft()? "go-previous" : "go-next"));
    this->deselectButton->setIcon( KIcon(QApplication::isRightToLeft()? "go-next" : "go-previous"));
    connect(this->upButton, SIGNAL(clicked()),  this, SLOT(moveUp()));
    connect(this->downButton, SIGNAL(clicked()),  this, SLOT(moveDown()));
    connect(this->selectButton, SIGNAL(clicked()),  this, SLOT(select()));
    connect(this->deselectButton, SIGNAL(clicked()),  this, SLOT(deselect()));
}

/*
void PluginSelector::addItem(QStandardItem * item, bool selected)
{
    if ( selected )
	m_selected->addRow(item);
    else
	m_available->addRow(item);
}

*/

void PluginSelector::addPlugin(const DataPPDescr & plugin, QString  category, bool selected)
{
    QStandardItem * item = new QStandardItem(plugin.name);
    item->setData(plugin.coff, CoffRole);
    item->setData(false, IsClassRole);
    item->setData(true, TrustedRole);
    item->setData(false, DescriptionToggledRole);


    if (category.isEmpty())
	category = i18n("Uncategorized");
    
    item->setData(category, CategoryRole);
    addItem(item, category, selected, plugin.rank);
}

void PluginSelector::addItem(QStandardItem * item, const QString & category, bool selected, double rank)
{
    // If category is known, search for item corresponding to category
    QList<QStandardItem*> items = m_available->findItems(category);
    // Filter evrything that is not category out
    for( QList<QStandardItem*>::iterator it = items.begin(); it != items.end(); it++)
    {
	if ( !(*it)->data(IsClassRole).value<bool>()) {
	    it = items.erase(it);
	}
    }


    // If there is no such category then add it
    QStandardItem * catitem;
    if (items.isEmpty()) {
	catitem = new QStandardItem(category);
	catitem->setData(QVariant(true), IsClassRole );
	m_available->appendRow(catitem);
    }
    else { 
	catitem = *items.begin();
    }

    if (!selected) {
	// If selected == false then add plugin to this category
	catitem->appendRow(item);
	kDebug() << "Adding plugin to available "<<item->text();
    }
    else {
	// Search place for is acording to rank
	int stp = m_selected->rowCount();
	// Find m: m/stp < rank < m+1/stp
	if ( rank > 1)
	    m_selected->insertRow(0,item);
	else if ( rank <= 0 )
	    m_selected->appendRow(item);
	else {
	    //m_selected->insertRow(0,item);
	    double nd = rank / (1.0 / stp );
	    int n = qMin(int(trunc(nd)),m_selected->rowCount()); ;
	    m_selected->insertRow(n, item);
	}
	//m_selected->appendRow(item);
	kDebug() << "Adding plugin to selected "<<item->text();
	kDebug() << "Selected row count " << m_selected->rowCount();
    }

}

void PluginSelector::moveUp()
{
    QItemSelectionModel *m = m_selectedView->selectionModel();
    QModelIndexList s = m->selectedIndexes();
    if (s.isEmpty())
	return;

    qSort(s);
    // Move all selected indexes one step up.
    foreach(const QModelIndex idx, s)
    {
	int row = idx.row();
	if (!row)
	    continue;

	m_selected->insertRow( row - 1, m_selected->takeRow(row) );
    }
    // Restore selection
    m->clearSelection();
    foreach(const QModelIndex idx, s)
    {
	int row = idx.row();
	if (!row)
	    continue;

    	m->clearSelection();
	m->select(m_selected->index(idx.row() - 1, idx.column()),QItemSelectionModel::Select);
    }

    emit movedUp();
}

void PluginSelector::moveDown()
{
    QItemSelectionModel *m = m_selectedView->selectionModel();
    QModelIndexList s = m->selectedIndexes();
    if (s.isEmpty())
	return;

    qSort(s);
    // Move all selected indexes one step up.
    foreach(const QModelIndex idx, s)
    {
	int row = idx.row();
	if (row == m_selected->rowCount()-1)
	    continue;

	m_selected->insertRow( row + 1, m_selected->takeRow(row) );
    }
    // Restore selection
    m->clearSelection();
    foreach(const QModelIndex idx, s)
    {
	int row = idx.row();
	if (row == m_selected->rowCount()-1)
	    continue;

    	m->clearSelection();
	m->select(m_selected->index(idx.row() + 1, idx.column()),QItemSelectionModel::Select);
    }

    emit movedDown();
}

void PluginSelector::select()
{
    QModelIndexList s = m_availableView->selectionModel()->selectedIndexes();
    if (s.isEmpty())
	return;

    kDebug() << "Selecting!";
    qSort(s);
    QList< QStandardItem*> items;
    QStandardItem* item;
    QModelIndexList toRemove;
    //QList<int> rows;
    foreach( const QModelIndex & idx,s)
    {
	item = m_available->itemFromIndex(idx);
	items << item;
        // If item is category	
	if ( item->data(IsClassRole).toBool() ) {
	    const QString & catname = item->text();
	    // If this category is not already selected
	    QList< QStandardItem*> items2 = m_selected->findItems(catname);
	    // Filter non-category
	    for( QList<QStandardItem*>::iterator it = items2.begin(); it != items2.end(); it++)
	    {
		if ( !(*it)->data(IsClassRole).value<bool>()) {
		    it = items2.erase(it);
		}
	    }

	    // So, if this category not selected
	    if (items2.isEmpty()) {
		// Select it
		QStandardItem * catitem = new QStandardItem(catname);
		catitem->setData(true, IsClassRole);
		//kDebug() << "Selecting whole category";
		m_selected->appendRow(catitem);
	    }
	    else {
		//kDebug() << "Category already selected";
	    }

	}
	else {
	    // The item is plugin
	    // Just move it
	    //m_available->takeItem(idx.row(),idx.column());
	    m_selected->appendRow(item->clone());
	    //toRemove << idx;
	    item->parent()->removeRow(idx.row());
	}
	//rows << idx.row();
    }
    emit added();
    /*
    qSort(rows);
    int i = 0;
    foreach( int row, rows)
    {
	int r = row - i;
	m_selected->appendRow(m_available->takeRow(r));
	i++;
    }
    */
}

void PluginSelector::deselect()
{
    QModelIndexList s = m_selectedView->selectionModel()->selectedIndexes();
    if (s.isEmpty())
	return;

    qSort(s);
    QList< QStandardItem*> items;
    QList<int> rows;
    QStandardItem * item;
    foreach( const QModelIndex & idx,s)
    {
	item = m_selected->itemFromIndex(idx);
	if (!item->data(IsClassRole).value<bool>()) {
	    //m_selected->takeItem(idx.row(), idx.column());
	    addItem(item->clone(), item->data(CategoryRole).value<QString>(), false);

	}
	m_selected->removeRow(idx.row());
    }
    /*
    qSort(rows);
    int i = 0;
    foreach( int row, rows)
    {
	int r = row - i;
	m_selected->takeRow(r);
	i++;
    }
    */

    emit removed();
}

QList< DataPPDescr > PluginSelector::selectedPlugins() const
{
    QList< DataPPDescr > answer;
    int stp = m_selected->rowCount();
    for ( int i = 0; i < stp; i++)
    {
	QStandardItem * item = m_selected->item(i);
	if ( !item->data(IsClassRole).toBool() ) {
	    DataPPDescr d(item->text());
	    d.rank = double(stp - i)/stp;
	    d.coff = item->data(CoffRole).toDouble();
	    d.trusted = item->data(TrustedRole).toBool();
	    answer << d;
	}
    }

    return answer;
}
