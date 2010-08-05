#include "graphvizmainwindow.h"
#include <webextractor/modelgraphvisitor.h>
#include <webextractor/algorithm.h>
#include <QHash>
#include <QFile>
#include <KMessageBox>
#include <KSaveFile>
#include <Soprano/Node>


// Implement Dot-visitor

class DotVisitor : public Nepomuk::Graph::NullVisitor<void*, Soprano::Node>
{

    public:
        DotVisitor(QTextStream * stream);
        void start(void *, bool);
        void finish(void *, bool);
        void enter_vertex(void *  base, Soprano::Node node);
        void enter_edge(void * base, const Soprano::Node &  node, const Soprano::Node &  currentNode, const Soprano::Node &  propertyNode, const Soprano::Node &  childNode);
        //  write(QTextStream &);
    private:
        QTextStream * m_stream;
        int vnum;
        int edgenum;
        QString vertexID(const Soprano::Node &);
        QString vertexDescription(const Soprano::Node &);
        QHash< Soprano::Node, int > vertices;
        //Q
        /*
            class Private;
            QSharedPointer<Private> d;
            */
};

/*
class DotVisitor::Private
{
    public:
    QStringList vertices;
    QStringLis edges;
}
*/

DotVisitor::DotVisitor(QTextStream * stream)
{
    this->m_stream = stream;
    vnum = 0;
    edgenum = 0;
}

QString DotVisitor::vertexDescription(const Soprano::Node & node)
{
    if(node.isBlank()) {
        static QString blank("blank");
        return blank;
    } else if(node.isLiteral())
        return node.toString().replace('\"', "'");
    else { // Node is resource
        return node.uri().toString();
    }
}
QString DotVisitor::vertexID(const Soprano::Node & node)
{
    if(node.isBlank()) {
        static QString blankTemplate("blank%1");
        vnum++;
        return blankTemplate.arg(QString::number(vnum));
    } else {
        QHash< Soprano::Node , int >::const_iterator fit =
            vertices.find(node);
        if(fit == vertices.end()) {
            // Write new node
            vnum++;
            vertices.insert(node, vnum);
            return QString::number(vnum);
        } else {
            return QString::number(fit.value());
        }
    }

}

void DotVisitor::enter_vertex(void *  base, Soprano::Node node)
{
    Q_UNUSED(base);
    static QString vertexTemplate = QString("v%1 [label=\"%2\"]\n");
    *m_stream <<  vertexTemplate.arg(vertexID(node), vertexDescription(node));
}

void DotVisitor::enter_edge(void * base, const Soprano::Node &  node, const Soprano::Node &  currentNode, const Soprano::Node &  propertyNode, const Soprano::Node &  childNode)
{
    Q_UNUSED(base);
    static QString edgeTemplate = QString(" v%1 -> v%2 [label=\"%3\"]\n");
    *m_stream << edgeTemplate.arg(
                  vertexID(currentNode),
                  vertexID(childNode),
                  propertyNode.uri().fragment()
              );
}

void DotVisitor::start(void * base, bool val)
{
    if(val)
        *m_stream << "digraph {\n";
}

void DotVisitor::finish(void * base, bool val)
{
    if(val)
        *m_stream << "}\n";
}

GraphVizMainWindow::GraphVizMainWindow(QWidget * parent):
    KXmlGuiWindow(parent)
{
    this->setupUi(this);

    // Set query core term for properties selection
    //

    //  Set widgets
    this->resultFileRequester->setMode(KFile::File);

    //  connect signals
    connect(this->applybutton, SIGNAL(clicked()), this, SLOT(draw()));
}
GraphVizMainWindow::~GraphVizMainWindow()
{
}
void GraphVizMainWindow::draw()
{
    // Get current resources
    QList<QUrl> targets;
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
        filename = targets[0].toString();
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

    typedef void * BaseType; /* Base class, because we don't need it */
    typedef Soprano::Node NodeType; /* Type of the Node. It can be ignored too */
    typedef Nepomuk::DummyNodeFunc<void*> NodeFunc; /* For given Node return itself as Node descriptor */
    typedef DotVisitor
    Visitor; /* Conversation func. This function will perform actual copying */
    typedef Nepomuk::Graph::SelectedResourcePropertiesFunc<void*, Soprano::Node>
    NextFunc;/* This function will return as childs all nodes that are resource and there is triple ( subject, ?p, child )*/

    // Parse some parames
    NextFunc::IgnoreFlags flags = NextFunc::NoIgnore;
    if(this->ignoreOntologyCheckBox->isChecked())
        flags = NextFunc::IgnoreFlags(flags | NextFunc::IgnoreOntology);

    Nepomuk::Graph::visit_model_graph <
    BaseType,
    NodeType,
    NodeFunc,
    Visitor,
    NextFunc
    > (
        0,
        Nepomuk::ResourceManager::instance()->mainModel(),
        targets,
        -1,
        NodeFunc(),
        Visitor(&stream),
        NextFunc(QSet<QUrl>(), flags),
        false
    );


    file.finalize();
    //delete file;

}
