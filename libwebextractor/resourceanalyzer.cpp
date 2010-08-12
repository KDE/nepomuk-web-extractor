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
#include "resourceanalyzer.h"
#include "resourceservicedata.h"
#include <KDebug>
#include <QDateTime>
#include <QtCore/QTimer>
#include <assert.h>
#include <stdint.h>
#include <QSharedData>
#include <Nepomuk/Resource>

namespace NW = Nepomuk::WebExtractor;


class Nepomuk::WebExtractor::ResourceAnalyzer::Private /*: public QSharedData*/
{
    public:
        Private(const DataPPKeeper & dataPPKeeper, DecisionFactory * fact);
        ~Private();
    public:
        //int tmp_count;
        // Number of responds that must be recived
        int m_respWaits;
        // THis is hash <DataPP*,DataPPWrapper*> where all DataPPWrappers are storeg
        // keys are corresponding DataPP*
        DataPPKeeper m_dataPPKeeper;
        //DataPPKeeper::const_iterator it;
        // DecisionFactory that will be passed to DataPP
        DecisionFactory * m_fact;
        //QMap< DataPPReply*, double > m_replyAndRanks;
        WE::LaunchPolitics m_launchPolitics;
        unsigned int m_step;
        // All collected Decisions are stored there
        DecisionList m_decisions;
        WE::MergePolitics m_mergePolitics;
        double m_mergeCoff;
        // This is stored resource we currently analyze. Or are going to analyze.
        Nepomuk::Resource m_res;
        double m_acrit;
        double m_ucrit;
        // This is queue where all DataPP that must be launched are stored
        QQueue<DataPPWrapper*> m_queue;
        // THis variable prevent executing apply() method more then one time
        bool m_applied;
        // This variable keep running/not-running state of ResourceAnalyzer
        bool m_running;
        // The last error that occure while analyzing
        ResourceAnalyzer::AnalyzingError m_error;
        // Store all replies there
        QSet<DataPPReply*> m_replies;

        // Store pointers to examined DataPP ( DataPP that has finished w/out errors )
        QSet<DataPP*> m_examined;

        // Store analyzing politics
        AnalyzingPolitics m_apolitics;

        // In this variable we store the context url for target resource,
        // where all metadata for resource are stored
        QUrl resourceContext;

        // The data about examined DataPP are managed with this variable
        ResourceServiceData rsd;

        // When working in Iterative mode, it is useless to write information about
        // examined DataPP in current iteration directly to storage. Instead it is
        // stored there
        QMap<DataPPWrapper*, QDateTime > examinedDates;


        // Convinience method to add set of DataPP* to queue
        void enqueue(const QSet<const DataPP*> &);

        //static Nepomuk::Query::Query findContextGraphQuery(const Nepomuk::Resource & );

        //QSet<QString> findUnexaminedDataPP() const;
        void filterExaminedDataPP() ;

};

Nepomuk::WebExtractor::ResourceAnalyzer::Private::Private(
    const DataPPKeeper & dataPPKeeper,
    DecisionFactory * fact
):
    m_respWaits(0),
    m_dataPPKeeper(dataPPKeeper),
    m_fact(fact),
    m_mergeCoff(1),
    m_applied(false),
    m_running(false),
    m_error(ResourceAnalyzer::NoError),
    m_apolitics(ResourceAnalyzer::SingleStep)
{
    //DataPPKeeper::const_iterator it = m_dataPPKeeper.begin();


}

Nepomuk::WebExtractor::ResourceAnalyzer::Private::~Private()
{
    //kDebug() << "Deleted: " << uintptr_t(this);
    delete m_fact;
}

void Nepomuk::WebExtractor::ResourceAnalyzer::Private::enqueue(const QSet<const DataPP*> & targets)
{
    foreach(const DataPP * pp, targets) {
        DataPPWrapper * dpp = m_dataPPKeeper[pp];
        Q_CHECK_PTR(dpp);
        m_queue.enqueue(dpp);
    }
}

/*
Nepomuk::Query::Query NW::ResourceAnalyzer::Private::findContextGraphQuery(const Nepomuk::Resource & )
{

    return Nepomuk::Query::Query();
}
*/


