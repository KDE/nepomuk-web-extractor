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

#ifndef CATEGORIESMODEL_H
#define CATEGORIESMODEL_H

#include <QAbstractListModel>

class CategoriesModel : public QAbstractListModel
{
    Q_OBJECT

public:
    CategoriesModel(QObject *parent = 0);

    enum CustomRoles {
        CategoryRole = 685689
    };

    QVariant data(const QModelIndex &index, int role) const;
    int rowCount(const QModelIndex &parent) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);

private Q_SLOTS:
    void slotCategoriesChanged();
};

#endif // CATEGORIESMODEL_H
