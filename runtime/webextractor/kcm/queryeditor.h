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

#ifndef QUERYEDITOR_H
#define QUERYEDITOR_H

#include <KDialog>

class KLineEdit;
namespace Nepomuk {
namespace Utils {
class SearchWidget;
}
namespace Query {
class Query;
}
}

class QueryEditor : public KDialog
{
    Q_OBJECT

public:
    QueryEditor(QWidget *parent = 0);

    void setTitle(const QString& title);
    QString title() const;
    void setQuery(const Nepomuk::Query::Query& query);
    Nepomuk::Query::Query query() const;

private Q_SLOTS:
    void slotDescChanged(const QString &s);

private:
    KLineEdit* m_editQueryDesc;
    Nepomuk::Utils::SearchWidget* m_searchWidget;
};

#endif // QUERYEDITOR_H
