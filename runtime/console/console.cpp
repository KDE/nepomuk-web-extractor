#include "console.h"
#include "datapppool.h"
#include <KDebug>
#include <QUrl>
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
#include <querybuildersearchwidget.h>
#include <stdint.h>
#include <changelog.h>
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
    m_tmpDir(0)
{
    this->setupUi(this);
    //new ModelTest(Nepomuk::DataPPPool::self(), this);
    this->dataPPView->setModel(Nepomuk::DataPPPool::self());
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
    connect(this->resWidget, SIGNAL(selectionChanged()),
            this, SLOT(onCurrentResourceChanged()));

    // Set table widget of examined DataPP
    this->examinedDataPPWidget->setVerticalHeaderItem(0, new QTableWidgetItem("Name"));
    this->examinedDataPPWidget->setVerticalHeaderItem(1, new QTableWidgetItem("Version"));
    this->examinedDataPPWidget->setVerticalHeaderItem(2, new QTableWidgetItem("Last extraction date"));
    this->examinedDataPPWidget->viewport()->setAcceptDrops(true);
    connect(this->examinedDataPPWidget, SIGNAL(setExaminedDataPP(const QString &, float)), this, SLOT(onMarkedExamined(const QString &, float)));

    // Connect signals
    connect(this->uriLineEdit, SIGNAL(textChanged(const QString &)), this, SLOT(onSelectedResourceChanged()));

    connect(this->clearExaminedButton, SIGNAL(clicked()), this, SLOT(onClearExamined()));
    connect(this->clearAllExaminedButton, SIGNAL(clicked()), this, SLOT(onClearAllExamined()));
    //connect(this->clearServiceButton, SIGNAL(clicked()), this, SLOT(onClearAllExamined()));

    // Set Decisions widget
    connect(this->decisionListWidget, SIGNAL(currentItemChanged(QListWidgetItem*, QListWidgetItem*)), this, SLOT(onCurrentDecisionChanged(QListWidgetItem*, QListWidgetItem*)));

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

}


void ConsoleMainWindow::startExtracting()
{
    if(workThread->isRunning()) {
        kError() << "Application is currently analyzing another resource. You must abort previous analyzing first";
        KMessageBox::sorry(this, "Application is currently analyzing another resource. You must abort previous analyzing first");
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
    p->setACrit(Nepomuk::WebExtractor::WE::maxACrit());
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
    //resanal->analyze();

}

void ConsoleMainWindow::cleanAfterAnalyzing()
{

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

void ConsoleMainWindow::dataPPClicked(QModelIndex index)
{
    if(!index.data(DataPPPool::DataPPRole).toBool()) {
        this->sourceNameLabel->setText("This is a category");
        return;
    }


    QString dataPPSource = index.data(DataPPPool::SourceRole).toString();
    if(dataPPSource.isEmpty()) {
        this->sourceNameLabel->setText("Invalid DataPP: Source not set");
        return;
    } else {
        this->sourceNameLabel->setText("Source: " + dataPPSource);
        return;
    }


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

    QMap< QString, float > ed = rsd.examinedDataPPInfo();
    QMap< QString, QDateTime > edd = rsd.examinedDataPPDates();
    kDebug() << "Examined info: " << ed;
    int currentRow = 0;
    this->examinedDataPPWidget->setRowCount(ed.size());
    for(
        QMap< QString, float >::iterator it = ed.begin();
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
}

void ConsoleMainWindow::updateDecisionsInfo()
{

    // clear widgets with  Decisions and other data structures
    this->decisionWidget->clear();
    //this->decisionInformationWidget->setRowCount(0);
    this->decisionListWidget->clear();
    this->m_decisionMap.clear();

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


void ConsoleMainWindow::onMarkedExamined(const QString & name, float version)
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

#if 0
    // Display decision
    // Display description
    this->descriptionTextBrowser->setPlainText(des.description());
    // Display ChangeLog
    Q_ASSERT(des.model());
    QString logText;
    QTextStream stream(&logText);
    Nepomuk::Sync::ChangeLog log(des.groupsUrls(), des.model());
    kDebug() << "Log created";
    foreach(const Nepomuk::Sync::ChangeLogRecord & rc, log.toList()) {
        //stream << rc.toString();
        stream << rc;
    }

    //stream << log;
    this->changeLogBrowser->setPlainText(logText);
    // Display changes
    foreach(const NW::PropertiesGroup & grp, des.groups()) {
        kDebug() << "PropertiesGroup: " << grp.uri();
        // Add widget item that corresponds the group
        QTreeWidgetItem * grpItem  = new QTreeWidgetItem();
        grpItem->setData(Qt::DisplayRole, 0, grp.uri());
        this->decisionInformationWidget->addTopLevelItem(grpItem);

        // Unfortunately displaying Decision doesn't work now. It is a bug
        // and I will fix it.
        // As temporary solution, decisions will be displayed in less
        // convinient form
#endif
#if 0

        // For each target resource show it's changes ( if any )
        for(
            QMap<QUrl, QUrl>::const_iterator it = tmpMap.begin();
            it != tmpMap.end();
            it++
        ) {
            QUrl originalResource = it.key();
            QUrl proxyResource = it.value();
            // Check that there is any changes for the resource in this group

            static QString query_template = "select distinct ?p ?o where { graph %1 {  %2 ?p ?o } }";
            QString query = query_template.arg(
                                Soprano::Node::resourceToN3(grp.uri()),
                                Soprano::Node::resourceToN3(proxyResource)
                            );

            // execute query
            Soprano::QueryResultIterator it = des.model()->executeQuery(
                                                  query,
                                                  Soprano::Query::QueryLanguageSparql
                                              );
            // Create item only if there is any result
            QTreeWidgetItem * resItem;
            if(it.next()) {
                // Create item that represents the resource
                resItem = new QTreeWidgetItem();
                resItem->setData(0, Qt::DisplayRole, originalResource.toString());
                // Add this item to widget
                grpItem->addChild(resItem);


                // Create item that represents the change
                QTreeWidgetItem * item = new QTreeWidgetItem();
                item->setData(0, Qt::DisplayRole, it.binding("p").uri());
                item->setData(1, Qt::DisplayRole, it.binding("o").uri());
                resItem->addChild(item);

                while(it.next()) {
                    // Create item that represents the change
                    QTreeWidgetItem * item = new QTreeWidgetItem();
                    item->setData(0, Qt::DisplayRole, it.binding("p").uri());
                    item->setData(1, Qt::DisplayRole, it.binding("o").uri());
                    resItem->addChild(item);
                }
            } else {
                kDebug() << "PropertiesGroup doesn't contains any changes for resource: " <<
                         originalResource;
            }

        }
#endif
        // Disable this when bug will be fixed
#if 0
        static QString queryTemplate = QString("select ?s ?p ?o where { graph %1 { ?s ?p ?o .} } ");
        QString queryString = queryTemplate.arg(Soprano::Node::resourceToN3(grp.uri()));
        Soprano::QueryResultIterator it = des.model()->executeQuery(
                                              queryString,
                                              Soprano::Query::QueryLanguageSparql
                                          );
        while(it.next()) {
            QTreeWidgetItem * item = new QTreeWidgetItem();
            item->setData(0, Qt::DisplayRole, it.binding("s").uri());
            item->setData(1, Qt::DisplayRole, it.binding("p").uri());
            item->setData(2, Qt::DisplayRole, it.binding("o").uri());
            grpItem->addChild(item);
            kDebug() << "Add statement to item";
        }
    }
#endif
}