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

#ifndef __webextractor_ui_decision_collection_widget_h_
#define __webextractor_ui_decision_collection_widget_h_

#include <QListWidget>
#include <QSharedPointer>

#include <webextractor/decision.h>
#include <webextractor/decisionapplicationrequest.h>

#include "webextractorui_export.h"

namespace Soprano {
    class Model;
}

namespace Nepomuk {
    namespace WebExtractor {
	class DecisionList;
    }
}

class WEBEXTRACTORUI_EXPORT DecisionCollectionWidget : public QListWidget
{
    Q_OBJECT;
    public:
        DecisionCollectionWidget( QWidget * parent);
        enum Role { DecisionRole = Qt::UserRole + 1, DecisionApplicationRole };
	void addDecision( const Nepomuk::WebExtractor::Decision & d);
	void addDecisionList( const Nepomuk::WebExtractor::DecisionList & list);
	Nepomuk::WebExtractor::Decision currentDecision() const;
	QSharedPointer<Nepomuk::WebExtractor::DecisionApplicationRequest> currentDecisionApplicationRequest() const;
	using QListWidget::count;
	using QListWidget::row;
	using QListWidget::currentRow;
	using QListWidget::clear;
	Nepomuk::WebExtractor::Decision decision(int index) const;
	QSharedPointer<Nepomuk::WebExtractor::DecisionApplicationRequest> decisionApplicationRequest(int index) const;
	QSharedPointer<Nepomuk::WebExtractor::DecisionApplicationRequest> decisionApplicationRequest(int index, Soprano::Model * targetModel) ;
    Q_SIGNALS:
	void currentDecisionChanged(Nepomuk::WebExtractor::Decision previous, Nepomuk::WebExtractor::Decision current);
        void currentDecisionApplicationRequestChanged( QSharedPointer<Nepomuk::WebExtractor::DecisionApplicationRequest> current, QSharedPointer<Nepomuk::WebExtractor::DecisionApplicationRequest> previous);
	void currentItemChanged( const QListWidgetItem * current, const QListWidgetItem * previous);
    private Q_SLOTS:
        void onCurrentItemChanged(QListWidgetItem * current, QListWidgetItem * previous);

};

#endif


