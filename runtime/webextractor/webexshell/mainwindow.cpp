/*
   This file is part of the Nepomuk KDE project.
   Copyright (C) 2010 Sebastian Trueg <trueg@kde.org>

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) version 3, or any
   later version accepted by the membership of KDE e.V. (or its
   successor approved by the membership of KDE e.V.), which shall
   act as a proxy defined in Section 6 of version 3 of the license.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with this library.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "resourceanalyzerthread.h"
#include "categoriesmodel.h"
#include <decision/decision.h>
#include <decision/decisionlist.h>

#include <KDebug>
#include <KLocale>


Q_DECLARE_METATYPE(Category*)

namespace {
const int DecisionRole = 76787;
}

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
{
    setupUi(this);

    m_analyzerThread = new ResourceAnalyzerThread(this);
    connect(m_analyzerThread, SIGNAL(analyzingResource(Nepomuk::Resource)), this, SLOT(slotAnalyzingResource(Nepomuk::Resource)));
    connect(m_analyzerThread, SIGNAL(infoMessage(QString)), m_statusLabel, SLOT(setText(QString)));
    connect(m_analyzerThread, SIGNAL(newDecisions(Nepomuk::WebExtractor::DecisionList)), this, SLOT(slotNewDecisions(Nepomuk::WebExtractor::DecisionList)));
    connect(m_analyzerThread, SIGNAL(finished()), this, SLOT(slotFinished()));
    m_comboCategory->setModel(new CategoriesModel(this));

    connect(m_buttonStart, SIGNAL(clicked()), this, SLOT(slotStart()));
    connect(m_buttonCancel, SIGNAL(clicked()), this, SLOT(slotCancel()));

    updateButtonStatus();
}

MainWindow::~MainWindow()
{
    delete m_analyzerThread;
}

void MainWindow::updateButtonStatus()
{
    m_comboCategory->setEnabled(!m_analyzerThread->isRunning());
    m_buttonStart->setEnabled(!m_analyzerThread->isRunning() && m_comboCategory->itemData(m_comboCategory->currentIndex(), CategoriesModel::CategoryRole).value<Category*>() != 0);
    m_buttonCancel->setEnabled(m_analyzerThread->isRunning());
    m_busyLabel->setVisible(m_analyzerThread->isRunning());
    if(!m_analyzerThread->isRunning()) {
        m_statusLabel->setText(i18n("Idle"));
        m_statusLabel->setToolTip(QString());
    }
}

void MainWindow::slotNewDecisions(const Nepomuk::Decision::DecisionList &decisions)
{
    kDebug();
    foreach(const Nepomuk::Decision::Decision & d, decisions) {
        // Add record to list widget
        QListWidgetItem * item = new QListWidgetItem();
        item->setData(Qt::DisplayRole, "There is no Decision::uri memeber any more");
        item->setData(DecisionRole, QVariant::fromValue(d));
        m_decisionWidget->addItem(item);
    }
}

void MainWindow::slotStart()
{
    if(!m_analyzerThread->isRunning()) {
        Category* cat = m_comboCategory->itemData(m_comboCategory->currentIndex(), CategoriesModel::CategoryRole).value<Category*>();
        if( cat ) {
            m_decisionWidget->clear();
            m_analyzerThread->start(cat);
            updateButtonStatus();
        }
    }
}

void MainWindow::slotCancel()
{
    m_analyzerThread->quit();
}

void MainWindow::slotAnalyzingResource(const Nepomuk::Resource &res)
{
    m_statusLabel->setText(i18n("Analyzing resource %1", res.genericLabel()));
    m_statusLabel->setToolTip(res.resourceUri().toString());
}

void MainWindow::slotFinished()
{
    kDebug();
    updateButtonStatus();
}
