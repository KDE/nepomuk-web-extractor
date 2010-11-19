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

#include "queryeditor.h"

#include <KLineEdit>
#include <Nepomuk/Utils/SearchWidget>

#include <QtGui/QGridLayout>
#include <QtGui/QLabel>

QueryEditor::QueryEditor(QWidget *parent)
    : KDialog(parent)
{
    setButtons(Ok|Cancel);
    setCaption(i18n("Edit Category Query"));

    QWidget* w = mainWidget();
    QGridLayout* layout = new QGridLayout(w);
    m_editQueryDesc = new KLineEdit(w);
    m_searchWidget = new Nepomuk::Utils::SearchWidget(w);
    layout->addWidget(new QLabel(i18n("Query title:"),w), 0, 0);
    layout->addWidget(m_editQueryDesc, 0, 1);
    layout->addWidget(m_searchWidget, 1, 0, 1, 2);

    connect(m_editQueryDesc, SIGNAL(textChanged(QString)),
            this, SLOT(slotDescChanged(QString)));

    // no need to query endless numbers of results when we only want to create a query
    Nepomuk::Query::Query baseQuery;
    baseQuery.setLimit(10);
    m_searchWidget->setBaseQuery(baseQuery);

    slotDescChanged(QString());
}


void QueryEditor::slotDescChanged(const QString& s)
{
    enableButton(Ok, !s.isEmpty());
}

void QueryEditor::setQuery(const Nepomuk::Query::Query &query)
{
    m_searchWidget->setQuery(query);
}

Nepomuk::Query::Query QueryEditor::query() const
{
    return m_searchWidget->query();
}

void QueryEditor::setTitle(const QString &title)
{
    m_editQueryDesc->setText(title);
}

QString QueryEditor::title() const
{
    return m_editQueryDesc->text();
}
