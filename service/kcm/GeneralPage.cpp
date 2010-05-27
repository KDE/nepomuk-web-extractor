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
#include "GeneralPage.h"

GeneralPage::GeneralPage(Nepomuk::WebExConfigBase* config, QWidget * parent):
    QWidget(parent),
    m_config(config)
{
    this->setupUi(this);
    this->advanced_widget->hide();
}

GeneralPage::~GeneralPage()
{;}

void GeneralPage::load()
{

    QSpinBox * boxes[] = {
	kcfg_maxCategoriesSimult, 
	kcfg_maxResSimult, 
	kcfg_maxResSimultPerCategory
	    };
    KConfigSkeletonItem * items[] = {
	m_config->maxCategoriesSimultItem(),
	m_config->maxResSimultItem(),
	m_config->maxResSimultPerCategoryItem()
	    };
    QSpinBox * w;
    KConfigSkeletonItem * item;
    for ( int i = 0; i < 3; i++)
    {
	w = boxes[i];
	item = items[i];

	w->setToolTip( item->toolTip());
	w->setWhatsThis( item->whatsThis());
	w->setMinimum( item->minValue().toUInt());
	w->setMaximum( item->maxValue().toUInt());
	connect(w,SIGNAL(valueChanged(int)), this, SLOT(emitChanged()));
    }

    /* max categories simult */
    this->kcfg_maxCategoriesSimult->setValue( m_config->maxCategoriesSimult());

    /* max res simult per category */
    this->kcfg_maxResSimultPerCategory->setValue( m_config->maxResSimultPerCategory());

    /* max res simult */
    this->kcfg_maxResSimult->setValue( m_config->maxResSimult());
}
void GeneralPage::save()
{
    m_config->setMaxCategoriesSimult(this->kcfg_maxCategoriesSimult->value());
    m_config->setMaxResSimultPerCategory(this->kcfg_maxResSimultPerCategory->value( ));
    m_config->setMaxResSimult(this->kcfg_maxResSimult->value( ));
}
void GeneralPage::defaults()
{
}

void GeneralPage::emitChanged()
{
    emit changed(true);
}
