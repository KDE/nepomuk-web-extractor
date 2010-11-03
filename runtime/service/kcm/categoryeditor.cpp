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

#include <KComboBox>
#include <KPushButton>
#include <KDialog>

#include <Nepomuk/Query/FileQuery>
#include <Nepomuk/Query/ResourceTypeTerm>
#include <Nepomuk/Vocabulary/NFO>

Q_DECLARE_METATYPE(Nepomuk::Query::Query)

using namespace Nepomuk::Query;

CategoryEditor::CategoryEditor(QWidget *parent)
    : QWidget(parent)
{
    setupUi(this);

    m_buttonCustomQuery->setIcon(KIcon(QLatin1String("configure")));
    connect(m_buttonCustomQuery, SIGNAL(clicked()),
            this, SLOT(slotCustomQuery()));

    m_buttonAddPlugin->setIcon(KIcon(QLatin1String("list-add")));
    m_buttonRemovePlugin->setIcon(KIcon(QLatin1String("list-remove")));

    m_pluginModel = new PluginModel(this);
    m_viewPlugins->setModel(m_pluginModel);

    // add the default queries
    selectQuery(i18nc("@item:inlistbox Description of a desktop query", "All Images"),
             FileQuery(ResourceTypeTerm(Nepomuk::Vocabulary::NFO::Image())));
    selectQuery(i18nc("@item:inlistbox Description of a desktop query", "All Video Files"),
             FileQuery(ResourceTypeTerm(Nepomuk::Vocabulary::NFO::Video())));
    selectQuery(i18nc("@item:inlistbox Description of a desktop query", "All Audio Files"),
             FileQuery(ResourceTypeTerm(Nepomuk::Vocabulary::NFO::Audio())));
}

void CategoryEditor::loadCategory(const Category &cat)
{
    m_loadedCategory = cat;

    m_editName->setText(cat.name());
    selectQuery(cat.queryDescription(), cat.query());

    const QList<DataPPDescr> plugins = cat.plugins();
    m_pluginModel->setPlugins(plugins);
}

Category CategoryEditor::category() const
{
    Category cat = m_loadedCategory;
    cat.setName(m_editName->text());
    cat.setQuery(m_comboQuery->itemData(m_comboQuery->currentIndex()).value<Query>());
    cat.setQueryDescription(m_comboQuery->itemText(m_comboQuery->currentIndex()));

    // FIXME: save plugins

    return cat;
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
}

Category CategoryEditor::editCategory(QWidget *parent, const Category &cat)
{
    KDialog dlg;
    dlg.setButtons(KDialog::Ok|KDialog::Cancel);
    dlg.setCaption(i18n("Edit Category"));
    CategoryEditor* ce = new CategoryEditor(&dlg);
    dlg.setMainWidget(ce);
    ce->loadCategory(cat);
    if( dlg.exec() ) {
        return ce->category();
    }
    else {
        return cat;
    }
}

Category CategoryEditor::createCaterory(QWidget *parent)
{
    return editCategory(parent, Category());
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
