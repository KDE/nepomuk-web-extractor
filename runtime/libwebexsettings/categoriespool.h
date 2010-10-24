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
#include <QStringList> 
#include <KDirWatch> 

namespace Nepomuk {
    /*! \brief This class is used for storing information about webextractor service categories
     * Category  means some set of configurations here. It is a config object 
     * that contains information about what Nepomuk resources belong to
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
	Q_OBJECT;
	public:
	    static QSet< QString> categories();
	    static CategoriesPool * self(); 
	    static void addCategory(const QString & name);
	Q_SIGNALS:
	    void categoriesChanged();
	private Q_SLOTS:
	    void update();
	private:
	    CategoriesPool();
	    void init();
	    void EmitCatChanged();
	    QSet< QString > m_categories;
	    KDirWatch wc;
    };
}

#endif
