#include "console.h"
#include "datapppool.h"
#include <KDebug>
#include <KMessageBox>
#include <Nepomuk/Resource>
//#include "modeltest.h"

ConsoleMainWindow::ConsoleMainWindow(QWidget * parent):
    QMainWindow(parent)
{
    this->setupUi(this);
    //new ModelTest(Nepomuk::DataPPPool::self(), this);
    this->dataPPView->setModel(Nepomuk::DataPPPool::self());
    kDebug() << *Nepomuk::DataPPPool::self();
    connect( this->startButton, SIGNAL(clicked()),
	    this, SLOT(startExtracting()));
}


void ConsoleMainWindow::startExtracting()
{
    // Fist check that we have necessary uri
    QString uriString = uriLineEdit->text();
    if (uriString.isEmpty()) {
	KMessageBox::sorry(this, "Please provied an uri to extract metadata for");
	return;
    }

    // TODO Automaticaly fix some user errors like:
    // missed nepomuk:/
    // may be some others
    QUrl uri(uriString);


    // Check that there is a Resource with this uri
    Nepomuk::Resource res(uri);
    if (!res.exists()) {
	KMessageBox::sorry(this "There is no resource with uri you give me. Sorry");
	return;
    }


    // Now create a list of parameters
    ExtractParametersPtr parameters = ExtractParametersPtr(new ExtractParameters());
    parameters->setACrit(Nepomuk::WebExtractor::WE::maxAcrit());
    parameters->setUCrit(this->thresholdNumInput->value());

    // Add DataPP
    // TODO Currently system use view selection as list of all selected DataPP


}
