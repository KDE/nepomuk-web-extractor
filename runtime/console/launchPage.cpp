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

#include "launchPage.h"
#include <Soprano/QueryResultIterator>
#include <Soprano/Global>
#include <Soprano/BackendSettings>
#include <Soprano/Backend>
#include <KMessageBox>
#include <KStandardDirs>
#include <KTempDir>
#include "resourceanalyzer.h"
#include "resourceanalyzerfactory.h"
#include "resourceservicedata.h"
#include "decisionapplicationrequest.h"
#include "global.h"
#include "debug_datapp.h"
#include <kdebug.h>
#include <QUrl>
#include <stdint.h>
#include <QThread>

#include "datapppool.h"

namespace NW = Nepomuk::WebExtractor;
using namespace Nepomuk;
using namespace NW;

LaunchPage::LaunchPage(const QString & uri, const QStringList & datapps, bool autostart, QWidget * parent):
    QWidget(parent),
    workThread(0),
    m_tmpDir(0)
{
    this->setupUi(this);
    // Set properties of the DataPPView
    this->dataPPView->setModel(Nepomuk::DataPPPool::self());

    // Set Decisions widget
    connect(
	    this->decisionListWidget,
	    SIGNAL(currentItemChanged(const QListWidgetItem*,const QListWidgetItem*)), 
	    this,
	    SLOT(onCurrentDecisionChanged(const QListWidgetItem*,const QListWidgetItem*)));
    connect(this->applyDecisionButton, SIGNAL(clicked()), this, SLOT(onApplyDecision()));
    connect(this->identifyDecisionButton, SIGNAL(clicked()), this, SLOT(onIdentifyDecision()));
    connect(this->identifyMainButton, SIGNAL(clicked()), this, SLOT(onIdentifyMain()));
    connect(this->onlyMainCheckBox, SIGNAL(toggled(bool)), this, SLOT(updateIdentificationInfo()));

    // Init member variables
    
    kDebug() << *Nepomuk::DataPPPool::self();
    connect(this->startButton, SIGNAL(clicked()),
            this, SLOT(startExtracting()));
    workThread = new QThread();
    m_currentAnalyzer = 0;

    // Start extracting if necessary
    if(uri.size()) {
        this->uriLineEdit->setText(uri);
        this->selectTargets->setCurrentIndex(LineEditSelect);
    }

    if(datapps.size()) {
        QModelIndexList selected;
        foreach(const QString & dpp, datapps) {
            selected << DataPPPool::self()->match(QModelIndex(), DataPPPool::NameRole, dpp, -1);
        }
        kDebug() << "Finish searching";

        foreach(const QModelIndex & idx, selected) {
            dataPPView->selectionModel()->select(idx, QItemSelectionModel::Select);
        }
    }

    if(autostart) {
        QTimer::singleShot(0, this, SLOT(startExtracting()));
    }

    // Set buttons
    setButtonApply();
}
    
LaunchPage::~LaunchPage()
{
    workThread->quit();
    delete workThread;
}

void LaunchPage::onIdentifyDecision()
{
    QSharedPointer<NW::DecisionApplicationRequest> req = this->decisionListWidget->currentDecisionApplicationRequest();
    if(!req) {
        req = decisionListWidget->decisionApplicationRequest(decisionListWidget->currentRow(), Nepomuk::ResourceManager::instance()->mainModel());
        if(!req)  // In case decision was invalid
            return;
    }

    Q_ASSERT(req);

    req->identify();
    updateIdentificationInfo();
}

void LaunchPage::onIdentifyMain()
{
    QSharedPointer<NW::DecisionApplicationRequest> req = this->decisionListWidget->currentDecisionApplicationRequest();
    if(!req) {
        req = decisionListWidget->decisionApplicationRequest(decisionListWidget->currentRow(), Nepomuk::ResourceManager::instance()->mainModel());
        if(!req)  // In case decision was invalid
            return;
    }

    Q_ASSERT(req);

    req->identifyTargets();
    if(!req->isTargetsIdentified())
        KMessageBox::sorry(this, "Identification of main Decision's resources failed");
    updateIdentificationInfo();
}

