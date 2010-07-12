#include "nepomukservicedatabackend.h"
#include <QUrl>
#include <Soprano/Node>
#include <KDebug>
#include <Nepomuk/Variant>
#include <Soprano/Model>

#define USING_SOPRANO_NRLMODEL_UNSTABLE_API
//#include <Soprano/NRLModel>

#include <Soprano/QueryResultIterator>
#include <Soprano/BindingSet>
#include <Soprano/Vocabulary/NAO>
#include <Soprano/Vocabulary/RDF>
#include <Soprano/Vocabulary/NRL>
#include <Soprano/Vocabulary/RDFS>
#include <Soprano/QueryResultIterator>
#include <Nepomuk/ResourceManager>
#include <Nepomuk/Query/Query>
#include <Nepomuk/Query/ComparisonTerm>
#include <Nepomuk/Query/ResourceTerm>
#include <Nepomuk/Query/AndTerm>
#include <Nepomuk/Query/LiteralTerm>
#include <Nepomuk/Query/NegationTerm>
#include "ndco.h"
#include "datapp.h"

namespace NW = Nepomuk::WebExtractor;
namespace NQ = Nepomuk::Query;

NW::NepomukServiceDataBackend::NepomukServiceDataBackend(const QUrl & url)
{
    m_res = Nepomuk::Resource(url);
    m_url = url;
}

void NW::NepomukServiceDataBackend::setExaminedDataPPInfo(const QString & dataPPName, const QString & dataPPVersion)
{
    // Get graph node
    loadCreateGraph();
    if(!m_graphNode.isValid()) {
        kError() << "Invalid graph for storing datapp info. Probably unexisting resource";
    }


    // Get DataPP Resource uri
    QUrl dataPPUrl = dataPPResourceUrl(dataPPName, dataPPVersion);

    if(!dataPPUrl.isValid()) {
        kError() << "Invalid DataPP: " << dataPPName << ',' << dataPPVersion;
        return;
    }



    // Add necessary properties
    //kDebug() << "Enter";
    Soprano::Error::ErrorCode error = Nepomuk::ResourceManager::instance()->mainModel()->addStatement(
                                          m_url,
                                          NW::Vocabulary::NDCO::extractionFinished(),
                                          dataPPUrl,
                                          m_graphNode
                                      );

    if(error != Soprano::Error::ErrorNone) {
        kError() << "Adding statement failed with folowing error: " << Soprano::Error::errorMessage(error);
    }

    // TODO FIXME Add properties in separate graph
    //res.addProperty(NW::Vocabulary::NDCO::extractionFinished(), dataPPUrl);

}


