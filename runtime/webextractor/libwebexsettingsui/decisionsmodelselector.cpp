/*
   Copyright (C) 2010 by Serebriyskiy Artem <v.for.vandal at gmail.com>

   This library is free software; you can redistribute it and/or modify
   it under the terms of the GNU Library General Public License as published by
   the Free Software Foundation; either version 3, or (at your option)
   any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */


#include "decisionsmodelselector.h"
#include <Soprano/Global>

DecisionsModelSelector::DecisionsModelSelector( QWidget * parent):
    QWidget(parent)
{
    this->setupUi(this);
    this->userSettingsGroupBox->setEnabled(false);
    updateSchemeList();
    connect( this->decisionsModelComboBox, SIGNAL(currentIndexChanged(int)), this, SIGNAL(valueChanged()));
}

void DecisionsModelSelector::updateSchemeList()
{
    this->decisionsModelComboBox->clear();
    bool enableAll = this->enableAllSchemesCheckBox->isChecked();
    // For each named scheme ( virtuoso, redland ) check that it is 
    // available
    this->decisionsModelComboBox->addItem("Auto",QVariant(int(Nepomuk::WebExConfigBase::EnumDecisionsModelScheme::Auto)));
    this->decisionsModelComboBox->addItem("User",QVariant(int(Nepomuk::WebExConfigBase::EnumDecisionsModelScheme::User)));
     
    // Virtuoso
    // TODO Found method that checks backend aviability without loading it 
    if ( Soprano::discoverBackendByName("virtuoso") || enableAll ) {
	// We found it.
	this->decisionsModelComboBox->addItem("Virtuoso", QVariant(int(Nepomuk::WebExConfigBase::EnumDecisionsModelScheme::Virtuoso)));
    }
    // Redland
    if ( Soprano::discoverBackendByName("redland") || enableAll ) {
	// We found it.
	this->decisionsModelComboBox->addItem("Redland", QVariant(int(Nepomuk::WebExConfigBase::EnumDecisionsModelScheme::Redland)));
    }

}

void DecisionsModelSelector::setScheme( Nepomuk::WebExConfigBase::EnumDecisionsModelScheme::type scheme)
{
    // Set current item to given scheme. If there is no such scheme available -
    // for example some backend was removed, then reset to auto
    int index = decisionsModelComboBox->findData(QVariant(int(scheme)));
    if ( index == -1 ) { // Reset to auto scheme
	decisionsModelComboBox->setCurrentIndex(
		decisionsModelComboBox->findData(QVariant(int(
			    Nepomuk::WebExConfigBase::EnumDecisionsModelScheme::Auto
			    )))
		);
    }
    else {
	decisionsModelComboBox->setCurrentIndex(index);
    }

}

Nepomuk::WebExConfigBase::EnumDecisionsModelScheme::type DecisionsModelSelector::scheme() const
{
    return static_cast<Nepomuk::WebExConfigBase::EnumDecisionsModelScheme::type>(
	    decisionsModelComboBox->itemData(
		decisionsModelComboBox->currentIndex(),
		Qt::UserRole
		).toInt()
	    );
}
