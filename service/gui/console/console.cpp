#include "console.h"
#include "datapppool.h"
#include <KDebug>
#include <KMessageBox>
#include <Nepomuk/Resource>
#include <Soprano/Vocabulary/NAO>
#include <Nepomuk/Query/Query>
#include <Nepomuk/Query/LiteralTerm>
#include <Nepomuk/Query/ComparisonTerm>
#include <webextractor/parameters.h>
#include <webextractor/global.h>
#include <webextractor/debug_datapp.h>
#include <webextractor/resourceanalyzerfactory.h>
#include <querybuildersearchwidget.h>
#include <stdint.h>
//#include "modeltest.h"

using namespace Nepomuk;
namespace NW = Nepomuk::WebExtractor;
ConsoleMainWindow::ConsoleMainWindow(
    const QString & uri ,
    const QStringList & datapps ,
    bool autostart ,
    QWidget * parent):
    KXmlGuiWindow(parent),
    workThread(0)
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
    resWidget->setCoreQueryTerm(coreTerm);

    //connect(this->resWidget, SIGNAL(currentChanged(const Nepomuk::Resource &, const Nepomuk::Resource &)),
    //      this, SLOT(onCurrentResourceChanged(const Nepomuk::Resource &, const Nepomuk::Resource &)));
    connect(this->resWidget, SIGNAL(selectionChanged()),
            this, SLOT(onCurrentResourceChanged()));

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

    // delete previous analyzes
    delete m_currentAnalyzer;
    m_currentAnalyzer = resanal;
    resanal->setParent(0);
    resanal->moveToThread(workThread);
    connect(workThread, SIGNAL(started()), resanal, SLOT(analyze()));
    connect(resanal, SIGNAL(analyzingFinished()), this, SLOT(extractingFinished()), Qt::QueuedConnection);
    workThread->start();
    //resanal->analyze();

}

ConsoleMainWindow::~ConsoleMainWindow()
{
    workThread->quit();
    delete workThread;
}


void ConsoleMainWindow::extractingFinished()
{
    kDebug() << "Analyzing finished";
    workThread->quit();
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