QMap< QString, QString> NW::NepomukServiceDataBackend::examinedDataPPInfo()
{
    Nepomuk::Resource m_res(m_url);
    if(!m_res.isValid() or !m_res.exists())
        return QMap<QString, QString>();

    NQ::ComparisonTerm DataPPTerm = NQ::ComparisonTerm(
                                        Nepomuk::WebExtractor::Vocabulary::NDCO::extractionFinished(),
                                        NQ::ResourceTerm(m_res)
                                    ).inverted();

    Nepomuk::Query::Query query = NQ::Query(DataPPTerm);

    // TODO Comment the folowing kDebug
    kDebug() << "Search for DataPP query: " << query.toSparqlQuery();

    QMap< QString, QString >  answer;
    Soprano::QueryResultIterator it = Nepomuk::ResourceManager::instance()->mainModel()->executeQuery(
                                          query.toSparqlQuery(), Soprano::Query::QueryLanguageSparql
                                      );
    // TODO implement counting in query and if there are less then X results
    // ( X is user-defined parameter) cache all bindings
    // Look at bindingCount()
    while(it.next()) {
        Nepomuk::Resource dataPPRes(it.binding("r").uri());
        // TODO Comment folowing kDebug
        kDebug() << "Found folowing DataPP Resource: " << dataPPRes.resourceUri();

        // Name and version
        QString name = dataPPRes.property(Soprano::Vocabulary::RDFS::label()).toString();
        if(name.isEmpty())
            continue;
        Nepomuk::Variant vv = dataPPRes.property(Soprano::Vocabulary::NAO::version());
        if(!vv.isValid())
            continue;

        //QString name = ln[0];
        QString version = vv.toString();

        // TODO Comment folowing kDebug
        kDebug() << "Name: " << name << " Version: " << version;

        answer.insert(name, version);

    }

    return answer;
}
void NW::NepomukServiceDataBackend::loadCreateGraph()
{
    loadGraph();
    Nepomuk::Resource m_res(m_url);
    if(!m_res.isValid() or !m_res.exists())  {
        kError() << "Resource is invalid";
        return;
    }

    QUrl graphUrl;
    QUrl metaGraphUrl;

    if(!m_graphNode.isValid()) {
        // If there is no graph then create one
        Nepomuk::ResourceManager * manager = Nepomuk::ResourceManager::instance();
        // Graph doesn't exist. Create it
        // FIXME Currently usage nrl model is impossible. When it's API
        // will become stable switch back to nrl model
        /*
        Soprano::NrlModel model(Nepomuk::ResourceManager::instance()->mainModel());
        graphUrl = model.createGraph(Soprano::Vocabulary::NRL::InstanceBase(),&metaGraphUrl);
        */
        graphUrl = manager->generateUniqueUri(QLatin1String("ctx"));
        Soprano::Node graphNode = Soprano::Node(graphUrl);
        m_graphNode = graphNode;
        //d->graphRes = Nepomuk::Resource(graphUrl);
        //Soprano::Node metaGraphNode = Soprano::Node(metaGraphUrl);

        /* FIXME Disable this when switching back to nrl model
         */
        metaGraphUrl = Nepomuk::ResourceManager::instance()->generateUniqueUri(QLatin1String("ctx"));
        Soprano::Node metaGraphNode = Soprano::Node(metaGraphUrl);

        manager->mainModel()->addStatement(graphNode,
                                           Soprano::Vocabulary::RDF::type(),
                                           Soprano::Vocabulary::NRL::InstanceBase(),
                                           metaGraphNode);
        manager->mainModel()->addStatement(graphNode,
                                           Soprano::Vocabulary::NAO::created(),
                                           Soprano::LiteralValue(QDateTime::currentDateTime()),
                                           metaGraphNode);
        manager->mainModel()->addStatement(metaGraphNode,
                                           Soprano::Vocabulary::RDF::type(),
                                           Soprano::Vocabulary::NRL::GraphMetadata(),
                                           metaGraphNode);
        manager->mainModel()->addStatement(metaGraphNode,
                                           Soprano::Vocabulary::NRL::coreGraphMetadataFor(),
                                           graphNode,
                                           metaGraphNode);
        /* END of FIXME */
        manager->mainModel()->addStatement(graphNode,
                                           NW::Vocabulary::NDCO::decisionMetaGraphFor(),
                                           m_res.resourceUri(),
                                           metaGraphNode);
    }
}

void NW::NepomukServiceDataBackend::loadGraph()
{
    Nepomuk::Resource m_res(m_url);
    if(!m_res.isValid() or !m_res.exists())  {
        kError() << "Resource is invalid";
        return;
    }


    // Find graph or create one
    NQ::Query query = NQ::Query(NQ::ComparisonTerm(
                                    NW::Vocabulary::NDCO::decisionMetaGraphFor(),
                                    NQ::ResourceTerm(m_res)
                                )
                               );

    //kDebug() << "Graph search query: " << query.toSparqlQuery();
    Soprano::QueryResultIterator it = Nepomuk::ResourceManager::instance()->mainModel()->executeQuery(
                                          query.toSparqlQuery(), Soprano::Query::QueryLanguageSparql
                                      );
    QUrl graphUrl;
    QUrl metaGraphUrl;
    if(it.next()) {
        // Graph exist
        m_graphNode = it.binding("r");
        //d->graphRes = Nepomuk::Resource( it.binding("r").uri());
    }
    //kDebug() << "Graph url:" << m_graphNode.uri();
    return;
}

