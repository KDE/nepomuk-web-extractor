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

#include <decision/decision.h>
#include <decision/decisionapplicationrequest.h>

#include "decisionui_export.h"

namespace Soprano {
    class Model;
}

namespace Nepomuk {
    namespace Decision {
        class DecisionList;
    }
}

class DECISIONUI_EXPORT DecisionCollectionWidget : public QListWidget
{
    Q_OBJECT;
    public:
        DecisionCollectionWidget( QWidget * parent);
        enum Role { DecisionRole = Qt::UserRole + 1, DecisionApplicationRole };
        void addDecision( const Nepomuk::Decision::Decision & d);
        void addDecisionList( const Nepomuk::Decision::DecisionList & list);
        Nepomuk::Decision::Decision currentDecision() const;
        QSharedPointer<Nepomuk::Decision::DecisionApplicationRequest> currentDecisionApplicationRequest() const;
        using QListWidget::count;
        using QListWidget::row;
        using QListWidget::currentRow;
        using QListWidget::clear;
        Nepomuk::Decision::Decision decision(int index) const;
        QSharedPointer<Nepomuk::Decision::DecisionApplicationRequest> decisionApplicationRequest(int index) const;
        QSharedPointer<Nepomuk::Decision::DecisionApplicationRequest> decisionApplicationRequest(int index, Soprano::Model * targetModel) ;
    Q_SIGNALS:
        void currentDecisionChanged(Nepomuk::Decision::Decision previous, Nepomuk::Decision::Decision current);
        void currentDecisionApplicationRequestChanged( QSharedPointer<Nepomuk::Decision::DecisionApplicationRequest> current, QSharedPointer<Nepomuk::Decision::DecisionApplicationRequest> previous);
        void currentItemChanged( const QListWidgetItem * current, const QListWidgetItem * previous);
    private Q_SLOTS:
        void onCurrentItemChanged(QListWidgetItem * current, QListWidgetItem * previous);

};

#endif


