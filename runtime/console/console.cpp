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

#include "console.h"
#include "datapppool.h"
#include <KDebug>
#include <QUrl>
#include <QBrush>
#include <QColor>
#include <KMessageBox>
#include <KStandardDirs>
#include <Nepomuk/Resource>
#include <Soprano/Vocabulary/NAO>
#include <Soprano/QueryResultIterator>
#include <Soprano/Global>
#include <Soprano/BackendSettings>
#include <Soprano/Backend>
#include <Nepomuk/Query/Query>
#include <Nepomuk/Query/LiteralTerm>
#include <Nepomuk/Query/ComparisonTerm>
#include "parameters.h"
#include "global.h"
#include "debug_datapp.h"
#include "resourceanalyzerfactory.h"
#include "resourceservicedata.h"
#include "decisionapplicationrequest.h"
#include <stdint.h>
#include <nepomuk/changelog.h>
#include "webextractor_kcm.h"
//#include "modeltest.h"

using namespace Nepomuk;
namespace NW = Nepomuk::WebExtractor;
ConsoleMainWindow::ConsoleMainWindow(
    const QString & uri ,
    const QStringList & datapps ,
    bool autostart ,
    QWidget * parent):
    KXmlGuiWindow(parent),
    workThread(0),
    m_tmpDir(0),
    m_currentKcm(0),
    m_kcmChanged(false)
{
    this->setupUi(this);

    // Set properties of the DataPPView
    this->dataPPView->setModel(Nepomuk::DataPPPool::self());
#if 0
    connect(Nepomuk::DataPPPool::self(),
            SIGNAL(dataChanged(const QModelIndex & , const QModelIndex &)),
            this->dataPPView,
            SLOT(resizeColumnToContents())
           );
    connect(this->dataPPView, SIGNAL(expanded(const QModelIndex &)),
            this->dataPPView, SLOT(resizeColumnToContents()));
    connect(this->dataPPView, SIGNAL(collapsed(const QModelIndex &)),
            this->dataPPView, SLOT(resizeColumnToContents()));
    this->dataPPView->resizeColumnToContents();
#endif

    kDebug() << *Nepomuk::DataPPPool::self();
    connect(this->startButton, SIGNAL(clicked()),
            this, SLOT(startExtracting()));
    workThread = new QThread();
    m_currentAnalyzer = 0;
    m_previousIndex = 0;

    //m_machine = new QStateMachine();
    //m_selectLaunchState = new QState();
    //m_dataPPSettingsState = new QState();
    connect(this->ktabwidget, SIGNAL(currentChanged(int)),
            this, SLOT(tabChanged(int)));

    Nepomuk::Query::ComparisonTerm coreTerm(Soprano::Vocabulary::NAO::hasTag(),
                                            Nepomuk::Query::LiteralTerm(),
                                            Nepomuk::Query::ComparisonTerm::Contains);
    //resWidget->setCoreQueryTerm(coreTerm);

    //connect(this->resWidget, SIGNAL(currentChanged(const Nepomuk::Resource &, const Nepomuk::Resource &)),
    //      this, SLOT(onCurrentResourceChanged(const Nepomuk::Resource &, const Nepomuk::Resource &)));
    this->resWidget->setQuery(Nepomuk::Query::Query());
    connect(this->resWidget, SIGNAL(selectionChanged()),
            this, SLOT(onCurrentResourceChanged()));

    // Set misc
    this->examinedHelpLabel->hide();

    // Set buttons
    setButtonApply();

    // Set table widget of examined DataPP
    this->examinedDataPPWidget->setVerticalHeaderItem(0, new QTableWidgetItem("Name"));
    this->examinedDataPPWidget->setVerticalHeaderItem(1, new QTableWidgetItem("Version"));
    this->examinedDataPPWidget->setVerticalHeaderItem(2, new QTableWidgetItem("Last extraction date"));
    this->examinedDataPPWidget->viewport()->setAcceptDrops(true);
    connect(this->examinedDataPPWidget, SIGNAL(setExaminedDataPP(const QString &, int)), this, SLOT(onMarkedExamined(const QString &, int)));

    // Connect signals
    connect(this->uriLineEdit, SIGNAL(textChanged(const QString &)), this, SLOT(onSelectedResourceChanged()));

    connect(this->clearExaminedButton, SIGNAL(clicked()), this, SLOT(onClearExamined()));
    connect(this->clearAllExaminedButton, SIGNAL(clicked()), this, SLOT(onClearAllExamined()));
    //connect(this->clearServiceButton, SIGNAL(clicked()), this, SLOT(onClearAllExamined()));

    // Set Decisions widget
    connect(this->decisionListWidget, SIGNAL(currentItemChanged(QListWidgetItem*, QListWidgetItem*)), this, SLOT(onCurrentDecisionChanged(QListWidgetItem*, QListWidgetItem*)));
    connect(this->applyDecisionButton, SIGNAL(clicked()), this, SLOT(onApplyDecision()));
    connect(this->identifyDecisionButton, SIGNAL(clicked()), this, SLOT(onIdentifyDecision()));
    connect(this->identifyMainButton, SIGNAL(clicked()), this, SLOT(onIdentifyMain()));
    connect(this->onlyMainCheckBox, SIGNAL(toggled(bool)), this, SLOT(updateIdentificationInfo()));

    // Set service info widget
    this->serviceInfoTableWidget->setColumnCount(2);

    if(uri.size())
        this->uriLineEdit->setText(uri);

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

    /* === Set properties of the DataPP Settings tab === */
    this->kcmDialogButtonBox->addButton(KStandardGuiItem::help(),QDialogButtonBox::HelpRole);
    this->kcmDialogButtonBox->addButton(KStandardGuiItem::defaults(),QDialogButtonBox::ResetRole, this, SLOT(defaultSettings()));
    this->kcmDialogButtonBox->addButton(KStandardGuiItem::reset(),QDialogButtonBox::ResetRole, this, SLOT(resetSettings()));
    this->kcmDialogButtonBox->addButton(KStandardGuiItem::apply(),QDialogButtonBox::ApplyRole, this, SLOT(applySettings()));

    // Initialization of tabs
    this->m_previousIndex = -1;
    tabChanged(this->ktabwidget->currentIndex());


}