QUrl NW::NepomukServiceDataBackend::dataPPResourceUrl(const QString & name, const QString & version)
{
    if(name.isEmpty() or version.isEmpty())
        return QUrl();


    NQ::Query query = NQ::Query(NQ::AndTerm(
                                    NQ::ComparisonTerm(
                                        Soprano::Vocabulary::NAO::version(),
                                        NQ::LiteralTerm(version),
                                        NQ::ComparisonTerm::Equal
                                    ),
                                    NQ::ComparisonTerm(
                                        Soprano::Vocabulary::RDFS::label(),
                                        NQ::LiteralTerm(name),
                                        NQ::ComparisonTerm::Equal
                                    )
                                )
                               );
    // TODO Comment the folowing kDebug() message
    kDebug() << "Query for searching DataPP Resource: " << query.toSparqlQuery();

    Soprano::QueryResultIterator it = Nepomuk::ResourceManager::instance()->mainModel()->executeQuery(
                                          query.toSparqlQuery(), Soprano::Query::QueryLanguageSparql
                                      );

    if(it.next()) {
        // TODO Comment the folowing kDebug() message
        kDebug() << "Found DataPP Resource| name :" << name << " version: " << version << " uri " << it.binding("r").uri();
        return it.binding("r").uri();
    } else {


        // Create new resource
        Nepomuk::Resource res(ResourceManager::instance()->generateUniqueUri(), NW::Vocabulary::NDCO::DataPP());
        // Set version and name
        //QStringList identifiers;
        //identifiers << name;
        //res.setIdentifiers(identifiers);
        res.setProperty(Soprano::Vocabulary::RDFS::label(), Nepomuk::Variant(name));
        res.setProperty(Soprano::Vocabulary::NAO::version(), Nepomuk::Variant(version));

        // TODO Comment the folowing kDebug() message
        kDebug() << "Creating new DataPP Resource| name :" << name << " version: " << version << "uri" << res.resourceUri();
        return res.resourceUri();
    }


}

void NW::NepomukServiceDataBackend::clearUnusedDataPP()
{
    // Find all unreferenced DataPP
    NQ::Query query(NQ::NegationTerm::negateTerm(NQ::ComparisonTerm(
                        NW::Vocabulary::NDCO::decisionMetaGraphFor(),
                        NQ::Term()
                    )));

    Soprano::QueryResultIterator it = ResourceManager::instance()->mainModel()->executeQuery(
                                          query.toSparqlQuery(), Soprano::Query::QueryLanguageSparql);
    // Store all bindings to prevent locking
    foreach(const Soprano::BindingSet & bs, it.allBindings()) {
        Nepomuk::Resource res(bs["r"].uri());
        res.remove();
    }

}

void NW::NepomukServiceDataBackend::clearExaminedIfno()
{
    loadGraph();
    //Soprano::NRLModel  model(ResourceManager::instance()->mainModel());
    //model.removeGraph(m_graphNode.uri());
    //kError() << "IMPLEMENT THIS";
    Soprano::Model  * model = ResourceManager::instance()->mainModel();
    // Find metagraph
    NQ::Query query = NQ::Query(NQ::ComparisonTerm(
                                    Soprano::Vocabulary::NRL::coreGraphMetadataFor(),
                                    NQ::ResourceTerm(m_graphNode.uri()),
                                    NQ::ComparisonTerm::Equal
                                )
                               );
    kDebug() << "Query: " << query.toSparqlQuery();
    Soprano::QueryResultIterator it = model->executeQuery(
                                          query.toSparqlQuery(),
                                          Soprano::Query::QueryLanguageSparql
                                      );

    // Cache bindigs
    foreach(const Soprano::BindingSet & set, it.allBindings()) {
        QUrl metaGraphUrl = it.binding("r").uri();
        kDebug() << "Graph url:" << m_graphNode;
        kDebug() << "Metagraph url: " << metaGraphUrl;
        model->removeContext(metaGraphUrl);
        model->removeContext(m_graphNode.uri());
        kDebug() << "Succusfuly remove graph";
    }
}

