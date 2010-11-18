/*
   Copyright (C) 2010 by Serebriyskiy Artem <v.for.vandal at gmail.com>

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

#ifndef _webexsettings_categories_h_
#define _webexsettings_categories_h_

#include "datappconfig.h"
#include "category.h"
#include <QtCore/QStringList>


namespace Nepomuk {
    /*! \brief This class is used for storing information about webextractor service categories
     * Category  means some set of configurations here. It is a config object 
     * that contains information about wha^t Nepomuk resources belong to
     * this category and what DataPP should process this resources.
     * \note <b> Do not mix the webextractor service category and category of the DataPP! </b>
     * The category of the DataPP is just a tags that can be used for more convinient 
     * representation.
     *
     * CategoriesPool contains all information about all existing categories, both 
     * system wide and user installed.
     */
    class CategoriesPool: public QObject
    {
	Q_OBJECT

    public:
        /**
         * Create a new CategoriesPool. Typically one would use
         * self() instead.
         */
        CategoriesPool();
        ~CategoriesPool();

        QList<Category*> categories() const;
        Category* category(const QString& name) const;

        /**
         * Stores a category into the pool, replacing
         * an already existing category with the same name.
         * \return \p true if \p cat was valid.
         */
        bool addCategory(Category* cat);

        /**
         * Remove the Category with the title \p name.
         * \return \p true if found, \p false otherwise.
         */
        bool removeCategory(const QString& name);

        /**
         * Access the global CategoriesPool. Categories are
         * loaded from disk on creation.
         */
        static CategoriesPool* self();

    public Q_SLOTS:
        /**
         * Reload the categories form configuration on disk.
         */
        void reloadCategories();

        /**
         * Save all categories to disk.
         */
        void saveCategories();

        /**
         * Set the auto update feature. If enabled CategoriesPool
         * will automatically reload changed on disk. This should
         * never be enabled for a configuration dialog.
         *
         * By default this is disabled.
         */
        void setAutoUpdate(bool autoUpdate);

    Q_SIGNALS:
        void categoriesChanged();

    private:
        class Private;
        Private* const d;

        Q_PRIVATE_SLOT(d, void _k_categoryChanged(Category*))
    };
}

#endif
