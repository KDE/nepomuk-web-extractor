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

#ifndef __webextractor_ui_decision_widget_h_
#define __webextractor_ui_decision_widget_h_

#include <QWidget>
#include "webextractorui_export.h"
#include "decision.h"

class WEBEXTRACTORUI_EXPORT DecisionWidget : public QWidget
{
        Q_OBJECT;
    public:
        DecisionWidget(QWidget * parent = 0);
        DecisionWidget(const Nepomuk::WebExtractor::Decision & dec, QWidget * parent = 0);
        ~DecisionWidget();
        void setDecision(const Nepomuk::WebExtractor::Decision & dec);
        Nepomuk::WebExtractor::Decision decision() const;
    public Q_SLOTS:
        void clear();
    private:
        class Private;
        Private * d;
        void setupUi();
};

#endif