void NW::NepomukServiceDataBackend::clearExaminedIfno(const QString & name)
{
    // Search for statement that link this datapp to the resource
    loadGraph();
    if(!m_graphNode.isValid()) {
        kError() << "No graph detected";
        return;
    }
    // select ?d { <resource> ndco:extractionFinished ?d. ?d nao:label <name> }
    // TODO Implement quering only in decisionMetaGraphFor graph
    NQ::Query query(NQ::AndTerm(
                        NQ::ComparisonTerm(
                            NW::Vocabulary::NDCO::extractionFinished(),
                            NQ::ResourceTerm(m_res)
                        ).inverted(),
                        NQ::ComparisonTerm(
                            Soprano::Vocabulary::RDFS::label(),
                            NQ::LiteralTerm(name)
                        )
                    ));

    Soprano::Model * model = ResourceManager::instance()->mainModel();
    Soprano::QueryResultIterator it = model->executeQuery(
                                          query.toSparqlQuery(),
                                          Soprano::Query::QueryLanguageSparql
                                      );

    // Cache bindings
    QList<Soprano::BindingSet> bindings = it.allBindings();

    foreach(const Soprano::BindingSet & set, bindings) {
        QUrl dataPPUrl = set.value("r").uri();

        // Make a statement. All such bindings must be in
        Soprano::Statement st(m_url, NW::Vocabulary::NDCO::extractionFinished(), dataPPUrl, m_graphNode);

        kDebug() << "Statement to remove: " << st;
        Soprano::Error::ErrorCode  error = model->removeStatement(st);

        ;
        if(error != Soprano::Error::ErrorNone) {
            kError() << "Removing statement failed with folowing error: " << Soprano::Error::errorMessage(error);
        }
    }
}

void NW::NepomukServiceDataBackend::clearServiceInfo()
{
    QString query = QString("select ?d ?g where { graph ?g { %1 %2 ?d. } }").arg(
                        Soprano::Node::resourceToN3(m_url),
                        Soprano::Node::resourceToN3(NW::Vocabulary::NDCO::extractionFinished())
                    );
    Soprano::Model * model = ResourceManager::instance()->mainModel();
    Soprano::QueryResultIterator it = model->executeQuery(
                                          query,
                                          Soprano::Query::QueryLanguageSparql
                                      );

    // Cache bindings
    QList<Soprano::BindingSet> bindings = it.allBindings();
    foreach(const Soprano::BindingSet & set, bindings) {
        // Make a statement
        Soprano::Node graphNode = set.value("g");
        Soprano::Node dataPPNode = set.value("d");
        Soprano::Statement st(m_url, NW::Vocabulary::NDCO::extractionFinished(), dataPPNode, graphNode);
        kDebug() << "Statement to remove: " << st;
        Soprano::Error::ErrorCode error = model->removeStatement(st);
        if(error != Soprano::Error::ErrorNone) {
            kError() << "Removing statement failed with folowing error: " << Soprano::Error::errorMessage(error);
        }
    }

}


Nepomuk::Resource NW::NepomukServiceDataBackend::resource() const
{
    return m_res;
    //return Nepomuk::Resource(m_url);
}

const QUrl & NW::NepomukServiceDataBackend::uri() const
{
    return m_url;
}

QUrl NW::NepomukServiceDataBackend::readGraphName() const
{
    const_cast<NW::NepomukServiceDataBackend*>(this)->loadGraph();
    return m_graphNode.uri();
}

QStringList NW::NepomukServiceDataBackend::serviceInfoPropertiesNames() const
{
    static QStringList lst = QStringList() << QString("graphName");
    return lst;
}