//QSet< QString > NW::ResourceAnalyzer::Private::findExaminedDataPP() const
void NW::ResourceAnalyzer::Private::filterExaminedDataPP()
{
    // "Select ?d where { <res> nao:lastModified ?d1 ; <res> ndco:lastExtractionDate ?d2;
    // ?d2 > ?d1; <res> ndco:extractionFinished ?d . }
#if 0
    // TODO Make setVariableName return *this to allow inline creation of queries
    NQ::ComparisonTerm lastModifiedTerm = NQ::ComparisonTerm(
            Soprano::Vocabulary::NAO::lastModified(),
            NQ::ResourceTerm(m_res),
            NQ::ComparisonTerm::Equal
                                          ).inverted();
    lastModifiedTerm.setVariableName("d1");

    NQ::ComparisonTerm lastExtractionTerm = NQ::ComparisonTerm(
            //Soprano::Vocabulary::NAO::lastModified(),
            NQ::ResourceTerm(m_res),
            NQ::ComparisonTerm::Equal
                                            ).inverted();
    lastExtractionTerm.setVariableName("d2");

#endif



#if 0
    QString query = QString(
                        "PREFIX nao: <%1>"
                        "PREFIX rdfs: <%2>"
                        "PREFIX ndco: <%3>"
                        "select ?d where { <%4> nao:lastModified ?d1; <%4> ndco:lastExtractionDate ?d2; ?d1 > ?d2; <%4> ndco:extractionFinished ?d . } ".arg(
                            Soprano::Vocabulary::NAO::naoNamespace().toString(),
                            Soprano::Vocabulary::RDFS::rdfsNamespace().toString(),
                            "ndco?"
                        )
                    )
                    QString rquery = query.arg(m_res.uri().toString());
#endif

    // For each examined DataPP check that the version of examined plugin match current
    // version of this DataPP. If not them mark this DataPP as unexamined
}
Nepomuk::WebExtractor::ResourceAnalyzer::ResourceAnalyzer(
    const DataPPKeeper & dataPPKeeper,
    DecisionFactory * fac,
    WE::MergePolitics mergePolitics,
    WE::LaunchPolitics launchPolitics,
    double acrit,
    double ucrit,
    unsigned int step,
    QObject * parent
):
    QObject(parent)
{
    d = new Nepomuk::WebExtractor::ResourceAnalyzer::Private(dataPPKeeper, fac);
    d->m_mergePolitics = mergePolitics;
    d->m_launchPolitics = launchPolitics;
    d->m_step = step;
    d->m_acrit = acrit;
    d->m_ucrit = ucrit;
    d->m_decisions = fac->newDecisionList();
    //d->it = d->m_dataPPKeeper.begin();
}

Nepomuk::WebExtractor::ResourceAnalyzer::~ResourceAnalyzer()
{
    //kDebug() << "Deleted: " << uintptr_t(this);
    // DecisionFactory is destroyed when Private class is Destroyed
    delete d;

}


void Nepomuk::WebExtractor/*::ResourceAnalyzer*/::ResourceAnalyzer::analyze()
{

    analyze(d->m_res);
}

void Nepomuk::WebExtractor/*::ResourceAnalyzer*/::ResourceAnalyzer::analyze(Nepomuk::Resource & res)
{
    //kDebug() << "This: " << uintptr_t(this) << "D-pointer: " << uintptr_t(this->d);
    if(isRunning()) {
        abort();
        clear();
    }


    // Reset error variable
    d->m_error = NoError;

    // Check that resource is valid
    if(!res.isValid()) {
        // Set error now to provide caller ability to check for errors immidiately
        // and delete ResourceAnalyzer, thus avoiding error signal(I hope Qt
        // doesn't pass signals from destroyed objects.)
        d->m_error = InvalidResource;
        QTimer::singleShot(0, this, SLOT(finishWithError()));
        return;
    }


    // Make some work here
    // Change current resource
    d->m_res = res;
    // Clear all previous data
    clear();
    // Set object for managing examined datapp info
    d->rsd = ResourceServiceData(d->m_res);

    // We check for resource existing AFTER switching from previously analyzed resource.
    // This is done for consistency with setResource() method that allow setting as
    // resource unexisting resource
    if(!res.exists()) {
        d->m_error = UnexistingResource;
        QTimer::singleShot(0, this, SLOT(finishWithError()));
        return;
    }

    // Add all datapp to queue
    QMap< QString, float > examined = d->rsd.examinedDataPPInfo();
    //kDebug() << "Examind info: " << examined;
    foreach(DataPPWrapper * dppw, d->m_dataPPKeeper) {
        // The algorithm is the folowing:
        // First we check examined DataPP that are written into storage. If some of the
        // DataPP is mark as examined, then we check local cache.
        //  This is necessary because when working in iterative
        // mode, information about examined plugins is not written to storage, but is
        // kept localy in examinedDates
        // If some DataPP is marked as examined in storage, but exists in local cache,
        // the we add it to queue. If it is not in local cache, then it is not added to
        // queue
        // See description of ResourceAnalyzer and ResourceAnalyzer::Iterative about
        // why do we need such behaviour
        QString n = dppw->pluginName();
        QMap< QString, float >::const_iterator it = examined.find(n);
        if(it != examined.end()) {
            // Check version
            //kDebug() << "Check version: " << it.value() << " vs " << dppw->pluginVersion();
            if(it.value() == dppw->pluginVersion())
                if(!d->examinedDates.contains(dppw))
                    continue;
        }
        d->m_queue.enqueue(dppw);
    }

    // start processing
    kDebug() << "Extracting data from resource";
    kDebug() << "List of DataPP to use:";
    foreach(DataPPWrapper * dppw, d->m_queue) {
        kDebug() << dppw->pluginName() << ":" << dppw->pluginVersion();
    }

    d->m_running = true;
    d->m_applied = false;
    if(!launchNext()) {
        // Can not analyze - no DataPP or any other problem
        // to avoid infinite recursion the analyzingFinished signal will
        // be called via QTimer::singleShot(0)
        if(d->m_error == NoError) {
            d->m_error = UnknownError;
        }
        d->m_running = false;
        kDebug() << "Failed to analyze resource. Check parameters.";
        QTimer::singleShot(0, this, SLOT(finishWithError()));
    }
}

