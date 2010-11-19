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

#include <QtCore/QObject>
#include <QtCore/QList>
#include <QtCore/QVariant>

#include <KConfigGroup>

#include "datappdescr.h"

namespace Nepomuk {
    namespace Query {
        class Query;
    }
}
class KConfigGroup;

class Category : public QObject
{
    Q_OBJECT

public:
    Category(bool global = false, QObject* parent = 0);
    ~Category();

    bool isValid() const;

    /**
     * Global categories cannot be renamed.
     */
    bool isGlobal() const;

    /**
     * A unique ID for the category. For global categories this is a readable
     * ID like "images" or "music" which can easily be used by plugin developers
     * to add their plugins to a category.
     * Local categories have random ids.
     */
    QString identifer() const;

    bool enabled() const;
    void setEnabled(bool enabled);

    QString name() const;
    void setName(const QString& name);
    Nepomuk::Query::Query query() const;
    void setQuery(const Nepomuk::Query::Query& query);
    QString queryDescription() const;
    void setQueryDescription(const QString& desc);
    int interval() const;
    void setInterval(int i);
    uint maxResSimult() const;
    void setMaxResSimult(uint step);
    double uCrit() const;
    void setUCrit(double uCrit);
    double aCrit() const;
    void setACrit(double aCrit);

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

    /**
     * Saves the category settings without any DataPP details.
     */
    void save(KConfigGroup& config) const;
    void load(const KConfigGroup& config);

    bool operator==(const Category& other) const;
    bool operator!=(const Category& other) const;

Q_SIGNALS:
    void changed(Category* cat);

private:
    class Private;
    Private* const d;
};

Q_DECLARE_METATYPE(Category*)

QDebug operator<<(QDebug& dbg, const Category& cat);

#endif // CATEGORY_H