void ConsoleMainWindow::startExtracting()
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
    // Fist check that we have necessary uri
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
    Nepomuk::Resource res(uri);
    if(!res.exists()) {
        KMessageBox::sorry(this, "There is no resource with uri you give me. Sorry");
        return;
    }


    // Now create a list of parameters
    NW::ExtractParameters * p = new Nepomuk::WebExtractor::ExtractParameters();
    p->setACrit(Nepomuk::WebExtractor::maxACrit());
    p->setUCrit(this->thresholdNumInput->value());

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
        p->addDataPP(dppw);
    }

    if(!hasAny) {
        // Warn user
        KMessageBox::sorry(this, "You forget to select DataPP. Or the DataPP you have selected are all invalid.\n  If you have selected category, then sorry - this feature is not supported yet");
        return;
    }

    // Set backend
    Soprano::BackendSettings settings;
    KTempDir * td = 0;    if(this->backendComboBox->currentText() == QString("Redland")) {
        settings << Soprano::BackendOptionStorageMemory;
        p->setBackendName("redland");
    } else if(this->backendComboBox->currentText() == QString("Virtuoso")) {
        td = new KTempDir(KStandardDirs::locateLocal("tmp", "desmodel"));
        settings << Soprano::BackendSetting(Soprano::BackendOptionStorageDir, td->name());
        p->setBackendName("virtuoso");
        // If we use virtuoso backend, then we should clean temporaly created model(s)
        p->setAutoDeleteModelData(true);
    } else {
        kDebug() << "Unknown backend is selected. Use default one.";
    }


    p->setBackendSettings(settings);

    delete m_tmpDir;
    m_tmpDir = td;


    Nepomuk::WebExtractor::ExtractParametersPtr parameters = Nepomuk::WebExtractor::ExtractParametersPtr(p);


    kDebug() << " Launch Resource Analyzer with folowing parameters: " << *p;

    m_parptr = parameters;

    NW::ResourceAnalyzerFactory factory(parameters);

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
    m_currentAnalyzer = resanal;
    resanal->setParent(0);
    resanal->moveToThread(workThread);
    connect(workThread, SIGNAL(started()), resanal, SLOT(analyze()));
    connect(resanal, SIGNAL(analyzingFinished()), this, SLOT(extractingFinished()), Qt::QueuedConnection);
    workThread->start();


    // Change button icon
    setButtonAbort();

}

