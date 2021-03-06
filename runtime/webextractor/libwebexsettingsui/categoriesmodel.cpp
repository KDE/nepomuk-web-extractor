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

#include "categoriesmodel.h"
#include "categoriespool.h"

#include <KGlobalSettings>
#include <KDebug>

CategoriesModel::CategoriesModel(QObject *parent) :
    QAbstractListModel(parent)
{
    connect(Nepomuk::CategoriesPool::self(), SIGNAL(categoriesChanged()),
            this, SLOT(slotCategoriesChanged()));
}

QVariant CategoriesModel::data(const QModelIndex &index, int role) const
{
    QList<Category*> cats = Nepomuk::CategoriesPool::self()->categories();
    if( index.row() >= 0 && index.row() < cats.count() ) {
        Category* cat = cats[index.row()];

        switch(role) {
        case Qt::DisplayRole:
            return cat->name();

        case Qt::FontRole:
            if( cat->isGlobal()) {
                QFont font = KGlobalSettings::generalFont();
                font.setItalic(true);
                return font;
            }
            break;

        case Qt::CheckStateRole:
            return cat->enabled() ? Qt::Checked : Qt::Unchecked;

        case CategoryRole:
            return QVariant::fromValue(cat);
        }
    }
    return QVariant();
}

int CategoriesModel::rowCount(const QModelIndex &parent) const
{
    return Nepomuk::CategoriesPool::self()->categories().count();
}

void CategoriesModel::slotCategoriesChanged()
{
    reset();
}

Qt::ItemFlags CategoriesModel::flags(const QModelIndex &index) const
{
    return QAbstractListModel::flags(index)|Qt::ItemIsUserCheckable;
}

bool CategoriesModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    QList<Category*> cats = Nepomuk::CategoriesPool::self()->categories();
    if( index.row() < cats.count() ) {
        Category* cat = cats[index.row()];
        if(role == Qt::CheckStateRole) {
            bool checked = value.toBool();
            kDebug() << checked;
            cat->setEnabled(checked);
            emit dataChanged(index, index);
            return true;
        }
    }

    return QAbstractListModel::setData(index, value, role);
}
