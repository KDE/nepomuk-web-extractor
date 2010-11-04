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

#ifndef CATEGORY_H
#define CATEGORY_H

#include <QtCore/QSharedDataPointer>
#include <QtCore/QList>
#include <QtCore/QVariant>

#include "datappdescr.h"

namespace Nepomuk {
    namespace Query {
        class Query;
    }
}

class Category
{
public:
    Category();
    Category(const Category& other);
    ~Category();

    Category& operator=(const Category& other);

    bool isValid() const;

    QString name() const;
    void setName(const QString& name);
    Nepomuk::Query::Query query() const;
    void setQuery(const Nepomuk::Query::Query& query);
    QString queryDescription() const;
    void setQueryDescription(const QString& desc);
    int interval() const;
    void setInterval(int i);
    int maxResStep() const;
    void setMaxResSimult(int step);
    double uCrit() const;
    void setUCrit(double uCrit);

    enum PluginSelectionType {
        Stepwise,
        All
    };
    PluginSelectionType pluginSelectionType() const;
    void setPluginSelectionType(PluginSelectionType type);
    int pluginSelectionStep() const;
    void setPluginSelectionStep(int step);

    QList<DataPPDescr> plugins() const;
    void setPlugins(const QList<DataPPDescr>& plugins);
    void addPlugin(const DataPPDescr& plugin);

    bool operator==(const Category& other) const;
    bool operator!=(const Category& other) const;

private:
    class Private;
    QSharedDataPointer<Private> d;
};

Q_DECLARE_METATYPE(Category)

#endif // CATEGORY_H
