#include "graphvizmainwindow.h"
#include "algorithm.h"
#include <QHash>
#include <QFile>
#include <KMessageBox>
#include <KSaveFile>
#include <KDebug>
#include <Soprano/Node>
#include <Nepomuk/Resource>
#include <Nepomuk/ResourceManager>
#include "dotvisitor.h"
#include "childqueryinterface.h"
#include "selectedpropertiesfunc.h"
#include "resourcenodefilter.h"
#include "modelgraphvisitor.h"
#include "modelgraph.h"
#include "graphalgorithm.h"
#include "plaintextvisitor.h"
#include <Nepomuk/Query/Query>


using namespace Nepomuk::Graph;


GraphVizMainWindow::GraphVizMainWindow(QWidget * parent):
    KXmlGuiWindow(parent)
{
    this->setupUi(this);

    // Set query core term for properties selection
    //

    //  Set widgets
    this->resultFileRequester->setMode(KFile::File);

    this->resourcesSelectWidget->setQuery(Nepomuk::Query::Query());

    //  connect signals
    connect(this->applybutton, SIGNAL(clicked()), this, SLOT(draw()));
}
GraphVizMainWindow::~GraphVizMainWindow()
{
}
void GraphVizMainWindow::draw()
{
    // Get current resources
    QSet<QUrl> targets;
    foreach(const Nepomuk::Resource & res, this->resourcesSelectWidget->selectedResources()) {
        targets << res.resourceUri();
    }
    if(targets.isEmpty())
        return;

    kDebug() << "Drawing resources: " << targets;

    // Get filename
    KUrl fileUrl = resultFileRequester->url();
    QString filename;

    if(fileUrl.isEmpty()) {
        // Use first resource url as filename
        filename = targets.begin()->toString();
    } else {
        filename = fileUrl.toLocalFile();
    }


    // Create text stream
    //QFile * file = new QFile(filename);
    //file->open( QIODevice::WriteOnly );
    //QTextStream stream(file);
    KSaveFile file(filename);
    if(!file.open(QIODevice::WriteOnly)) {
        KMessageBox::error(this, "Can't open file for saving");
        return;
    }
    QTextStream stream(&file);




    // launch algorithm
    ModelGraphVisitor * visitor = 0;
    ChildQueryInterface * childrenFunc = new SelectedPropertiesFunc();

    // Parse some parames
    if(this->nonresourceFilterCheckBox->isChecked())
        childrenFunc = new ResourceNodeFilter(childrenFunc);

    ModelGraph * mg = new ModelGraph(Nepomuk::ResourceManager::instance()->mainModel(), childrenFunc);

    if(this->dotRadioButton->isChecked()) {

        visitor = new DotVisitor(&stream); /* Conversation func. This function will perform actual copying */
    } else if(this->textRadioButton->isChecked()) {
        visitor = new PlainTextVisitor(&stream);
    }

    if(visitor) {
        Nepomuk::Graph::visit_model_graph(
            mg,
            targets,
            visitor,
            -1,
            false
        );
    }

    delete visitor;
    delete mg;
    delete childrenFunc;


    file.finalize();
    //delete file;

}
