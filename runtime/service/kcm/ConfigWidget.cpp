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
#include "ConfigWidget.h"
#include "categorieswidget.h"

ConfigWidget::ConfigWidget(Nepomuk::WebExtractorConfig* cfg,QWidget *parent)
        : KPageWidget(parent),
          m_generalPage(new GeneralPage(cfg,this)),
          m_categoriesPage(new CategoriesWidget(this))
{
    //layout()->setMargin(0);

    //m_generalPage->layout()->setMargin(0);
    
    KPageWidgetItem *general = addPage(m_generalPage, i18n("General Settings"));
    KPageWidgetItem *categories = addPage(m_categoriesPage, i18n("Categories Settings"));
    general->setHeader("");
    general->setIcon(KIcon("configure"));
    categories->setHeader("");
    categories->setIcon(KIcon("configure"));

    connect(m_generalPage, SIGNAL(changed(bool)), this, SIGNAL(changed(bool)));
    connect(m_categoriesPage, SIGNAL(changed(bool)), this, SIGNAL(changed(bool)));
}

ConfigWidget::~ConfigWidget()
{;}

void ConfigWidget::load()
{
    m_generalPage->load();
    //m_categoriesPage->load();
}
void ConfigWidget::save()
{
    m_generalPage->save();
    //m_categoriesPage->save();
}
void ConfigWidget::defaults()
{
    m_generalPage->defaults();
    //m_categoriesPage->defaults();
}