void ConsoleMainWindow::cleanAfterAnalyzing()
{

    // Remove all requests
    foreach(NW::DecisionApplicationRequest * req, this->m_requestsHash) {
        delete req;
    }
    this->m_requestsHash.clear();

    // First remove model data
    delete m_currentAnalyzer;
    m_currentAnalyzer = 0;
    delete m_tmpDir;
    m_tmpDir = 0;
}


ConsoleMainWindow::~ConsoleMainWindow()
{
    workThread->quit();
    delete workThread;
    cleanAfterAnalyzing();
}


void ConsoleMainWindow::extractingFinished()
{
    kDebug() << "Analyzing finished";
    workThread->quit();
    setButtonApply();
    updateExaminedInfo();
    updateServiceInfo();
    updateDecisionsInfo();
}
void ConsoleMainWindow::tabChanged(int currentIndex)
{
    // clear selection anyway
    dataPPView->clearSelection();
    switch(m_previousIndex) {
    case SelectLaunchPage : { // Do nothing
        break;
    }
    case SettingsPage: {
        // Disconnect signals
        disconnect(this->dataPPView, SIGNAL(clicked(QModelIndex)),
                   this, SLOT(dataPPClicked(QModelIndex)));
        break;
    }
    default: { // There was no previous tab
	     }
    }
    switch(currentIndex) {
    case SelectLaunchPage: {
        dataPPView->setSelectionMode(QAbstractItemView::ExtendedSelection);
        break;
    }
    case SettingsPage : {
        dataPPView->setSelectionMode(QAbstractItemView::SingleSelection);
        connect(this->dataPPView, SIGNAL(clicked(QModelIndex)),
                this, SLOT(dataPPClicked(QModelIndex)));
        break;
    }
    }
    m_previousIndex = currentIndex;


}

void ConsoleMainWindow::applySettings()
{
    if ( m_currentKcm )
    m_currentKcm->save();
}

void ConsoleMainWindow::resetSettings()
{
    if ( m_currentKcm )
    m_currentKcm->load();
}

void ConsoleMainWindow::defaultSettings()
{
    if ( m_currentKcm )
    m_currentKcm->defaults();
}

void ConsoleMainWindow::dataPPClicked(QModelIndex index)
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
	DataPPConfig * dppcfg = new DataPPConfig( index.data(DataPPPool::SystemNameRole).toString() );
	//m_currentDataPPConfig = dppcfg;
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

void ConsoleMainWindow::dataPPSettingsChanged(bool state)
{
    this->m_kcmChanged = state;

    // Enable buttons:

}


void ConsoleMainWindow::onCurrentResourceChanged()
{
    Nepomuk::Resource current = resWidget->currentResource();
    if(current.isValid()) {
        this->uriLineEdit->setText(current.uri());
    } else {
        this->uriLineEdit->setText(QString());
    }

}

void ConsoleMainWindow::onSelectedResourceChanged()
{
    updateServiceInfo();
    updateExaminedInfo();
}

