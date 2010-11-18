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

#include "decisionwidget.h"
#include "changelogwidget.h"
#include "ui_decisionForm.h"

#include <kdebug.h>

namespace NS = Nepomuk::Sync;
namespace NW = Nepomuk::WebExtractor;

class DecisionWidget::Private
{
    public:
        Ui_DecisionForm * ui;
        NW::Decision decision;
        ChangeLogWidget * logWidget;
};

void DecisionWidget::setupUi()
{
    d->ui = new Ui_DecisionForm();
    d->ui->setupUi(this);
    d->logWidget = new ChangeLogWidget(this);
    Q_ASSERT(this->layout());
    this->layout()->addWidget(d->logWidget);
}

DecisionWidget::DecisionWidget(QWidget * parent):
    QWidget(parent),
    d(new Private())
{
    setupUi();
}

DecisionWidget::DecisionWidget(const NW::Decision & decision, QWidget * parent):
    QWidget(parent),
    d(new Private())
{
    setupUi();
    setDecision(decision);
}

DecisionWidget::~DecisionWidget()
{
    delete d->ui;
    delete d;
}

void DecisionWidget::setDecision(const NW::Decision & decision)
{
    d->decision = decision;
    kDebug() << "Log size:" << decision.log().size();
    kDebug() << "Empty:" << decision.isEmpty();
    kDebug() << "Number of goups:" << decision.groups().size();

    d->logWidget->setLog(decision.log());
    d->ui->descriptionBrowser->setPlainText(decision.description());
}

Nepomuk::WebExtractor::Decision DecisionWidget::decision() const
{
    return d->decision;
}

void DecisionWidget::clear()
{
    d->logWidget->clear();
    d->decision = NW::Decision();
}
