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

#include "setupWPage.h"
#include "datapp.h"
#include "datappwizard.h"

#include <KDebug>


SetupWPage::SetupWPage(QWidget *parent  ):
    QWizardPage(parent)
{
    this->setupUi(this);
    // Register some fields
    registerField("name", nameEdit);
    registerField("description", descEdit);
    connect(nameEdit, SIGNAL(textChanged(const QString &)),
            this, SIGNAL(completeChanged())
           );
}

SetupWPage::~SetupWPage()
{
}

void SetupWPage::initializePage()
{
    Nepomuk::DataPP * dpp = qobject_cast<DataPPWizard*>(wizard())->m_result;
    QString source = field("source").toString();
    dpp->setSource(source);
    m_currentKcm = dpp->kcm(true);
    kDebug() << "Selected source: " << source;
    if (m_currentKcm) {
        this->kcmAreaLayout->insertWidget(1,m_currentKcm.data());
    }
}

void SetupWPage::cleanupPage()
{
    if (m_currentKcm) {
        this->kcmScrollAreaWidgetContents->layout()->removeWidget(m_currentKcm.data());
    }

}

bool SetupWPage::isComplete() const
{
    // Name must be non-empty and unique
    return !nameEdit->text().isEmpty(); 
}
