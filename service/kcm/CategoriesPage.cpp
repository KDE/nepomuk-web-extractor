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
#include "CategoriesPage.h"
#include "plugins.h"
#include "categories.h"
#include "settings_config.h"
#include <KMessageBox>
#include <QSet>
#include <QStringList>
#include <QStandardItem>


CategoriesPage::CategoriesPage(Nepomuk::WebExtractorConfig* cfg,QWidget * parent):
    QWidget(parent),
    m_config(cfg),
    m_categoryEdited(false)
{
    this->setupUi(this);
    this->query_edit->hide();
    this->query_prefix_edit->hide();
    this->plugins_selector = new PluginSelector(this);
    this->verticalLayout->insertWidget(0,this->plugins_selector);
    m_oldDelegate = this->plugins_selector->selectedView()->itemDelegate();
    //m_newDelegate = new CategoryPluginItemDelegate(this->plugins_selector->selectedView(),this);
    //this->plugins_selector->selectedView()->setItemDelegate(m_newDelegate);
    //this->plugins_selector->selectedListWidget()->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    connect( this->query_edit,SIGNAL(textChanged()), this, SLOT(setCategoryChanged()));
    connect( this->query_prefix_edit,SIGNAL(textChanged()), this, SLOT(setCategoryChanged()));
    connect( this->interval_spinbox,SIGNAL(valueChanged(int)), this, SLOT(setCategoryChanged()));
    connect( this->plugins_selector,SIGNAL(added()), this, SLOT(setCategoryChanged()));
    connect( this->plugins_selector,SIGNAL(removed()), this, SLOT(setCategoryChanged()));
    connect( this->plugins_selector,SIGNAL(movedUp()), this, SLOT(setCategoryChanged()));
    connect( this->plugins_selector,SIGNAL(movedDown()), this, SLOT(setCategoryChanged()));
    connect(enabled_categories_listwidget, SIGNAL(currentItemChanged(QListWidgetItem*, QListWidgetItem*)),
                    SLOT(switchCategory(QListWidgetItem*, QListWidgetItem*)));

    // Initialize machine
    m_machine = new QStateMachine();
    s1 = new QState();
    s2 = new QState();
    s3 = new QState();

    s1->addTransition(manageButton, SIGNAL(clicked()), s2);
    s2->addTransition(returnButton, SIGNAL(clicked()), s1);

    m_machine->addState(s1);
    m_machine->addState(s2);
    m_machine->setInitialState(s1);

    connect(s1, SIGNAL(entered()), this, SLOT(reloadEnabledCategoriesList()));
    connect(s2, SIGNAL(exited()), this, SLOT(syncEnabledCategoriesList()));
    s1->assignProperty(stackedWidget, "currentIndex", 0);
    s2->assignProperty(stackedWidget, "currentIndex", 1);


}

CategoriesPage::~CategoriesPage()
{
    
    //this->plugins_selector->selectedView()->setItemDelegate(m_oldDelegate);
    //delete m_newDelegate;
    
}

void CategoriesPage::load()
{
    m_enabledCategories = m_config->categories().toSet();
    reloadEnabledCategoriesList();
    reloadAvailableCategoriesList();
    if (!m_machine->isRunning())
        m_machine->start();
}

void CategoriesPage::save()
{
    saveCategory();
    saveEnabledCategoriesList();
}
void CategoriesPage::defaults()
{
}
void CategoriesPage::setCategoryChanged()
{
    m_categoryEdited = true;
    emitChanged();
}

void CategoriesPage::loadCategory()
{

    if (!enabled_categories_listwidget->currentItem())
        return;

    QString catname = enabled_categories_listwidget->currentItem()->text();
    kDebug() << "Loading a category" << catname;
    Nepomuk::WebExCategoryConfig * cat = m_categories[catname];
    if (!cat) {
	kDebug() << "No category associated with this name. Report a bug please";
	return;
    }
    plugins_selector->clear();
    query_edit->setPlainText( cat->query() );
    query_prefix_edit->setPlainText( cat->queryPrefix() );
    interval_spinbox->setValue( cat->interval() );

    // Add plugins
    const QStringList & availablePlugins = Plugins::plugins();
    QMap< QString, DataPPDescr> enabledPlugins = cat->plugins();

    foreach(const QString plg, availablePlugins)
    {
	kDebug() << "Adding plugin " << plg;
	if (enabledPlugins.contains(plg)) {
	    plugins_selector->addPlugin(enabledPlugins[plg],QString(),true);
	}
	else {
	    plugins_selector->addPlugin(DataPPDescr(plg),QString(),false);
	}
    }


    m_categoryEdited = false;
}