bool NW::ResourceAnalyzer::isRunning() const
{
    return d->m_running;
}

void NW::ResourceAnalyzer::abort()
{
    if(d->m_running) {
        // This is the only method where m_replies is necessary :(
        // Abort each reply, delete it and clear m_replies
        foreach(DataPPReply * repl, d->m_replies) {
            disconnect(repl, SIGNAL(finished()), this, SLOT(pluginFinished()));
            // FIXME Change error() signal to pluginError slot
            disconnect(repl, SIGNAL(error(DataPPReply::DataPPReplyError)), this, SLOT(pluginFinished()));
            repl->abort();
            delete repl;
        }
        d->m_replies.clear();

        d->m_running = false;
    }
    return;
}

void NW::ResourceAnalyzer::setResource(const Nepomuk::Resource & res)
{
    if(d->m_running) {
        abort();
    }
    if(!res.isValid())
        return;

    // Clear previously collected data
    clear();
    // Set new resource and return
    d->m_res = res;

}


bool Nepomuk::WebExtractor/*::ResourceAnalyzer*/::ResourceAnalyzer::launchNext()
{
    //kDebug() << "DISABLED";
    //return false;
    assert(d->m_respWaits == 0);
    //if (!tmp_count)
    //  return false;

    kDebug() << "Launching next portion of plugins";
    kDebug() << "Total DataPP: " << d->m_dataPPKeeper.size();

    int substop = 0;
    if(d->m_launchPolitics == WE::All)
        substop = d->m_dataPPKeeper.size();
    else if(d->m_launchPolitics == WE::StepWise)
        substop = d->m_step;

    int i = 0;



    for(; ((d->m_queue.size()) and(i < substop)); i++) {
        const DataPPWrapper * dpp = d->m_queue.dequeue();
        //kDebug() << "Datappwrapper: " << uintptr_t(dpp) << " DataPP: " << uintptr_t(dpp->data());

        // launch
        //DataPPReply * repl = dpp->requestDecisions(d->m_fact, d->m_res,this, SLOT( pluginFinished()), SLOT(pluginFinished()));
        DataPPReply * repl = dpp->requestDecisions(d->m_fact, d->m_res);
        // ATTENTION! repl object is executed in another thread!
        //repl->setParent(this);

        if(!repl) {
            kDebug() << "DataPP return 0 as reply. How should I handle it? Ignoring.";
            continue;
        }

        // Connect signals of this reply
        // FIXME Change error() signal to pluginError slot
        connect(repl, SIGNAL(finished()), this, SLOT(pluginFinished()));
        connect(repl, SIGNAL(error(DataPPReply::DataPPReplyError)), this, SLOT(pluginFinished()));

        Q_ASSERT(!d->m_replies.contains(repl));
        d->m_replies.insert(repl);

        //d->m_replyAndRanks[repl] = d->it->second;
        // Increase the number of active replies
        d->m_respWaits++;;

    }

    if(i == 0)
        return false;


    //tmp_count -= c;
    return true;
}

void Nepomuk::WebExtractor::ResourceAnalyzer::launchOrFinish()
{
    if(!launchNext()) {
        // No more plugins to launch and all plugins launched before
        // returned their data

        kDebug() << "Extracting for resource finished";
        kDebug() << "Total decisions count: " << d->m_decisions.size();
        analyzingSessionFinished();
    }
}


