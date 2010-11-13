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

#include <webextractor/decisionlist.h>
#include <webextractor/decisionapplicationrequest.h>

#include <QtGlobal>

#include "decisioncollectionwidget.h"

namespace NW = Nepomuk::WebExtractor;

Q_DECLARE_METATYPE(QSharedPointer<NW::DecisionApplicationRequest>);

DecisionCollectionWidget::DecisionCollectionWidget( QWidget * parent):
    QListWidget(parent)
{
    connect(
	    this, SIGNAL(currentItemChanged(QListWidgetItem *, QListWidgetItem*)),
	    this, SLOT(onCurrentItemChanged(QListWidgetItem *, QListWidgetItem*))
	   );
}

void DecisionCollectionWidget::addDecision( const NW::Decision & d)
{
    // Create item
    QListWidgetItem * item = new QListWidgetItem();
    // Set data
    item->setData(DecisionRole,QVariant::fromValue<NW::Decision>(d));
    
    // Add item
    this->addItem(item);
}

void DecisionCollectionWidget::addDecisionList( const NW::DecisionList & list)
{
    foreach( const NW::Decision & d, list)
    {
	addDecision(d);
    }
}

NW::Decision DecisionCollectionWidget::currentDecision() const
{
    QListWidgetItem * item = currentItem();
    NW::Decision d;
    if ( item )
        d = item->data(DecisionRole).value<NW::Decision>();
    
    return d;
}

QSharedPointer<NW::DecisionApplicationRequest> DecisionCollectionWidget::currentDecisionApplicationRequest() const
{
    QListWidgetItem * item = currentItem();
    QSharedPointer<NW::DecisionApplicationRequest> r;
    if ( item )
        r = item->data(DecisionApplicationRole).value<QSharedPointer<NW::DecisionApplicationRequest> >();
    
    return r;
}

void DecisionCollectionWidget::onCurrentItemChanged(QListWidgetItem * current, QListWidgetItem * previous )
{
    // Emit necessary signal 
    NW::Decision cur;
    NW::Decision prev;

    QSharedPointer<NW::DecisionApplicationRequest> curreq;
    QSharedPointer<NW::DecisionApplicationRequest> prevreq;

    if ( current ) {
	cur = current->data(DecisionRole).value<NW::Decision>();
	curreq = current->data(DecisionApplicationRole).value< QSharedPointer<NW::DecisionApplicationRequest> >();
    }
    if (previous ) {
	prev = previous->data(DecisionRole).value<NW::Decision>();
	prevreq = previous->data(DecisionApplicationRole).value< QSharedPointer< NW::DecisionApplicationRequest> >();
    }

    emit currentDecisionChanged(cur,prev);
    emit currentItemChanged(current,previous);
    emit currentDecisionApplicationRequestChanged(curreq,prevreq);
}


NW::Decision DecisionCollectionWidget::decision(int index) const
{
    if ( index < 0 or index >= this->count() )
        return NW::Decision();

    QListWidgetItem * item = this->item(index);
    
    NW::Decision d = item->data(DecisionRole).value<NW::Decision>();

    return d;
}

QSharedPointer<NW::DecisionApplicationRequest> DecisionCollectionWidget::decisionApplicationRequest(int index) const
{
    if ( index < 0 or index >= this->count() )
        return QSharedPointer<NW::DecisionApplicationRequest>();

    QListWidgetItem * item = this->item(index);
    
    QSharedPointer<NW::DecisionApplicationRequest> req = item->data(DecisionApplicationRole).value<QSharedPointer<NW::DecisionApplicationRequest> >();
    return req;
}

QSharedPointer<NW::DecisionApplicationRequest> DecisionCollectionWidget::decisionApplicationRequest(int index, Soprano::Model * targetModel)
{
    if ( index < 0 or index >= this->count() )
        return QSharedPointer<NW::DecisionApplicationRequest>();

    QListWidgetItem * item = this->item(index);
    
    QSharedPointer<NW::DecisionApplicationRequest> req = item->data(DecisionApplicationRole).value<QSharedPointer<NW::DecisionApplicationRequest> >();
    if (!req) { // If request doesn't exist
	// Create one
	NW::Decision d = item->data(DecisionRole).value<NW::Decision>();
	req = QSharedPointer<NW::DecisionApplicationRequest>( 
		d.applicationRequest(targetModel)
		);

	// Remember it
	item->setData(DecisionApplicationRole,QVariant::fromValue<QSharedPointer<NW::DecisionApplicationRequest> >(req));
    }
    return req;
}

