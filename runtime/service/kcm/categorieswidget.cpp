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

#include "categorieswidget.h"
#include "categoriesmodel.h"
#include "categoryeditor.h"
#include "categoriespool.h"

#include <KPushButton>
#include <KIcon>


CategoriesWidget::CategoriesWidget(QWidget *parent)
    : QWidget(parent)
{
    setupUi(this);

    m_buttonAddCategory->setIcon(KIcon(QLatin1String("list-add")));
    m_buttonRemoveCategory->setIcon(KIcon(QLatin1String("list-remove")));
    m_buttonEditCategory->setIcon(KIcon(QLatin1String("configure")));

    connect(m_buttonAddCategory, SIGNAL(clicked()), this, SLOT(slotAddCategory()));
    connect(m_buttonRemoveCategory, SIGNAL(clicked()), this, SLOT(slotRemoveCategory()));
    connect(m_buttonEditCategory, SIGNAL(clicked()), this, SLOT(slotEditCategory()));

    m_categoriesModel = new CategoriesModel(this);
    m_viewCategories->setModel(m_categoriesModel);

    connect(m_viewCategories->selectionModel(), SIGNAL(selectionChanged(QItemSelection,QItemSelection)),
            this, SLOT(slotSelectionChanged(QItemSelection,QItemSelection)));
    connect(m_viewCategories, SIGNAL(doubleClicked(QModelIndex)),
            this, SLOT(slotEditCategory()));
    slotSelectionChanged(QItemSelection(), QItemSelection());
}

CategoriesWidget::~CategoriesWidget()
{
}

void CategoriesWidget::slotAddCategory()
{
    CategoryEditor::createCaterory(this);
}

void CategoriesWidget::slotRemoveCategory()
{
}

void CategoriesWidget::slotEditCategory()
{
    Category* cat = m_viewCategories->currentIndex().data(CategoriesModel::CategoryRole).value<Category*>();
    CategoryEditor::editCategory(this, cat);
}

void CategoriesWidget::slotSelectionChanged(const QItemSelection& selected, const QItemSelection&)
{
    m_buttonEditCategory->setEnabled(selected.count() == 1);
    m_buttonRemoveCategory->setEnabled(selected.count() > 0);
}
