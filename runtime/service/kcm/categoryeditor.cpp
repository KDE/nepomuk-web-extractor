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

#include "categoryeditor.h"
#include "queryeditor.h"
#include "pluginmodel.h"
#include "categoriespool.h"
#include <KComboBox>
#include <KPushButton>
#include <KDialog>
#include <KService>
#include <KServiceTypeTrader>
#include <KDebug>

#include <QtGui/QMenu>

#include <Nepomuk/Query/FileQuery>
#include <Nepomuk/Query/ResourceTypeTerm>
#include <Nepomuk/Vocabulary/NFO>

#include <KColorScheme>

Q_DECLARE_METATYPE(Nepomuk::Query::Query)
Q_DECLARE_METATYPE(KService::Ptr)

using namespace Nepomuk::Query;

CategoryEditor::CategoryEditor(QWidget *parent)
    : KDialog(parent),
      m_addPluginMenu(0)
{
    setupUi(mainWidget());

    setButtons(KDialog::Ok|KDialog::Cancel);

    m_buttonCustomQuery->setIcon(KIcon(QLatin1String("configure")));
    connect(m_buttonCustomQuery, SIGNAL(clicked()),
            this, SLOT(slotCustomQuery()));

    m_buttonAddPlugin->setIcon(KIcon(QLatin1String("list-add")));
    m_buttonRemovePlugin->setIcon(KIcon(QLatin1String("list-remove")));

    m_pluginModel = new PluginModel(this);
    m_viewPlugins->setModel(m_pluginModel);
    connect(m_viewPlugins->selectionModel(), SIGNAL(selectionChanged(QItemSelection,QItemSelection)),
            this, SLOT(slotPluginSelectionChanged(QItemSelection,QItemSelection)));

    // add the available plugins as menu entries
    buildAddPluginMenu();

    // add the default queries
    selectQuery(i18nc("@item:inlistbox Description of a desktop query", "All Images"),
             FileQuery(ResourceTypeTerm(Nepomuk::Vocabulary::NFO::Image())));
    selectQuery(i18nc("@item:inlistbox Description of a desktop query", "All Video Files"),
             FileQuery(ResourceTypeTerm(Nepomuk::Vocabulary::NFO::Video())));
    selectQuery(i18nc("@item:inlistbox Description of a desktop query", "All Audio Files"),
             FileQuery(ResourceTypeTerm(Nepomuk::Vocabulary::NFO::Audio())));

    connect(m_editName, SIGNAL(textChanged(QString)), this, SLOT(slotChanged()));
    connect(m_pluginModel, SIGNAL(layoutChanged()), this, SLOT(slotChanged()));

    slotPluginSelectionChanged(QItemSelection(), QItemSelection());
}

void CategoryEditor::loadCategory(Category *cat)
{
    m_category = cat;

    m_editName->setText(cat->name());
    selectQuery(cat->queryDescription(), cat->query());
    m_pluginModel->setPlugins(cat->plugins());
}

void CategoryEditor::saveCategory(Category* cat)
{
    cat->setName(m_editName->text());
    cat->setQuery(m_comboQuery->itemData(m_comboQuery->currentIndex()).value<Query>());
    cat->setQueryDescription(m_comboQuery->itemText(m_comboQuery->currentIndex()));
    cat->setPlugins(m_pluginModel->plugins());
}

Category* CategoryEditor::category() const
{
    return m_category;
}

void CategoryEditor::slotCustomQuery()
{
    QueryEditor dlg(this);
    dlg.setQuery(m_comboQuery->itemData(m_comboQuery->currentIndex()).value<Query>());
    dlg.setTitle(m_comboQuery->itemText(m_comboQuery->currentIndex()));
    if(dlg.exec()) {
        selectQuery(dlg.title(), dlg.query());
    }
}

void CategoryEditor::selectQuery(const QString &title, const Nepomuk::Query::Query &query)
{
    if( query.isValid() && !title.isEmpty() && findQueryIndex(query) < 0) {
        m_comboQuery->addItem(title, QVariant::fromValue(query));
    }
    int i = findQueryIndex(query);
    if( i < 0 )
        i = 0; // default query: something
    m_comboQuery->setCurrentIndex(i);
    slotChanged();
}

void CategoryEditor::editCategory(QWidget *parent, Category *cat)
{
    CategoryEditor dlg;
    dlg.setCaption(i18n("Edit Category"));
    dlg.loadCategory(cat);
    // cannot rename global categories
    dlg.m_editName->setReadOnly(cat->isGlobal());
    if( dlg.exec() ) {
        dlg.saveCategory(cat);
    }
}

void CategoryEditor::createCaterory(QWidget *parent)
{
    CategoryEditor dlg;
    dlg.setCaption(i18n("Create new Category"));
    if( dlg.exec() ) {
        Category* cat = new Category();
        dlg.saveCategory(cat);
        Nepomuk::CategoriesPool::self()->addCategory(cat);
    }
}

int CategoryEditor::findQueryIndex(const Nepomuk::Query::Query &query)
{
    for(int i = 0; i < m_comboQuery->count(); ++i) {
        if(m_comboQuery->itemData(i).value<Query>() == query) {
            return i;
        }
    }
    return -1;
}

void CategoryEditor::slotPluginSelectionChanged(const QItemSelection& selected, const QItemSelection&)
{
    m_buttonRemovePlugin->setEnabled(!selected.isEmpty());
}

void CategoryEditor::buildAddPluginMenu()
{
    if(!m_addPluginMenu) {
        m_addPluginMenu = new QMenu(m_buttonAddPlugin);
        m_buttonAddPlugin->setMenu(m_addPluginMenu);
    }

    m_addPluginMenu->clear();

    // TODO: use a central mangement class to get all available plugins
    KService::List services = KServiceTypeTrader::self()->query(QLatin1String("Nepomuk/WebExtractorPlugin"));
    foreach (const KService::Ptr& service, services) {
        kDebug() << "read datapp" << service->name();
        QAction* a = m_addPluginMenu->addAction(service->name());
        a->setData(QVariant::fromValue(service));
        connect(a, SIGNAL(triggered()),
                this, SLOT(slotAddPluginActionTriggered()));
    }
}

void CategoryEditor::slotAddPluginActionTriggered()
{
    QAction* a = qobject_cast<QAction*>(sender());
    KService::Ptr service = a->data().value<KService::Ptr>();
    DataPPDescr datapp(service);
    m_pluginModel->addPlugin(datapp);
}

void CategoryEditor::slotChanged()
{
    // we have a valid name if either the name is the same as the loaded one (editing) or the name does not exist yet
    const bool haveValidName( (m_category && m_category->name() == m_editName->text()) || !Nepomuk::CategoriesPool::self()->category(m_editName->text()));

    enableButton(Ok, haveValidName && m_pluginModel->rowCount(QModelIndex()) > 0);

    if(haveValidName) {
        m_editName->setPalette(palette());
    }
    else {
        QPalette p(palette());
        KColorScheme::adjustBackground(p, KColorScheme::NegativeBackground);
        KColorScheme::adjustForeground(p, KColorScheme::NegativeText);
        m_editName->setPalette(p);
    }
}
