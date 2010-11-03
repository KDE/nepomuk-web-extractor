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

#ifndef CATEGORYEDITOR_H
#define CATEGORYEDITOR_H

#include <QWidget>
#include "ui_categoryeditor.h"

#include "category.h"

namespace Nepomuk {
    namespace Query {
        class Query;
    }
}

class PluginModel;

class CategoryEditor : public QWidget, public Ui::CategoryEditor
{
    Q_OBJECT

public:
    CategoryEditor(QWidget *parent = 0);

    void loadCategory(const Category& cat);
    Category category() const;

    static Category editCategory(QWidget* parent, const Category& cat);
    static Category createCaterory(QWidget* parent);

private Q_SLOTS:
    void slotCustomQuery();
    void selectQuery(const QString& title, const Nepomuk::Query::Query& query);

private:
    int findQueryIndex(const Nepomuk::Query::Query& query);

    /// we only keep this around since we do not handle all Category settings yet and do
    /// not want to drop any of them.
    Category m_loadedCategory;

    PluginModel* m_pluginModel;
};

#endif // CATEGORYEDITOR_H