void LaunchPage::onApplyDecision()
{
    QSharedPointer<NW::DecisionApplicationRequest> req = this->decisionListWidget->currentDecisionApplicationRequest();
    if(!req) {
        req = decisionListWidget->decisionApplicationRequest(decisionListWidget->currentRow(), Nepomuk::ResourceManager::instance()->mainModel());
        if(!req)  // In case decision was invalid
            return;
    }


    Q_ASSERT(req);

    /*
    NS::IdentificationRequest * req = des.identificationRequest();

    identReq->load();
    identReq->identifyAll();

    if (!identReq->done())
    return false;

    */
    if(!req->apply())
        KMessageBox::sorry(this, "Applying Decision failed");

    updateIdentificationInfo();
}

void LaunchPage::updateIdentificationInfo()
{
    NW::Decision dec = this->decisionListWidget->currentDecision();
    this->identificationTableWidget->clear();

    if(!dec.isValid())
        return;

    QSharedPointer<NW::DecisionApplicationRequest> req = this->decisionListWidget->currentDecisionApplicationRequest();
    if(req) {
        QHash<QUrl, QUrl> mcp; // mcp = Mappincg CoPy

        QSet<QUrl> uncp = req->unidentified(); // uncp = UNidentified CoPy
        kDebug() << "Unidentified: " << uncp;
        QSet<QUrl> targetResources = req->targetResources();


        Q_ASSERT(req);
        int currentRow = 0;
        if(
            /* User select to display only main resources*/
            this->onlyMainCheckBox->isChecked() or
            /* Only an attempt of identification of main resources
             * was performed
             */
            (uncp.isEmpty() and req->mappings().isEmpty())
        ) {
            mcp = req->targetsMappings();
            kDebug() << "Mappings: " << mcp;
            identificationTableWidget->setRowCount(targetResources.size());
            QSet<QUrl>::const_iterator mrit = targetResources.begin();
            QSet<QUrl>::const_iterator mrit_end = targetResources.end();
            for(; mrit != mrit_end; mrit++) {
                QTableWidgetItem * item = new QTableWidgetItem(mrit->toString());
                item->setFlags(Qt::ItemIsEnabled);
                QColor color;
                // If resource is not exist in mappings
                // then print it
                QHash<QUrl, QUrl>::const_iterator fit = mcp.find(*mrit);
                if(fit == mcp.end()) {
                    color = QColor(255, 0, 0);
                } else {
                    color = QColor(0, 255, 0);
                    // Add item to the second column
                    QTableWidgetItem * sitem = new QTableWidgetItem(fit.value().toString());
                    sitem->setFlags(Qt::ItemIsEnabled);
                    sitem->setBackground(QBrush(color));
                    identificationTableWidget->setItem(currentRow, 1, sitem);

                }
                item->setBackground(QBrush(color));
                identificationTableWidget->setItem(currentRow, 0, item);
                currentRow++;
            }
        } else {
            mcp = req->mappings();
            kDebug() << "Mappings: " << mcp;
            QSet<QUrl>::const_iterator uit ;
            QSet<QUrl>::const_iterator uit_end ;
            uit = uncp.begin();
            uit_end = uncp.end();

            identificationTableWidget->setRowCount(mcp.size() + uncp.size());
            for(; uit != uit_end; uit++) {
                QTableWidgetItem * item = new QTableWidgetItem(uit->toString());
                item->setFlags(Qt::ItemIsEnabled);
                QColor color = QColor(255, 0, 0);
                if(!targetResources.contains(*uit)) {
                    // Addjust color to make it more lighter
                    color = color.lighter();
                }

                item->setBackground(QBrush(color));
                identificationTableWidget->setItem(currentRow, 0, item);
                currentRow++;
            }

            QHash<QUrl, QUrl>::const_iterator it = mcp.begin();
            QHash<QUrl, QUrl>::const_iterator it_end = mcp.end();
            for(; it != it_end; it++) {
                kDebug() << it.key() << ":" << it.value();
                QTableWidgetItem * item = new QTableWidgetItem(it.key().toString());
                item->setFlags(Qt::ItemIsEnabled);
                item->setBackground(QBrush(QColor(0, 255, 0)));
                identificationTableWidget->setItem(currentRow, 0, item);

                item = new QTableWidgetItem(it.value().toString());
                item->setFlags(Qt::ItemIsEnabled);
                item->setBackground(QBrush(QColor(0, 255, 0)));
                identificationTableWidget->setItem(currentRow, 1, item);


                currentRow++;
            }
        }

    }
    identificationTableWidget->resizeColumnsToContents();
    return;
}

