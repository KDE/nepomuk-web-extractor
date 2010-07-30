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
    m_dataPPTerm = NQ::ComparisonTerm(
                       Nepomuk::WebExtractor::Vocabulary::NDCO::extractionFinished(),
                       NQ::ResourceTerm(m_res),
                       NQ::ComparisonTerm::Equal
                   ).inverted();

    m_dataPPQuery = NQ::Query(m_dataPPTerm);
    // TODO Comment the folowing kDebug
    //kDebug() << "Search for DataPP query: " << query.toSparqlQuery();
}

void NW::NepomukServiceDataBackend::setExaminedDataPPInfo(const QString & dataPPName, const QString & dataPPVersion, const QDateTime & ed)
{
    // Get graph node
    loadCreateGraph();
    if(!m_graphNode.isValid()) {
        kError() << "Invalid graph for storing datapp info. Probably unexisting resource";
        return;
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
        return;
    }

    // Add extraction date. if given datetime is invalid, the current one will be used.
    QDateTime ned;
    if(!ed.isValid())
        ned = QDateTime::currentDateTime();
    else
        ned = ed;

    error = Nepomuk::ResourceManager::instance()->mainModel()->addStatement(
                dataPPUrl,
                NW::Vocabulary::NDCO::extractionDate(),
                Soprano::LiteralValue(ned),
                m_graphNode
            );

    if(error != Soprano::Error::ErrorNone) {
        // Clean: Remove the previous statement
        Nepomuk::ResourceManager::instance()->mainModel()->removeStatement(
            m_url,
            NW::Vocabulary::NDCO::extractionFinished(),
            dataPPUrl,
            m_graphNode
        );

        kError() << "Adding statement with extraction date failed with folowing error: " << Soprano::Error::errorMessage(error);
        return;
    }



}