void CategoriesPage::saveCategory(QString  p)
{
    if (!enabled_categories_listwidget->currentItem() && p.isEmpty()) {
        return;
    }
    QString name;
    if (p.isEmpty()) {
        name = enabled_categories_listwidget->currentItem()->text();
    } else {
        name = p;
    }
    Nepomuk::WebExCategoryConfig * cat = m_categories[name];
    if (!cat) {
	kDebug() << "No category associated with this name. Report a bug please";
	return;
    }
    cat->setInterval(interval_spinbox->value());
    cat->setQuery( query_edit->toPlainText());
    cat->setQueryPrefix( query_prefix_edit->toPlainText());

    cat->clearPluginList();
    foreach( const DataPPDescr & dppd, this->plugins_selector->selectedPlugins() )
    {
	cat->addPlugin(dppd);
    }


    cat->writeConfig();
}
void CategoriesPage::switchCategory(QListWidgetItem *current, QListWidgetItem *previous)
{
    Q_UNUSED(current)

    if (!m_categoryEdited) {
        loadCategory();
    } else {
        if (!previous) {
             // Pass by, the profile has probably been deleted
            loadCategory();
            return;
        } else if (!m_enabledCategories.contains(previous->text())) {
            // Pass by, the profile has probably been deleted
            loadCategory();
            return;
        }

        int result = KMessageBox::warningYesNoCancel(this, i18n("The current category has not been saved.\n"
                     "Do you want to save it?"), i18n("Save Profile"));

        if (result == KMessageBox::Yes) {
            saveCategory(previous->text());
            loadCategory();
        } else if (result == KMessageBox::No) {
            loadCategory();
        } else if (result == KMessageBox::Cancel) {
            disconnect(enabled_categories_listwidget, SIGNAL(currentItemChanged(QListWidgetItem*, QListWidgetItem*)),
                       this, SLOT(switchCategory(QListWidgetItem*, QListWidgetItem*)));
            enabled_categories_listwidget->setCurrentItem(previous);
            connect(enabled_categories_listwidget, SIGNAL(currentItemChanged(QListWidgetItem*, QListWidgetItem*)),
                    SLOT(switchCategory(QListWidgetItem*, QListWidgetItem*)));
        }
    }
}
void CategoriesPage::emitChanged()
{
    emit changed(true);
}

void CategoriesPage::createCategory( const QString & name)
{
    if (name.isEmpty())
        return;

    if (m_categories.contains(name)) {
	KMessageBox::sorry(this,i18n("This category already exists"));
	return;
    }

    Nepomuk::WebExCategoryConfig * category = new Nepomuk::WebExCategoryConfig( name);
    category->writeConfig();

    m_categories.insert(name,category);

    saveEnabledCategoriesList();
    reloadEnabledCategoriesList();

}

void CategoriesPage::removeCategory( const QString & name)
{
    if (name.isEmpty())
        return;

    if (!m_categories.contains(name)) {
	KMessageBox::error(this,i18n("This category has been already removed"));
	return;
    }

    delete m_categories[name];
    m_categories.remove(name);
    saveEnabledCategoriesList();
    reloadEnabledCategoriesList();
}

void CategoriesPage::reloadEnabledCategoriesList()
{
    // Create set of already enabled categories
    
    /*
    QSet< QString > enabledCategories;
    for ( int i = 0; i < enabled_categories_listwidget->count(); i++)
    {
	QListWidgetItem * item = enabled_categories_listwidget->item(i);
	if ( item->data(EnabledRole).value<bool>() ) {
	    enabledCategories.insert(item->text());
	}
    }
    */

    disconnect(enabled_categories_listwidget, SIGNAL(currentItemChanged(QListWidgetItem*, QListWidgetItem*)),
	       this, SLOT(switchCategory(QListWidgetItem*, QListWidgetItem*)));
    this->enabled_categories_listwidget->clear();
    for( 
	    QHash< QString, Nepomuk::WebExCategoryConfig*>::iterator it = m_categories.begin();
	    it != m_categories.end();
	    it++
       )
    {
	if ( !m_enabledCategories.contains(it.key()) ) {
		delete it.value();
	    it = m_categories.erase(it);
	}

    }

    //const QStringList & categories = Categories::categories();
    

    
    foreach( const QString & cat, m_enabledCategories)
    {
	QListWidgetItem * item = new QListWidgetItem(cat);

	/*
	if (enabledCategories.contains(cat) ) {
	    item->setData(EnabledRole,true);
	}
	else {
	    item->setData( EnabledRole, false);
	}
	*/

	this->enabled_categories_listwidget->addItem(item);

	if (!m_categories.contains(cat)) {
	    m_categories.insert(
		    cat, 
		    new Nepomuk::WebExCategoryConfig(cat)
		    );
	}
	    
    }
    connect(enabled_categories_listwidget, SIGNAL(currentItemChanged(QListWidgetItem*, QListWidgetItem*)),
	    SLOT(switchCategory(QListWidgetItem*, QListWidgetItem*)));
}

void CategoriesPage::saveEnabledCategoriesList()
{
    /*
    QSet< QString > enabledCategories;
    for ( int i = 0; i < enabled_categories_listwidget->count(); i++)
    {
	QListWidgetItem * item = enabled_categories_listwidget->item(i);
	if ( item->data(EnabledRole).value<bool>() ) {
	    enabledCategories.insert(item->text());
	}
    }
    */
    // Select only enabled categories
    QStringList lst = m_enabledCategories.toList();
    m_config->setCategories(lst);
}

void CategoriesPage::reloadAvailableCategoriesList()
{
    this->category_selector->selectedListWidget()->clear();
    this->category_selector->availableListWidget()->clear();

    // Fill lists
    QStringList categories = Categories::categories();
    foreach( const QString & cat, categories) 
    {
	// If enabled add to selected
	if ( m_enabledCategories.contains(cat) ) {
	    category_selector->selectedListWidget()->addItem(cat);
	}
	else {
	    category_selector->availableListWidget()->addItem(cat);
	}
    }
}

void CategoriesPage::removeButton()
{
    if ( enabled_categories_listwidget->currentItem() )
	removeCategory(enabled_categories_listwidget->currentItem()->text());
}

void CategoriesPage::addButton()
{
}

void CategoriesPage::syncEnabledCategoriesList()
{
    m_enabledCategories.clear();
    int stp = category_selector->selectedListWidget()->count();
    for ( int i = 0; i < stp; i++ )
    {
	m_enabledCategories.insert( 
		category_selector->selectedListWidget()->item(i)->text()
		);
    }	
}
/*
void CategoriesPage::selectCategory( QListWidgetItem * category)
{
    QString catname = category->text();
    m_enabledCategories.insert(catname);
}

void CategoriesPage::deselectCategory( QListWidgetItem * category)
{
}
*/
