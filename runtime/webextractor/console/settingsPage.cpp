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

#include <KStandardGuiItem>
#include <KMessageBox>
#include <KDebug>
#include <KCModuleProxy>

#include "datapp.h"
#include "settingsPage.h"
#include "webextractor_kcm.h"
#include "datapppool.h"


namespace NW = Nepomuk::WebExtractor;

using namespace Nepomuk;

SettingsPage::SettingsPage(QWidget * parent):
    QWidget(parent),
    m_kcmChanged(false)
{
    this->setupUi(this);
    m_proxy = new KCModuleProxy("webextractorconfig");
    this->verticalLayout->insertWidget(0,m_proxy);
    connect(m_proxy,SIGNAL(changed(bool)),
            this,SLOT(dataPPSettingsChanged(bool))
           );
    this->kcmDialogButtonBox->addButton(KStandardGuiItem::help(),QDialogButtonBox::HelpRole);
    this->kcmDialogButtonBox->addButton(KStandardGuiItem::defaults(),QDialogButtonBox::ResetRole, this, SLOT(defaultSettings()));
    this->kcmDialogButtonBox->addButton(KStandardGuiItem::reset(),QDialogButtonBox::ResetRole, this, SLOT(resetSettings()));
    this->kcmDialogButtonBox->addButton(KStandardGuiItem::apply(),QDialogButtonBox::ApplyRole, this, SLOT(applySettings()));

}

void SettingsPage::applySettings()
{
    m_proxy->save();
}

void SettingsPage::resetSettings()
{
    m_proxy->load();
}

void SettingsPage::defaultSettings()
{
    m_proxy->defaults();
}


void SettingsPage::dataPPSettingsChanged(bool state)
{
    this->m_kcmChanged = state;

    // Enable buttons:

}

