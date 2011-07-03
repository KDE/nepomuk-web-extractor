/*
   Copyright (C) 2011 by Serebriyskiy Artem <v.for.vandal at gmail.com>

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

#include "mainwindow.h"
#include "client.h"
#include "decision_model.h"

#include <QtCore/QtDebug>
#include <QDeclarativeContext>


MainWindow::MainWindow(
        Nepomuk::DecisionManagementClient * client,
        QWidget * parent):
    KXmlGuiWindow(parent),
    m_client(0),
    m_model(0)
{
    this->setupUi(this);
    Q_ASSERT(client);

    this->m_client = client;
    this->m_model = new Nepomuk::DecisionModel(m_client);
    QDeclarativeContext * ctx = this->mainView->rootContext();
    ctx->setContextProperty("MainModel",m_model);
    mainView->setSource(QUrl("qrc:/qml/list/list.qml"));

    connect(this->kurlrequester,SIGNAL(urlSelected(const KUrl & )),
            this, SLOT(onUrlSelected(const KUrl &))
           );
}

void MainWindow::onUrlSelected( const KUrl & url)
{
    this->m_model->setUri(url);
}
