/*
   Copyright (C) 2010 by Serebriyskiy Artem <v.for.vandal at gmail.com>
   Copyright (C) 2010 Sebastian Trueg <trueg@kde.org>

   This library is free software; you can redistribute it and/or modify
   it under the terms of the GNU Library General Public License as published by
   the Free Software Foundation; either version 3, or (at your option)
   any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#ifndef __datappdescr_h_
#define __datappdescr_h_

#include <QtCore/QString>
#include <QtCore/QSharedDataPointer>

#include <KService>

namespace Nepomuk {
class CategoriesPool;
class WebExtractorPlugin;
}

class KConfigGroup;
class Category;

class DppExecutiveDescr
{
public:
    DppExecutiveDescr(const QString & id);
    DppExecutiveDescr(const DppExecutiveDescr&);
    ~DppExecutiveDescr();

    DppExecutiveDescr& operator=(const DppExecutiveDescr&);

    Category* category() const;
    //KService::Ptr service() const;

    /**
     * Create an instance of the corresponding plugin.
     * \return The new plugin instance or 0 on failure.
     * The caller takes ownership.
     */
    //Nepomuk::WebExtractorPlugin* createPlugin() const;

    /**
     * A unique ID for the datapp. This is a random string which is
     * automatically generated for local datapps.
     * Plugin developers would come up with their own ids.
     */
    //QString identifier() const;

    double rank() const;
    double coff() const;
    bool trusted() const;
    bool enabled() const;
    /*! \brief System name of the DppExecutive.
     * It is not for displaying
     */
    QString identifier() const;

    void setRank(double rank);
    void setCoff(double coff);
    void setTrusted(bool trusted);
    void setEnabled(bool enabled);


    bool isValid() const;

    bool operator==(const DppExecutiveDescr& other) const;

    void save(KConfigGroup& config) const;
    static DppExecutiveDescr load(const KConfigGroup& config);

Q_SIGNALS:
    void changed(DppExecutiveDescr*);

private:
    class Private;
    QSharedDataPointer<Private> d;

    void setCategory(Category*);

    friend class Category;
};

#endif