// TODO I am not sure that even after deleting the reply there will not be any
// queued signals left. May be it is necessary to add filter that will ignore
// 'obsolete' replies
void Nepomuk::WebExtractor/*::ResourceAnalyzer*/::ResourceAnalyzer::pluginFinished()
{
    //kDebug() << "This: " << uintptr_t(this) << "D-pointer: " << uintptr_t(this->d);
    d->m_respWaits--;
    kDebug() << "Recived answer from plugin.";

    // Process data plugin has returned
    DataPPReply * repl = qobject_cast<DataPPReply*>(QObject::sender());
    if(repl) {
        //Q_ASSERT(d->m_replies.contains(repl));
        // Remove reply from set
        d->m_replies.remove(repl);
        const DataPP * parent = repl->parentDataPP();
        // Error check - check that this reply is not from unknown DataPP
        if(d->m_dataPPKeeper.contains(parent)) {
            // Delete it from map and call deleteLater
            //double repl_rank = d->m_replyAndRanks[repl];
            //double repl_rank = d->m_dataPPKeeper[parent]->rank();


            if(repl->isValid()) {
                kDebug() << "Reply has " << repl->decisions().size() << " decisions";
                // Process Decision list
                d->m_decisions.mergeWith(repl->decisions(), d->m_mergePolitics, d->m_mergeCoff);
                // Mark DataPP as examined. Not in ResourceServiceData, but in
                // internal storage. This is necessary to reduce amount of
                // requests to examined info storage in case ResourceAnalyzer is
                // working in Iterative mode.
                DataPPWrapper * dppw = d->m_dataPPKeeper[parent];
                d->examinedDates[dppw] = QDateTime::currentDateTime();
            } else {
                kDebug() << "Reply is invalid";
            }

            repl->deleteLater();
        } else {
            kDebug() << "Recived answer from unregistred DataPP";
        }
    } else {
        kDebug() << "Recive answer not from DataPPReply object";
    }




    if(d->m_respWaits == 0) {
        // All launched plugins return data
        // Process it
        // Filter obsolete Decisions and add the DataPP that generate
        // this obsolete Decisions back to queue
        // FIXME Fix infinite loop that is possible there
        QSet< const DataPP*> set = d->m_decisions.filterObsolete();
        if(set.size()) {
            d->enqueue(set);
        }

        // Launching other plugins if necessary
        launchOrFinish();
    }
    /*
    else {
       kDebug() << "Only " << m_respWaits << " answers remaining";
    }
    */
}

NW::DecisionList NW::ResourceAnalyzer::decisions() const
{
    return d->m_decisions;
}

void NW::ResourceAnalyzer::apply()
{
    // This function can be called only once.
    if(d->m_applied)
        return;

    // Process data
    if(d->m_decisions.hasAutoApplicable()) {
        d->m_decisions.best().apply();
    } else {
        d->m_decisions.addToUserDiscretion();
    }

    d->m_applied = true;
}

void NW::ResourceAnalyzer::clear()
{
    if(d->m_running)
        return;

    d->m_decisions.clear();
    d->m_examined.clear();
    d->examinedDates.clear();
}

NW::ResourceAnalyzer::AnalyzingPolitics NW::ResourceAnalyzer::analyzingPolitics() const
{
    return d->m_apolitics;
}

void NW::ResourceAnalyzer::setAnalyzingPolitics(AnalyzingPolitics politics)
{
    d->m_apolitics = politics;
}

NW::ResourceAnalyzer::AnalyzingError NW::ResourceAnalyzer::error() const
{
    return d->m_error;
}

void NW::ResourceAnalyzer::analyzingSessionFinished()
{
    if(d->m_apolitics == Iterative) {
        // Relaunch algorithm
        // If there is no decison or no automaticaly applicable decision
        if(d->m_decisions.best().isValid()) {
            // Emit signal
            emit nextIteration();
            apply();
            d->m_applied = false;


            // Relaunch algorithm
            QMetaObject::invokeMethod(this, "analyze", Qt::QueuedConnection);
            return;
        }
    }
    /*
    else if (d->m_apolitics == StepWise)  {
    // Write all necessary data - names,version, data
    d->m_applied = false;
    emit analyzingFinished();
    }
    */
    for(QMap<DataPPWrapper*, QDateTime>::const_iterator it = d->examinedDates.begin();
            it != d->examinedDates.end();
            it++
       ) {
        d->rsd.setExaminedDataPPInfo(it.key()->pluginName(), it.key()->pluginVersion());
        kDebug() << "Mark as examined:" << it.key()->pluginName();
    }

    d->m_applied = false;
    emit analyzingFinished();
}

