/*
   Copyright (C) 2010 by Serebriyskiy Artem <v.for.vandal at gmail.com>

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

#include "sourceWPage.h"
#include "settings_config.h"
#include "webexsettings_global.h"

#include <KService>
#include <KDebug>

#include <QListWidgetItem>

SourceWPage::SourceWPage(QWidget * parent):
    QWizardPage(parent)
{
    this->setupUi(this);
    connect(pluginWidget, 
            SIGNAL(currentItemChanged( QListWidgetItem *, QListWidgetItem *)),
            this,
            SLOT(onSelectedPluginChanged(QListWidgetItem *, QListWidgetItem *))
           );
    registerField("source",this,"source");
}

void SourceWPage::initializePage()
{
    this->pluginWidget->clear();
    KService::List plugins = Nepomuk::GlobalSettings::availablePlugins();
    for ( 
            KService::List::const_iterator it = plugins.begin();
            it != plugins.end();
            ++it
        )
    {
        KService::Ptr s = *it;
        QListWidgetItem * item = new QListWidgetItem( 
                s->property("Name", QVariant::String).toString()
                );
        item->setData(
                Qt::UserRole + 1,
                s->property(WE_PLUGIN_NAME_KEY,QVariant::String)
                );
        item->setData(
                Qt::UserRole + 2,
                s->property("Comment",QVariant::String)
                );


        this->pluginWidget->addItem(item);
    }


}

bool SourceWPage::isComplete() const
{
    return ( pluginWidget->currentItem()!=0 and !m_source.isEmpty() );
}

void SourceWPage::onSelectedPluginChanged(QListWidgetItem * current, QListWidgetItem * previous)
{
    if (current) {
        m_source = current->data(Qt::UserRole+1).toString();
        // Set description
        this->pluginComment->setText(current->data(Qt::UserRole+2).toString());
    }
    emit completeChanged();
}

QString SourceWPage::source() const
{
    return m_source;
}

void SourceWPage::setSource( const QString & value)
{
    this->m_source = value;
}