void ConsoleMainWindow::updateExaminedInfo()
{
    // Clear examined info
    this->examinedDataPPWidget->clear();
    examinedDataPPWidget->setRowCount(0);

    Nepomuk::Resource current(QUrl(uriLineEdit->text()));
    NW::ResourceServiceData rsd(current);
    if(!rsd.isValid()) {
        kError() << "ResourceServiceData is invalid";
        return;
    }

    // Change examined widget

    QMap< QString, int > ed = rsd.examinedDataPPInfo();
    QMap< QString, QDateTime > edd = rsd.examinedDataPPDates();
    kDebug() << "Examined info: " << ed;
    int currentRow = 0;
    this->examinedDataPPWidget->setRowCount(ed.size());
    for(
        QMap< QString, int >::iterator it = ed.begin();
        it != ed.end();
        it++
    ) {
        QTableWidgetItem * item = new QTableWidgetItem(it.key());
        item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
        this->examinedDataPPWidget->setItem(currentRow, 0, item);

        item = new QTableWidgetItem(QString::number(it.value()));
        item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
        this->examinedDataPPWidget->setItem(currentRow, 1, item);

        QDateTime d = edd[it.key()];
        item = new QTableWidgetItem(d.toString());
        item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
        this->examinedDataPPWidget->setItem(currentRow, 2, item);

        currentRow++;
    }
    examinedDataPPWidget->resizeColumnsToContents();
}

void ConsoleMainWindow::updateServiceInfo()
{
    // Clear service info
    this->serviceInfoTableWidget->clear();
    this->serviceInfoTableWidget->setRowCount(0);

    // Check
    Nepomuk::Resource current(QUrl(uriLineEdit->text()));
    NW::ResourceServiceData rsd(current);
    if(!rsd.isValid()) {
        kError() << "ResourceServiceData is invalid";
        return;
    }

    // Change service info
    QStringList lst = rsd.serviceInfoPropertiesNames();
    kDebug() << "Properties: " << lst;
    int siCurrentRow = 0;
    serviceInfoTableWidget->setRowCount(lst.size());
    foreach(const QString & name, lst) {
        QVariant val = rsd.property(name.toAscii().data());
        if(!val.isValid()) {
            kError() << "Property is invalid: " << name;
        }
        QTableWidgetItem * item = new QTableWidgetItem(name);
        item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
        serviceInfoTableWidget->setItem(siCurrentRow, 0, item);
        if(val.type() == QVariant::Url) {
            kDebug() << "url: " << val.toUrl();
            item = new QTableWidgetItem(val.toUrl().toString());
        } else if(val.canConvert(QVariant::String)) {
            item = new QTableWidgetItem(val.toString());
        } else {
            kError() << "Can not convert property to representable value";
            item = new QTableWidgetItem("Unrepresentable");
        }
        item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
        serviceInfoTableWidget->setItem(siCurrentRow, 1, item);

        siCurrentRow++;
    }
    serviceInfoTableWidget->resizeColumnsToContents();
}

void ConsoleMainWindow::updateDecisionsInfo()
{

    // clear widgets with  Decisions and other data structures
    this->decisionWidget->clear();
    //this->decisionInformationWidget->setRowCount(0);
    this->decisionListWidget->clear();
    this->m_decisionMap.clear();

    foreach(NW::DecisionApplicationRequest * req, this->m_requestsHash) {
        delete req;
    }
    this->m_requestsHash.clear();

    // If there is no analyzer, then return
    if(!m_currentAnalyzer) {
        kDebug() << "No curret analyzer";
        return;
    }


    NW::DecisionList lst(m_currentAnalyzer->decisions());
    kDebug() << "Number of the decisions: " << lst.size();
    foreach(const NW::Decision & d, lst) {
        // Add record to list widget
        QListWidgetItem * item = new QListWidgetItem();
        item->setData(Qt::DisplayRole, d.uri());
        this->decisionListWidget->addItem(item);

        // Add record to the map
        m_decisionMap.insert(d.uri(), d);
    }
}