void NW::ResourceAnalyzer::finishWithError()
{
    emit error(d->m_error);
    emit analyzingFinished();
}

void NW::ResourceAnalyzer::finishWithError(AnalyzingError code)
{
    d->m_error = code;
    finishWithError();
}



/*
void NW::ResourceAnalyzer::setDebugInterrupter( void (*newInterrupter)() )
{
    d->debug_interrupter = newInterrupter;
}
*/
/*
QMap< QString, QString> NW::ResourceAnalyzer::examinedDataPPInfo(const Nepomuk::Resource & res)
{
}

QUrl NW::ResourceAnalyzer::getDecisionMetaGraph(const Nepomuk::Resource & res)
{
    // Find graph or create one
    NQ::Query query = NQ::Query( NQ::ComparisonTerm(NW::Vocabulary::NDCO::decisionMetaGraphFor(),NQ::ResourceTerm(res)).inverted());
    Soprano::QueryResultIterator it = Nepomuk::ResourceManager::instance()->mainModel()->executeQuery(
        query.toSparqlQuery(),Soprano::Query::QueryLanguageSparql
        );
    QUrl graph;
    if ( it.next() ) {
    // Graph exist
    graph = it.binding("r").uri();
    }
    else {
    Nepomuk::ResourceManager * manager = Nepomuk::ResourceManager::instance();
    // Graph doesn't exist. Create it
    graph
    graph = Nepomuk::ResourceManager::instance()->generateUniqueUri( QLatin1String("ctx") );
    Soprano::Node graphNode = Soprano::Node(graph);
    QUrl metaGraphUrl = Nepomuk::ResourceManager::instance()->generateUniqueUri( QLatin1String("ctx") );
    Soprano::Node metaGraphNode = Soprano::Node(metaGraphUrl);

    manager->mainModel()->addStatement( graphNode,
                          Soprano::Vocabulary::RDF::type(),
                          Soprano::Vocabulary::NRL::InstanceBase(),
                          metaGraphNode );
    manager->mainModel()->addStatement( graphNode,
                          Soprano::Vocabulary::NAO::created(),
                          Soprano::LiteralValue( QDateTime::currentDateTime() ),
                          metaGraphNode );
    manager->mainModel()->addStatement( metaGraphNode,
                          Soprano::Vocabulary::RDF::type(),
                          Soprano::Vocabulary::NRL::GraphMetadata(),
                          metaGraphNode );
    manager->mainModel()->addStatement( metaGraphNode,
                          Soprano::Vocabulary::NRL::coreGraphMetadataFor(),
                          graphNode,
                          metaGraphNode );
    manager->mainModel()->addStatement( graphNode,
                          NW::Vocabulary::NDCO::decisionMetaGraphFor(),
                          res.resourceUri(),
                          metaGraphNode );
    }
    return graph;
}
void NW::ResourceAnalyzer::setExaminedDataPPInfo( Nepomuk::Resource & res, const  QString & name,const QString & version)
{

}
*/
#if 0
void NW::ResourceAnalyzer::clearObsoleteExaminedDataPPInfo(Nepomuk::Resource & res, int expirationInterval)
{
    // If lastExtractionDate is less then last modification date, then remove all
    // information about examined DataPP
    // If there is no lastModificated property available, then all resources  that has
    // currentDate - lastExtractionDate > expirationInterval are cleaned
    if(!res.hasProperty(NW::Vocabulary::NDCO::lastExtracted())) {
        // There must not be any DataPP. But in case of any errors clean this resource
        // and if any resource was found then print error message
        /*
        NQ::Query query( NQ::ComparisonTerm(
            Nepomuk::WebExtractor::Vocabulary::NDCO::extractionFinished(),
            NQ::ResourceTerm(res)
            ).inverted()
        );

        while
        */
        return;
    }
    Nepomuk::Variant ledatev = res
    if(res.hasProperty(Soprano::Vocabulary::NAO::lastModified())) {
        // Look for interval

        QDateTime lmdate = res.property(Soprano::Vocabulary::NAO::lastModified()).toDateTime();
        QDateTime ledate = res.property(NW::Vocabulary::NDCO::lastExtracted()).toDateTime();

        int interval = ledate.secsTo(lmdate);

        if(interval > 0) {    // If last extraction date < last modification date
            // Remove all DataPP
            NQ::Query query(NQ::ComparisonTerm(
                                Nepomuk::WebExtractor::Vocabulary::NDCO::extractionFinished(),
                                NQ::ResourceTerm(res)
                            ).inverted()
                           );
        }

    }
}

#endif