void LaunchPage::updateDecisionsInfo()
{

    // clear widgets with  Decisions and other data structures
    this->decisionWidget->clear();
    //this->decisionInformationWidget->setRowCount(0);
    this->decisionListWidget->clear();


    // If there is no analyzer, then return
    if(!m_currentAnalyzer) {
        kDebug() << "No curret analyzer";
        return;
    }


    NW::DecisionList lst(m_currentAnalyzer->decisions());
    kDebug() << "Number of the decisions: " << lst.size();
    decisionListWidget->addDecisionList(lst);
}

void LaunchPage::cleanAfterAnalyzing()
{

    this->decisionListWidget->clear();

    // First remove model data
    delete m_currentAnalyzer;
    m_currentAnalyzer = 0;
    delete m_tmpDir;
    m_tmpDir = 0;
}

void LaunchPage::startExtracting()
{
    if(workThread->isRunning()) {
        int answer = KMessageBox::questionYesNo(
                         this,
                         "Application is currently analyzing another resource. \
		Do you want to abort?",
                         "Abort?"
                     );
        if(answer == KMessageBox::Yes) {
            QMetaObject::invokeMethod(this->m_currentAnalyzer, "abort", Qt::QueuedConnection);
            extractingFinished();
        }
        return;
    }

    // Fist check what uri(s) we should parse
    AnalyzationType desiredType;
    Nepomuk::Resource res;
    switch ( selectTargets->currentIndex() ) 
    {
        case (LineEditSelect) : {
                       // Uri is taken from uriLineEdit
                        // that we have necessary uri
                        QString uriString = uriLineEdit->text();
                        if(uriString.isEmpty()) {
                            KMessageBox::sorry(this, "Please provied an uri to extract metadata for");
                            return;
                        }

                        // TODO Automaticaly fix some user errors like:
                        // missed nepomuk:/
                        // may be some others
                        QUrl uri(uriString);


                        // Check that there is a Resource with this uri
                        res = Nepomuk::Resource(uri);
                        if(!res.exists()) {
                            KMessageBox::sorry(this, "There is no resource with uri you give me. Sorry");
                            return;
                        }
                        desiredType = LaunchPage::Single;
                        
                       break;
                   }
        case ( GuiSelect ) : {
                                KMessageBox::sorry(this, "Not supported yet");
                                desiredType = LaunchPage::Set;
                                return;
                                 break;
                             }
        case (CategorySelect) : {
                                KMessageBox::sorry(this, "Not supported yet");
                                desiredType = LaunchPage::Query;
                                return;
                                 break;
                             }
        default : {
                        KMessageBox::sorry(this, "Unknown selection method");
                        return;
                         break;
                     }
    }





    // Now create a list of parameters
    NW::ExtractParameters p;
    p.setACrit(Nepomuk::WebExtractor::maxACrit());
    p.setUCrit(this->thresholdNumInput->value());

    // Add DataPP
    // TODO Currently system use view selection as list of all selected DataPP
    QModelIndexList selected = dataPPView->selectionModel()->selectedIndexes();
    bool hasAny = false;

    foreach(const QModelIndex & index, selected) {
        // If category then skip
        // FIXME Select all datapp from category if category is selected
        if(!index.data(DataPPPool::DataPPRole).toBool())
            continue;


        QString dataPPName = index.data(DataPPPool::NameRole).toString();

        NW::DataPP * dpp = DataPPConfig::dataPP(dataPPName);
        //FIXME Enable proper plugin selection back
        //kDebug() << "DataPP selection is disabled. DebugDataPP is used instead";
        //NW::DataPP * dpp = new NW::DebugDataPP();
        if(!dpp)
            continue;

        hasAny = true;

        NW::DataPPWrapper * dppw =  new NW::DataPPWrapper(dpp, dataPPName, 1.0, 1.0);
        p.addDataPP(dppw);
    }

    if(!hasAny) {
        // Warn user
        KMessageBox::sorry(this, "You forget to select DataPP. Or the DataPP you have selected are all invalid.\n  If you have selected category, then sorry - this feature is not supported yet");
        return;
    }

    // Set backend
    Soprano::BackendSettings settings;
    KTempDir * td = 0;
    if(this->backendComboBox->currentText() == QString("Redland")) {
        settings << Soprano::BackendOptionStorageMemory;
        p.setBackendName("redland");
    } else if(this->backendComboBox->currentText() == QString("Virtuoso")) {
        td = new KTempDir(KStandardDirs::locateLocal("tmp", "desmodel"));
        settings << Soprano::BackendSetting(Soprano::BackendOptionStorageDir, td->name());
        p.setBackendName("virtuoso");
        // If we use virtuoso backend, then we should clean temporaly created model(s)
        p.setAutoDeleteModelData(true);
    } else {
        kDebug() << "Unknown backend is selected. Use default one.";
    }


    p.setBackendSettings(settings);

    delete m_tmpDir;
    m_tmpDir = td;


    kDebug() << " Launch Resource Analyzer with folowing parameters: " << p;

    m_par = p;

    NW::ResourceAnalyzerFactory factory(p);

    NW::ResourceAnalyzer * resanal = factory.newAnalyzer(res);
    if(!resanal) {
        kError() << "ResourceAnalyzerFactory failed to create Analyzer";
        return;
    }

    kDebug() << "Create ResourceAnalyzer: " << uintptr_t(resanal);

    // Set other settings of analyzer
    NW::ResourceAnalyzer::AnalyzingPolitics ap;
    if(this->analyzingPoliticsComboBox->currentIndex() == 0)
        ap = NW::ResourceAnalyzer::SingleStep;
    else
        ap = NW::ResourceAnalyzer::Iterative;

    resanal->setAnalyzingPolitics(ap);

    // delete previous analyzes
    cleanAfterAnalyzing();

    // Set this analyzer as current
    m_currentAnalyzer = resanal;
    m_currentAnalizationType = desiredType;


    resanal->setParent(0);
    resanal->moveToThread(workThread);
    connect(workThread, SIGNAL(started()), resanal, SLOT(analyze()));
    connect(resanal, SIGNAL(analyzingFinished()), this, SLOT(extractingFinished()), Qt::QueuedConnection);
    workThread->start();




    // Change button icon
    setButtonAbort();

}