QMap< QString, QString> NW::NepomukServiceDataBackend::examinedDataPPInfo()
{
    //Nepomuk::Resource m_res(m_url);
    loadGraph();
    // if(!m_res.isValid() or !m_res.exists())
    if(!m_graphNode.isValid()) {
        kDebug() << "No graph  with examined info found";
        return QMap<QString, QString>();
    }

    QDateTime lmd = m_res.property(Soprano::Vocabulary::NAO::lastModified()).toDateTime();

    // If lastModification date is greater then last extraction date then
    // all examined resources must be discarded
    /*
    if ( lmd > led ) {
    clearExaminedInfo();
    return QMap<QString, QString >();
    }
    */



    QMap< QString, QString >  answer;
    Soprano::QueryResultIterator it = Nepomuk::ResourceManager::instance()->mainModel()->executeQuery(
                                          m_dataPPQuery.toSparqlQuery(), Soprano::Query::QueryLanguageSparql
                                      );
    QSet<QString> toRemove;
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
            // This is a bug.
            continue;
        Nepomuk::Variant vv = dataPPRes.property(Soprano::Vocabulary::NAO::version());
        if(!vv.isValid()) {
            // This is a bug. Clear
            toRemove.insert(name);
            continue;
        }

        //QString name = ln[0];
        QString version = vv.toString();

        // Now found date of extraction
        // Use QString because it is much easier to read and understand

        // If lastModificationDate of the resource is not availabel, then skip this check
        if(lmd.isValid()) {
            QString date_query = date_query_templ().arg(
                                     Soprano::Node::resourceToN3(m_graphNode.uri()),
                                     Soprano::Node::resourceToN3(dataPPRes.uri()),
                                     Soprano::Node::resourceToN3(NW::Vocabulary::NDCO::extractionDate())
                                 );
            Soprano::QueryResultIterator it2 = Nepomuk::ResourceManager::instance()->mainModel()->executeQuery(
                                                   date_query,
                                                   Soprano::Query::QueryLanguageSparql
                                               );
            QDateTime ed;
            if(it2.next()) {
                Soprano::Node edNode = it2.binding("r");
                if(!edNode.isValid() or !edNode.isLiteral()) {
                    // Date is incorrect. Clear
                    toRemove.insert(name);
                    continue;
                }
                Soprano::LiteralValue edv = edNode.literal();
                if(!edv.isDateTime()) {
                    // Date is incorrect. Clear
                    toRemove.insert(name);
                    continue;
                }

                ed = edv.toDateTime();
            } else {
                kError() << "No record about date. Resource: " << dataPPRes.uri();
                toRemove.insert(name);
                continue;
            }
            // Check extraction date
            if(lmd > ed) {
                // Resource has changed since last extraction.
                // We should remove record about this from the model
                toRemove.insert(name);
                continue;
            }
        }


        // TODO Comment folowing kDebug
        kDebug() << "Name: " << name << " Version: " << version;

        answer.insert(name, version);

    }

    // Clear obsolete examined info
    kDebug() << "The records about the folowing examined DataPP are now invalid and will be removed: " << toRemove;
    foreach(const QString & name, toRemove) {
        clearExaminedInfo(name);
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

        QList<Soprano::Statement> statements;
        statements <<
                   Soprano::Statement(graphNode,
                                      Soprano::Vocabulary::RDF::type(),
                                      Soprano::Vocabulary::NRL::InstanceBase(),
                                      metaGraphNode) <<
                   Soprano::Statement(graphNode,
                                      Soprano::Vocabulary::NAO::created(),
                                      Soprano::LiteralValue(QDateTime::currentDateTime()),
                                      metaGraphNode) <<
                   Soprano::Statement(metaGraphNode,
                                      Soprano::Vocabulary::RDF::type(),
                                      Soprano::Vocabulary::NRL::GraphMetadata(),
                                      metaGraphNode) <<
                   Soprano::Statement(metaGraphNode,
                                      Soprano::Vocabulary::NRL::coreGraphMetadataFor(),
                                      graphNode,
                                      metaGraphNode);
        /* END of FIXME */
        statements << Soprano::Statement(
                       graphNode,
                       NW::Vocabulary::NDCO::decisionMetaGraphFor(),
                       m_res.resourceUri(),
                       metaGraphNode);

        Soprano::Error::ErrorCode error = Nepomuk::ResourceManager::instance()->mainModel()->addStatements(statements);
        if(error != Soprano::Error::ErrorNone) {
            kError() << "Failed to create examined graph. Error: " << Soprano::Error::errorMessage(error);
        } else {
            kDebug() << "Create examinend info graph: " << graphUrl;
        }
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
    // Temporaly switch to the QString queries
    /*
    NQ::Query query = NQ::Query(NQ::ComparisonTerm(
                                    NW::Vocabulary::NDCO::decisionMetaGraphFor(),
                                    NQ::ResourceTerm(m_res),
                                    NQ::ComparisonTerm::Equal
                                )
                               );
    QString queryString = query.toSparqlQuery();
    */
    /* Comment this when uncomment above */
    static QString queryTempl = QString("select ?r where { ?r %1 %2 .}");
    QString queryString = queryTempl.arg(
                              Soprano::Node::resourceToN3(NW::Vocabulary::NDCO::decisionMetaGraphFor()),
                              Soprano::Node::resourceToN3(m_res.resourceUri())
                          );




    kDebug() << "Graph search query: " << queryString;
    Soprano::QueryResultIterator it = Nepomuk::ResourceManager::instance()->mainModel()->executeQuery(
                                          queryString, Soprano::Query::QueryLanguageSparql
                                      );
    QUrl graphUrl;
    QUrl metaGraphUrl;
    if(it.next()) {
        // Graph exist
        m_graphNode = it.binding("r");
        //d->graphRes = Nepomuk::Resource( it.binding("r").uri());
        kDebug() << "Graph url:" << m_graphNode.uri();
    } else {
        kDebug() << "Graph not found";
    }
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

void NW::NepomukServiceDataBackend::clearExaminedInfo()
{
    loadGraph();
    if(!m_graphNode.isValid()) {
        return;
    }

    //Soprano::NRLModel  model(ResourceManager::instance()->mainModel());
    //model.removeGraph(m_graphNode.uri());
    //kError() << "IMPLEMENT THIS";
    Soprano::Model  * model = ResourceManager::instance()->mainModel();
    // Find metagraph
    /* Uncomment and fix this when Nepomuk Query API become more stable */
    /*
    NQ::Query query = NQ::Query(NQ::ComparisonTerm(
                                    Soprano::Vocabulary::NRL::coreGraphMetadataFor(),
                                    NQ::ResourceTerm(m_graphNode.uri()),
                                    NQ::ComparisonTerm::Equal
                                )
                               );
    QString queryString = query.toSparqlQuery();
    */
    /* Uncomment above and comment this when it will be necessary*/
    static QString queryTempl = QString("select ?r where {?r %1 %2 }");
    QString queryString = queryTempl.arg(
                              Soprano::Node::resourceToN3(Soprano::Vocabulary::NRL::coreGraphMetadataFor()),
                              Soprano::Node::resourceToN3(m_graphNode.uri())
                          );
    /* End of second variant */
    kDebug() << "Query: " << queryString;
    Soprano::QueryResultIterator it = model->executeQuery(
                                          queryString,
                                          Soprano::Query::QueryLanguageSparql
                                      );

    // Cache bindigs
    foreach(const Soprano::BindingSet & set, it.allBindings()) {
        QUrl metaGraphUrl = set.value("r").uri();
        kDebug() << "Graph url:" << m_graphNode;
        kDebug() << "Metagraph url: " << metaGraphUrl;
        model->removeContext(metaGraphUrl);
        model->removeContext(m_graphNode.uri());
        kDebug() << "Succusfuly remove graph";
    }
}

void NW::NepomukServiceDataBackend::clearExaminedInfo(const QString & name)
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

        // Now delete statement about extraction date. Statement is wildward statement
        Soprano::Statement d_st(dataPPUrl, NW::Vocabulary::NDCO::extractionDate(), Soprano::Node(), m_graphNode);
        error = model->removeAllStatements(d_st);

        ;
        if(error != Soprano::Error::ErrorNone) {
            kError() << "Removing statement with extraction date failed with folowing error: " << Soprano::Error::errorMessage(error);
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

QMap< QString, QDateTime > NW::NepomukServiceDataBackend::examinedDataPPDates()
{
    loadGraph();
    if(!m_graphNode.isValid())
        return QMap< QString, QDateTime >();


    Soprano::QueryResultIterator it = Nepomuk::ResourceManager::instance()->mainModel()->executeQuery(
                                          m_dataPPQuery.toSparqlQuery(),
                                          Soprano::Query::QueryLanguageSparql
                                      );
    QMap< QString, QDateTime > answer;
    while(it.next()) {
        Nepomuk::Resource dataPPRes(it.binding("r").uri());

        // Name
        QString name = dataPPRes.property(Soprano::Vocabulary::RDFS::label()).toString();
        // Date
        QString date_query = date_query_templ().arg(
                                 Soprano::Node::resourceToN3(m_graphNode.uri()),
                                 Soprano::Node::resourceToN3(dataPPRes.uri()),
                                 Soprano::Node::resourceToN3(NW::Vocabulary::NDCO::extractionDate())
                             );
        Soprano::QueryResultIterator it2 = Nepomuk::ResourceManager::instance()->mainModel()->executeQuery(
                                               date_query,
                                               Soprano::Query::QueryLanguageSparql
                                           );
        QDateTime ed;
        if(it2.next()) {
            Soprano::Node edNode = it2.binding("r");
            if(!edNode.isValid() or !edNode.isLiteral())
                continue;
            Soprano::LiteralValue edv = edNode.literal();
            if(!edv.isDateTime())
                continue;

            ed = edv.toDateTime();
        }

        answer[name] = ed;
    }

    return answer;

}

QDateTime NW::NepomukServiceDataBackend::examinedDate(const QString & name)
{
    loadGraph();

    if(!m_graphNode.isValid())
        return QDateTime();

    // Search for this datapp
    NQ::Query query = NQ::Query(NQ::AndTerm(
                                    m_dataPPTerm,
                                    NQ::ComparisonTerm(
                                        Soprano::Vocabulary::RDFS::label(),
                                        NQ::LiteralTerm(name),
                                        NQ::ComparisonTerm::Equal
                                    )
                                )
                               );

    Soprano::QueryResultIterator it = Nepomuk::ResourceManager::instance()->mainModel()->executeQuery(
                                          query.toSparqlQuery(),
                                          Soprano::Query::QueryLanguageSparql
                                      );
    QDateTime ed;
    if(it.next()) {
        QString date_query = date_query_templ().arg(
                                 Soprano::Node::resourceToN3(m_graphNode.uri()),
                                 Soprano::Node::resourceToN3(it.binding("r").uri()),
                                 Soprano::Node::resourceToN3(NW::Vocabulary::NDCO::extractionDate())
                             );
        Soprano::QueryResultIterator it2 = Nepomuk::ResourceManager::instance()->mainModel()->executeQuery(
                                               date_query,
                                               Soprano::Query::QueryLanguageSparql
                                           );
        if(it2.next()) {
            Soprano::Node edNode = it2.binding("r");
            if(!edNode.isValid() or !edNode.isLiteral())
                return ed;
            Soprano::LiteralValue edv = edNode.literal();
            if(!edv.isDateTime())
                return ed;

            ed = edv.toDateTime();
        }
    }

    return ed;

}

QString NW::NepomukServiceDataBackend::date_query_templ()
{
    static QString v = "select ?r where { graph %1 { %2 %3 ?r . } }";
    return v;
}
