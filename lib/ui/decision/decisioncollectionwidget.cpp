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

#include <decision/decisionlist.h>
#include <decision/decisionapplicationrequest.h>

#include <QtGlobal>


#include "decisioncollectionwidget.h"

namespace ND = Nepomuk::Decision;

Q_DECLARE_METATYPE(QSharedPointer<KJob>);

DecisionCollectionWidget::DecisionCollectionWidget( QWidget * parent):
    QListWidget(parent)
{
    connect(
	    this, SIGNAL(currentItemChanged(QListWidgetItem *, QListWidgetItem*)),
	    this, SLOT(onCurrentItemChanged(QListWidgetItem *, QListWidgetItem*))
	   );
}

void DecisionCollectionWidget::addDecision( const ND::Decision & d)
{
    // Create item
    QListWidgetItem * item = new QListWidgetItem();
    // Set data
    item->setData(DecisionRole,QVariant::fromValue<ND::Decision>(d));
    item->setData(Qt::DisplayRole, this->count());
    
    // Add item
    this->addItem(item);
}

void DecisionCollectionWidget::addDecisionList( const ND::DecisionList & list)
{
    foreach( const ND::Decision & d, list)
    {
	addDecision(d);
    }
}

ND::Decision DecisionCollectionWidget::currentDecision() const
{
    QListWidgetItem * item = currentItem();
    ND::Decision d;
    if ( item )
        d = item->data(DecisionRole).value<ND::Decision>();
    
    return d;
}

QSharedPointer<KJob> 
DecisionCollectionWidget::currentDecisionApplicationRequest() const
{
    QListWidgetItem * item = currentItem();
    QSharedPointer<KJob> r;
    if ( item )
        r = item->data(DecisionApplicationRole).value<QSharedPointer<KJob> >();
    
    return r;
}

void DecisionCollectionWidget::onCurrentItemChanged(QListWidgetItem * current, QListWidgetItem * previous )
{
    // Emit necessary signal 
    ND::Decision cur;
    ND::Decision prev;

    QSharedPointer<KJob> curreq;
    QSharedPointer<KJob> prevreq;

    if ( current ) {
	cur = current->data(DecisionRole).value<ND::Decision>();
	curreq = current->data(DecisionApplicationRole).value< QSharedPointer<KJob> >();
    }
    if (previous ) {
	prev = previous->data(DecisionRole).value<ND::Decision>();
	prevreq = previous->data(DecisionApplicationRole).value< QSharedPointer< KJob> >();
    }

    emit currentDecisionChanged(cur,prev);
    emit currentItemChanged(current,previous);
    emit currentDecisionApplicationRequestChanged(curreq,prevreq);
}


ND::Decision DecisionCollectionWidget::decision(int index) const
{
    if ( index < 0 or index >= this->count() )
        return ND::Decision();

    QListWidgetItem * item = this->item(index);
    
    ND::Decision d = item->data(DecisionRole).value<ND::Decision>();

    return d;
}

QSharedPointer<KJob>
DecisionCollectionWidget::decisionApplicationRequest(int index) const
{
    if ( index < 0 or index >= this->count() )
        return QSharedPointer<KJob>();

    QListWidgetItem * item = this->item(index);
    
    QSharedPointer<KJob> req = item->data(DecisionApplicationRole).value<QSharedPointer<KJob> >();
    if (!req) { // If request doesn't exist
	// Create one
	ND::Decision d = item->data(DecisionRole).value<ND::Decision>();
	req = QSharedPointer<KJob>( 
		d.applyJob()
		);

	// Remember it
	item->setData(DecisionApplicationRole,QVariant::fromValue<QSharedPointer<KJob> >(req));
    }
    return req;
}

/*
QSharedPointer<ND::DecisionApplicationRequest> DecisionCollectionWidget::decisionApplicationRequest(int index, Soprano::Model * targetModel)
{
    if ( index < 0 or index >= this->count() )
        return QSharedPointer<ND::DecisionApplicationRequest>();

    QListWidgetItem * item = this->item(index);
    
    QSharedPointer<ND::DecisionApplicationRequest> req = item->data(DecisionApplicationRole).value<QSharedPointer<ND::DecisionApplicationRequest> >();
    if (!req) { // If request doesn't exist
	// Create one
	ND::Decision d = item->data(DecisionRole).value<ND::Decision>();
	req = QSharedPointer<ND::DecisionApplicationRequest>( 
		d.applicationRequest(targetModel)
		);

	// Remember it
	item->setData(DecisionApplicationRole,QVariant::fromValue<QSharedPointer<ND::DecisionApplicationRequest> >(req));
    }
    return req;
}
*/

