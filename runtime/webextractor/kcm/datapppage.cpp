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
#include "datapppage.h"
#include "datapppool.h"
#include "datapp.h"
#include "datappwizard.h"

#include <KMessageBox>

#include <QtCore/QDir>


using namespace Nepomuk;

Q_DECLARE_METATYPE(WebExtractorPluginKCM::Ptr);


DataPPPage::DataPPPage(QWidget * parent):
    QWidget(parent),
    m_currentKcm(0),
    m_kcmChanged(false)
{
    this->setupUi(this);
    this->datappView->setModel(Nepomuk::DataPPPool::self());
    connect(
            datappView->selectionModel(),
            SIGNAL(
                currentChanged(const QModelIndex &, const QModelIndex &)
                ),
            this,
            SLOT(
                onCurrentChanged(const QModelIndex&, const QModelIndex&)
                )
           );
    /* === Set properties of the DataPP Settings tab === */
#if 0
    this->kcmDialogButtonBox->addButton(KStandardGuiItem::help(),QDialogButtonBox::HelpRole);
    this->kcmDialogButtonBox->addButton(KStandardGuiItem::defaults(),QDialogButtonBox::ResetRole, this, SLOT(defaultSettings()));
    this->kcmDialogButtonBox->addButton(KStandardGuiItem::reset(),QDialogButtonBox::ResetRole, this, SLOT(resetSettings()));
    this->kcmDialogButtonBox->addButton(KStandardGuiItem::apply(),QDialogButtonBox::ApplyRole, this, SLOT(applySettings()));
#endif
    this->newButton->setGuiItem(KStandardGuiItem::Add);
    connect(this->newButton,SIGNAL(clicked()),
            this,SLOT(newDataPP())
           );

    this->removeButton->setGuiItem(KStandardGuiItem::Remove);
    connect(this->removeButton,SIGNAL(clicked()),
            this,SLOT(removeDataPP())
           );

}

DataPPPage::~DataPPPage()
{
    ;
}

void DataPPPage::load()
{

    this->datappView->clearSelection();
    DataPPPool::self()->update();
    QModelIndex currentIndex = this->datappView->currentIndex();
    if ( currentIndex.isValid() ) {
        this->previousDataPP = currentIndex;
        if ( isDataPP(currentIndex) ) {
            loadDataPP(currentIndex.data(DataPPPool::IdRole).toString());
        }
    }
}

void DataPPPage::save()
{
    if (m_currentKcm && m_kcmChanged) {
        m_currentKcm->save();
    }
}
void DataPPPage::defaults()
{
    load();
}

void DataPPPage::emitChanged()
{
    emit changed(true);
}

bool DataPPPage::isDataPP( const QModelIndex & index)
{
    return index.data(DataPPPool::TypeRole).toBool(); 
}

void DataPPPage::loadDataPP(const QString  & id)
{

    kDebug() << "Load DataPP. Id: " << id << "KCM? :" << !m_currentKcm.isNull();

    // Load KCM
    if (m_currentKcm) {
        kDebug() << "Disconnectind old kcm";
        disconnect(m_currentKcm.data(),SIGNAL(changed(bool)),this,SLOT(dataPPSettingsChanged(bool))); 
        this->kcmScrollAreaWidgetContents->layout()->removeWidget(m_currentKcm.data());
        m_currentKcm.data()->hide();
        this->kcmScrollAreaWidgetContents->show();
    }


    WebExtractorPluginKCM::Ptr kcm = DataPPPool::dataPPById(id)->kcm();
    if ( kcm ) {
            this->kcmAreaLayout->insertWidget(1,kcm.data());
            kcm.data()->show();
            connect(kcm.data(), SIGNAL(changed(bool)), this, SLOT(dataPPSettingsChanged(bool)));
    }
    else {
        // Do nothing
    }
    m_currentKcm = kcm;
    m_kcmChanged = false;
    QString source = DataPPPool::sourceById(id);
    this->sourceLabel->setText(source);
}

void DataPPPage::saveDataPP()
{
    if (m_currentKcm) {
        m_currentKcm->save();
    }
}

bool DataPPPage::switchDataPP(const QString & newId)
{
    // check changes
    if (m_currentKcm && m_kcmChanged) {
           // Show save/discard changes
            int result = KMessageBox::warningYesNoCancel(this, i18n("The current DataPP has not been saved.\n"
                         "Do you want to save it?"), i18n("Save DataPP"));

            if (result == KMessageBox::Yes) {
                m_currentKcm->save();
            } else if (result == KMessageBox::No) {
                // Do nothing
            } else if (result == KMessageBox::Cancel) {
                // Do nothing and simply return
                return false;
            }
    }

    // No switch
    loadDataPP(newId);
    return true;
}

void DataPPPage::onCurrentChanged(const QModelIndex& current, const QModelIndex& previous)
{
    if ( previousDataPP == current) // Do nothing
        return;

    // Check that current is DataPP
    if ( isDataPP(current) ) {
        if ( switchDataPP(current.data(DataPPPool::IdRole).toString() ) )
        {
            previousDataPP = current;
        }
        // Enable remove button
        this->removeButton->setEnabled(true);
    }
    else {
        this->removeButton->setEnabled(false);
    }

}

void DataPPPage::dataPPSettingsChanged(bool state)
{
    this->m_kcmChanged = state;
}

void DataPPPage::newDataPP()
{
    DataPPWizard * w = new DataPPWizard();
    int rv = w->exec();
    if ( rv == QDialog::Accepted ) {
        // Save common parameters
        w->result()->setDisplayName(w->field("name").toString());
        w->result()->setDescription(w->field("description").toString());

        // Save changes introduced by kcm
        Nepomuk::WebExtractorPluginKCM::Ptr kcm = w->kcm();
        if ( kcm ) 
            kcm->save();

        // Commit DataPP;
        w->result()->sync();
        DataPPPool::self()->update();
        kDebug() << *DataPPPool::self();
    }
    delete w;

}

void DataPPPage::removeDataPP()
{
    QModelIndex index = datappView->currentIndex();
    if (isDataPP(index)) {
        QString id = index.data(DataPPPool::IdRole).toString();
        // Remove file
        DataPP::removeDataPP(id);

        // Update DataPPPool
        DataPPPool::self()->update();
    }
}
