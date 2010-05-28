/*
   Copyright (C) 2010 by Serebriyskiy Artem <v.for.vandal at gmail.com>
   Copyright (C) 2008 by Dario Freddi <drf@kde.org>                      

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#ifndef CATEGORIESPAGE_H
#define CATEGORIESPAGE_H

#include <QWidget>
#include <QHash>
#include <QAbstractItemDelegate>
#include <QStateMachine>
#include <QStandardItemModel>

#include "ui_categoriesPage.h"
#include "webextractor_config.h"
#include "webexcategory.h"
#include "pluginItemDelegate.h"
#include "pluginselector.h"

class CategoriesPage : public QWidget, private Ui_categoriesPage
{
    Q_OBJECT;

public:
    CategoriesPage(Nepomuk::WebExtractorConfig* cfg,QWidget *parent = 0);
    ~CategoriesPage();
    public Q_SLOTS:
	virtual void load();
        void save();
        void defaults();
        void emitChanged();
	void setCategoryChanged();
	void switchCategory(QListWidgetItem *current, QListWidgetItem *previous);
	void reloadEnabledCategoriesList();
	void reloadAvailableCategoriesList();
	void saveEnabledCategoriesList();
	void removeButton();
	void addButton();
	/*
	void selectCategory( QListWidgetItem * category);
	void deselectCategory( QListWidgetItem * category);
	*/
    Q_SIGNALS:
	void changed(bool);

    public:
            void loadCategory();
            void saveCategory( QString name = QString());
	    void createCategory(const QString & name);
	    void removeCategory(const QString & name);
	    //void addCategory();
    private Q_SLOTS:
	void syncEnabledCategoriesList();
    private:
	enum CategoriItemRole { EnabledRole = Qt::UserRole + 1 };
	Nepomuk::WebExtractorConfig* m_config;
	QHash< QString, Nepomuk::WebExCategoryConfig *> m_categories;
	QSet< QString > m_enabledCategories;
	QAbstractItemDelegate * m_oldDelegate;
	CategoryPluginItemDelegate * m_newDelegate;
	bool m_categoryEdited;
	QStandardItemModel * m_selectedPlugins;
	QStandardItemModel * m_availablePlugins;
	PluginSelector * plugins_selector;
	QStateMachine * m_machine;
	QState * s1;
	QState * s2;
	QState * s3;

};

#endif