void LaunchPage::extractingFinished()
{
    // 2 possible ways. If we analyze one resource, then
    // we can quit there.
    // If we analyze more then one resource then we should 
    // start analyzing of the next resource
    if (m_currentAnalizationType == LaunchPage::Query ) {
        // Relaunch
    }
    else if (m_currentAnalizationType == LaunchPage::Set ) {
        // Relaunch
    }
    // Either analization type is single or there is no more
    // resources to launch
    // We can exit
    kDebug() << "Analyzing finished";
    workThread->quit();
    setButtonApply();
    updateDecisionsInfo();
}

void LaunchPage::setButtonApply()
{
    this->startButton->setGuiItem(KStandardGuiItem::Apply);
    this->startButton->setToolTip("Start extracting");
}

void LaunchPage::setButtonAbort()
{
    this->startButton->setGuiItem(KStandardGuiItem::Discard);
    this->startButton->setToolTip("Abort extracting");
}

void LaunchPage::onCurrentDecisionChanged(const QListWidgetItem * current,const QListWidgetItem * previous)
{
    kDebug() << "current decision changed";
    if(!current)
        return;


    // Get decision by url
    NW::Decision des = decisionListWidget->currentDecision();
    if(!des.isValid()) {
        kDebug() << "Decision is invalid";
        return;
    }

    //kDebug() << "Decision description: " << des.description();

    this->decisionWidget->setDecision(des);

    updateIdentificationInfo();
}