void ConsoleMainWindow::updateIdentificationInfo()
{
    NW::Decision dec = this->decisionWidget->decision();
    this->identificationTableWidget->clear();

    if(!dec.isValid())
        return;

    // Search for request
    QHash<QUrl, NW::DecisionApplicationRequest*>::const_iterator fit =
        this->m_requestsHash.find(dec.uri());

    NW::DecisionApplicationRequest * req = 0;
    if(fit != this->m_requestsHash.end()) {
        req = fit.value();
        QHash<QUrl, QUrl> mcp; // mcp = Mappincg CoPy

        QSet<QUrl> uncp = req->unidentified(); // uncp = UNidentified CoPy
        kDebug() << "Unidentified: " << uncp;
        QSet<QUrl> mainResources = req->mainResources();


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
            mcp = req->mainMappings();
            kDebug() << "Mappings: " << mcp;
            identificationTableWidget->setRowCount(mainResources.size());
            QSet<QUrl>::const_iterator mrit = mainResources.begin();
            QSet<QUrl>::const_iterator mrit_end = mainResources.end();
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
                if(!mainResources.contains(*uit)) {
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

void ConsoleMainWindow::onMarkedExamined(const QString & name, int version)
{
    QUrl url = uriLineEdit->text();
    Nepomuk::Resource res(url);
    if(res.isValid() and res.exists() and url.isValid()) {
        NW::ResourceServiceData rsd(res);
        if(!rsd.isValid()) {
            kError() << "ResourceServiceData is invalid";
            return;
        }
        rsd.setExaminedDataPPInfo(name, version);
        // Add to model
        // Search for it
        QList<QTableWidgetItem*> res = examinedDataPPWidget->findItems(name, Qt::MatchExactly);
        bool hasAny = false;
        // Names are stored at column 0, versions at column 1
        // Check that there is any name in res
        QTableWidgetItem * datapp_item = 0;
        foreach(QTableWidgetItem * _item, res) {
            // Check that there is only one item
            if(_item->column() == 0) {
                if(hasAny) {
                    kError() << "There must be only one item for every DataPP";
                }
                hasAny = true;
                datapp_item = _item;
            }
        }
        QMap< QString, QDateTime > edd = rsd.examinedDataPPDates();
        if(!hasAny) {

            kDebug() << "Add new item";
            // If no version available then there is no such datapp
            // Add new item
            examinedDataPPWidget->setRowCount(examinedDataPPWidget->rowCount() + 1);
            QTableWidgetItem * _item = new QTableWidgetItem(name);
            _item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
            examinedDataPPWidget->setItem(examinedDataPPWidget->rowCount() - 1, 0, _item);

            _item = new QTableWidgetItem(version);
            _item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
            examinedDataPPWidget->setItem(examinedDataPPWidget->rowCount() - 1, 1, _item);

            QDateTime d = edd[name];
            _item = new QTableWidgetItem(d.toString());
            _item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
            this->examinedDataPPWidget->setItem(examinedDataPPWidget->rowCount() - 1, 2, _item);
        } else {
            kDebug() << "Update version";
            // Update version
            // Get item for version ( we have item for name )
            int row = datapp_item->row();
            examinedDataPPWidget->item(row, 1)->setData(Qt::DisplayRole, version);
            QDateTime d = edd[name];
            examinedDataPPWidget->item(row, 2)->setText(d.toString());
        }

        updateServiceInfo();

    } else {
        KMessageBox::sorry(this, "Please select any existing resource");
    }

    examinedDataPPWidget->resizeColumnsToContents();
}
void ConsoleMainWindow::onClearExamined()
{
    // Get selected items
    QModelIndexList lst = examinedDataPPWidget->selectionModel()->selectedRows();
    if(lst.isEmpty())
        return;


    QUrl url = uriLineEdit->text();
    Nepomuk::Resource res(url);
    if(res.isValid() and res.exists() and url.isValid()) {
        NW::ResourceServiceData rsd(res);
        if(!rsd.isValid()) {
            kError() << "ResourceServiceData is invalid";
            return;
        }
        // Convert it to the list
        foreach(const QModelIndex & idx, lst) {
            QString name = idx.data().toString();
            rsd.clearExaminedInfo(name);
        }
        this->onSelectedResourceChanged();
    }

}

void ConsoleMainWindow::onClearAllExamined()
{
    QUrl url = uriLineEdit->text();
    Nepomuk::Resource res(url);
    if(res.isValid() and res.exists() and url.isValid()) {
        NW::ResourceServiceData rsd(res);
        if(!rsd.isValid()) {
            kError() << "ResourceServiceData is invalid";
            return;
        }
        rsd.clearExaminedInfo();
        this->onSelectedResourceChanged();
    }
}

void ConsoleMainWindow::onIdentifyDecision()
{
    NW::Decision des = this->decisionWidget->decision();

    // check that there is no application request  already
    QHash<QUrl, NW::DecisionApplicationRequest*>::const_iterator fit =
        this->m_requestsHash.find(des.uri());

    NW::DecisionApplicationRequest * req = 0;
    if(fit != this->m_requestsHash.end()) {
        req = fit.value();
    } else {
        req = des.applicationRequest(Nepomuk::ResourceManager::instance()->mainModel());
        if(req)  // In case decision was invalid

            this->m_requestsHash.insert(des.uri(), req);
        else
            return;
    }

    Q_ASSERT(req);

    req->identify();
    updateIdentificationInfo();
}

void ConsoleMainWindow::onIdentifyMain()
{
    NW::Decision des = this->decisionWidget->decision();

    // check that there is no application request  already
    QHash<QUrl, NW::DecisionApplicationRequest*>::const_iterator fit =
        this->m_requestsHash.find(des.uri());

    NW::DecisionApplicationRequest * req = 0;
    if(fit != this->m_requestsHash.end()) {
        req = fit.value();
    } else {
        req = des.applicationRequest(Nepomuk::ResourceManager::instance()->mainModel());
        if(req)  // In case decision was invalid

            this->m_requestsHash.insert(des.uri(), req);
        else
            return;
    }

    Q_ASSERT(req);

    req->identifyMain();
    if(!req->isMainIdentified())
        KMessageBox::sorry(this, "Identification of main Decision's resources failed");
    updateIdentificationInfo();
}

void ConsoleMainWindow::onApplyDecision()
{
    NW::Decision des = this->decisionWidget->decision();

    // check that there is no application request  already
    QHash<QUrl, NW::DecisionApplicationRequest*>::const_iterator fit =
        this->m_requestsHash.find(des.uri());

    NW::DecisionApplicationRequest * req = 0;
    if(fit != this->m_requestsHash.end()) {
        req = fit.value();
    } else {
        req = des.applicationRequest();
        if(req)  // In case decision was invalid

            this->m_requestsHash.insert(des.uri(), req);
        else
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

void ConsoleMainWindow::onCurrentDecisionChanged(QListWidgetItem * current, QListWidgetItem * previous)
{
    if(!current)
        return;

    // Get current decision uri
    QUrl url = current->text();

    // Get decision by url
    NW::Decision des = m_decisionMap[url];
    if(!des.isValid()) {
        kDebug() << "Decision is invalid";
        return;
    }

    QHash<QUrl, QUrl> tmpMap = des.proxies();
    kDebug() << "Number of proxy resources in Decision: " << tmpMap.size();
    //kDebug() << "Decision description: " << des.description();

    this->decisionWidget->setDecision(des);

    updateIdentificationInfo();
}
void ConsoleMainWindow::setButtonApply()
{
    this->startButton->setGuiItem(KStandardGuiItem::Apply);
    this->startButton->setToolTip("Start extracting");
}

void ConsoleMainWindow::setButtonAbort()
{
    this->startButton->setGuiItem(KStandardGuiItem::Discard);
    this->startButton->setToolTip("Abort extracting");
}
