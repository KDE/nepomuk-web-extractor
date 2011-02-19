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

#include "datapp.h"
#include "settingsPage.h"
#include "webextractor_kcm.h"
#include "datapppool.h"


namespace NW = Nepomuk::WebExtractor;

using namespace Nepomuk;

SettingsPage::SettingsPage(QWidget * parent):
    QWidget(parent),
    m_currentKcm(0),
    m_kcmChanged(false)
{
    this->setupUi(this);
    this->dataPPView->setModel(Nepomuk::DataPPPool::self());

    /* === Set properties of the DataPP Settings tab === */
    this->kcmDialogButtonBox->addButton(KStandardGuiItem::help(),QDialogButtonBox::HelpRole);
    this->kcmDialogButtonBox->addButton(KStandardGuiItem::defaults(),QDialogButtonBox::ResetRole, this, SLOT(defaultSettings()));
    this->kcmDialogButtonBox->addButton(KStandardGuiItem::reset(),QDialogButtonBox::ResetRole, this, SLOT(resetSettings()));
    this->kcmDialogButtonBox->addButton(KStandardGuiItem::apply(),QDialogButtonBox::ApplyRole, this, SLOT(applySettings()));


}

void SettingsPage::applySettings()
{
    if ( m_currentKcm )
    m_currentKcm->save();
}

void SettingsPage::resetSettings()
{
    if ( m_currentKcm )
    m_currentKcm->load();
}

void SettingsPage::defaultSettings()
{
    if ( m_currentKcm )
    m_currentKcm->defaults();
}

void SettingsPage::dataPPClicked(QModelIndex index)
{
    kDebug() << "Enter";

    if(!index.data(DataPPPool::DataPPRole).toBool()) {
	// Do nothing and do not switch
	kDebug() << "Do nothing";
        return;
    }

    /* Get previous KCM. Ask for saving if necessary */
    if (m_currentKcm) { // If there is previous kcm
       if ( m_kcmChanged ) { // If there were changes
	   // Show save/discard changes
	    int result = KMessageBox::warningYesNoCancel(this, i18n("The current DataPP has not been saved.\n"
			 "Do you want to save it?"), i18n("Save Profile"));

	    if (result == KMessageBox::Yes) {
		applySettings();
	    } else if (result == KMessageBox::No) {
		// Do nothing
	    } else if (result == KMessageBox::Cancel) {
		// Do nothing and simply return
		return;
	    }

       }
       // Disconnect signals
       disconnect(m_currentKcm,SIGNAL(changed(bool)),this,SLOT(dataPPSettingsChanged(bool))); 
       // Delete widget from the model
	Q_ASSERT(this->kcmScrollAreaWidgetContents->layout());
	this->kcmScrollAreaWidgetContents->layout()->removeWidget(m_currentKcm);

       m_currentKcm = 0;
       
    }

    QString dataPPSource = index.data(DataPPPool::SourceRole).toString();
    if(dataPPSource.isEmpty()) {
        this->sourceNameLabel->setText("Invalid DataPP: Source not set");
	this->noKcmLabel->setHidden(true);
        return;
    } else {
	/* Get source and display it */
        this->sourceNameLabel->setText("Source: " + dataPPSource);

	/* Get KCM and display it */
	DataPP * dppcfg = new DataPP( index.data(DataPPPool::IdRole).toString() );
	//m_currentDataPP = dppcfg;
	WebExtractorPluginKCM * kcm = dppcfg->kcm();
	this->m_currentKcm = kcm;
	if ( kcm ) {
	    this->noKcmLabel->setHidden(true);
	    Q_ASSERT(this->kcmScrollAreaWidgetContents->layout());
	    this->kcmScrollAreaWidgetContents->layout()->addWidget(kcm);
	    connect(kcm, SIGNAL(changed(bool)), this, SLOT(dataPPSettingsChanged(bool)));
	}
	else {
	    this->noKcmLabel->setHidden(false);
	}

        return;
    }

}

void SettingsPage::dataPPSettingsChanged(bool state)
{
    this->m_kcmChanged = state;

    // Enable buttons:

}

