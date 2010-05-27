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
#include "kcm_config.h"
#include <KMessageBox>
#include <QSet>
#include <QStringList>
#include "pluginItemDelegate.h"


CategoriesPage::CategoriesPage(Nepomuk::WebExConfigBase* cfg,QWidget * parent):
    QWidget(parent),
    m_config(cfg),
    m_categoryEdited(false)
{
    this->setupUi(this);
    this->query_edit->hide();
    this->query_prefix_edit->hide();
    //new CategoryPluginItemDelegate(this->plugins_selector->selectedListWidget());
    connect( this->query_edit,SIGNAL(textChanged()), this, SLOT(setCategoryChanged()));
    connect( this->query_prefix_edit,SIGNAL(textChanged()), this, SLOT(setCategoryChanged()));
    connect( this->interval_spinbox,SIGNAL(valueChanged(int)), this, SLOT(setCategoryChanged()));
    connect( this->plugins_selector,SIGNAL(added(QListWidgetItem*)), this, SLOT(setCategoryChanged()));
    connect( this->plugins_selector,SIGNAL(removed(QListWidgetItem*)), this, SLOT(setCategoryChanged()));
    connect( this->plugins_selector,SIGNAL(movedUp(QListWidgetItem*)), this, SLOT(setCategoryChanged()));
    connect( this->plugins_selector,SIGNAL(movedDown(QListWidgetItem*)), this, SLOT(setCategoryChanged()));
    connect(enabled_categories_listwidget, SIGNAL(currentItemChanged(QListWidgetItem*, QListWidgetItem*)),
                    SLOT(switchCategory(QListWidgetItem*, QListWidgetItem*)));
}

CategoriesPage::~CategoriesPage()
{;}

void CategoriesPage::load()
{
    reloadCategoriesList();
}

void CategoriesPage::save()
{
    saveCategory();
    saveCategoriesList();
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
    Nepomuk::WebExCategory * cat = m_categories[catname];
    if (!cat) {
	kDebug() << "No category associated with this name. Report a bug please";
	return;
    }
    plugins_selector->selectedListWidget()->clear();
    plugins_selector->availableListWidget()->clear();
    query_edit->setPlainText( cat->query() );
    query_prefix_edit->setPlainText( cat->queryPrefix() );
    interval_spinbox->setValue( cat->interval() );
    //switch ( cat->
    // Add plugins
    const QStringList & availablePlugins = Plugins::plugins();
    KConfig * catconfig = cat->config();
    QSet< QString> enabledPlugins = catconfig->groupList().toSet();
    enabledPlugins.remove(CATEGORY_CONFIG_GROUP);
    QMultiMap<double,QListWidgetItem*> enabledItems, availableItems;
    foreach(const QString plg, availablePlugins)
    {
	QListWidgetItem * w = new QListWidgetItem(plg);
	KConfigGroup pg = catconfig->group(plg);
	double coff = pg.readEntry("coff",1.0);
	double rank = pg.readEntry("rank",1.0);
	w->setData(Qt::UserRole,QVariant(coff));
	if (enabledPlugins.contains(plg))
	    enabledItems.insert(rank,w);
	else
	    availableItems.insert(rank,w);
    }

    foreach(QListWidgetItem * w, enabledItems)
    {
		plugins_selector->selectedListWidget()->addItem(w);
    }

    foreach(QListWidgetItem * w, availableItems)
    {
		plugins_selector->availableListWidget()->addItem(w);
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
    Nepomuk::WebExCategory * cat = m_categories[name];
    if (!cat) {
	kDebug() << "No category associated with this name. Report a bug please";
	return;
    }
    cat->setInterval(interval_spinbox->value());
    cat->setQuery( query_edit->toPlainText());
    cat->setQueryPrefix( query_prefix_edit->toPlainText());
    KConfig * catconfig = cat->config();
    int stp = plugins_selector->selectedListWidget()->count();
    QListWidgetItem * w;

    // Remove other plugins
    QStringList  groups = catconfig->groupList();
    foreach( const QString & groupName, groups)
    {
	// If it is not built in
	if (groupName != CATEGORY_CONFIG_GROUP)
	    catconfig->deleteGroup(groupName);
    }

    for ( int i = 0; i < stp; i++)
    {
	w = plugins_selector->selectedListWidget()->item(i);
	QString pluginName = w->text();
	KConfigGroup pluginConfigGroup = catconfig->group(pluginName);
	pluginConfigGroup.writeEntry("rank", (double(stp) - i)/stp );
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
        } else if (!m_config->categories().contains(previous->text())) {
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
                       this, SLOT(switchProfile(QListWidgetItem*, QListWidgetItem*)));
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

    Nepomuk::WebExCategory * category = new Nepomuk::WebExCategory( KSharedConfig::openConfig("webextractor/categories/"+name+"rc"));
    category->writeConfig();

    m_categories.insert(name,category);

    saveCategoriesList();
    reloadCategoriesList();

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
    saveCategoriesList();
    reloadCategoriesList();
}

void CategoriesPage::reloadCategoriesList()
{
    const QStringList & categories = m_config->categories();
    
    foreach( Nepomuk::WebExCategory * c, m_categories)
    {
	delete c;
    }

    m_categories.clear();
    
    foreach( const QString & cat, categories)
    {
	KSharedConfigPtr ptr = 
		    KSharedConfig::openConfig(
			QString("webextractor/categories/") + cat +"rc"
			);
	// Check that config file exists
	if (!ptr.isNull()) {
	    this->enabled_categories_listwidget->addItem(cat);
	    m_categories.insert(
		    cat, 
		    new Nepomuk::WebExCategory(ptr)
		    );
	}

    }
}

void CategoriesPage::saveCategoriesList()
{
    QStringList lst = m_categories.keys();
    m_config->setCategories(lst);
}

void CategoriesPage::removeButton()
{
    if ( enabled_categories_listwidget->currentItem() )
	removeCategory(enabled_categories_listwidget->currentItem()->text());
}

void CategoriesPage::addButton()
{
}
