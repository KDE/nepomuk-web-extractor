#include "console.h"
#include "datapppool.h"
#include <KDebug>
#include <KMessageBox>
#include <Nepomuk/Resource>
#include <webextractor/parameters.h>
#include <webextractor/global.h>
//#include "modeltest.h"

using namespace Nepomuk;
namespace NW = Nepomuk::WebExtractor;
ConsoleMainWindow::ConsoleMainWindow(QWidget * parent):
    QMainWindow(parent)
{
    this->setupUi(this);
    //new ModelTest(Nepomuk::DataPPPool::self(), this);
    this->dataPPView->setModel(Nepomuk::DataPPPool::self());
    kDebug() << *Nepomuk::DataPPPool::self();
    connect(this->startButton, SIGNAL(clicked()),
            this, SLOT(startExtracting()));
}


void ConsoleMainWindow::startExtracting()
{
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

        hasAny = true;

        QString dataPPName = index.data(DataPPPool::NameRole).toString();

        NW::DataPP * dpp = DataPPConfig::dataPP(dataPPName);
        NW::DataPPWrapper * dppw =  new NW::DataPPWrapper(dpp, dataPPName, 1.0, 1.0);
        p->addDataPP(dppw);
    }

    if(!hasAny) {
        // Warn user
        KMessageBox::sorry(this, "You forget to select DataPP. If you have selected category, then sorry - this feature is not supported yet");
        return;
    }

    Nepomuk::WebExtractor::ExtractParametersPtr parameters = Nepomuk::WebExtractor::ExtractParametersPtr(p);

    kDebug() << " Launch Resource Analyzer with folowing parameters: " << *p;

}
