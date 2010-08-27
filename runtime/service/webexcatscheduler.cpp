/*
   Copyright (c) 2008-2010 Sebastian Trueg <trueg@kde.org>
   Copyright (C) 2010 by Serebriyskiy Artem <v.for.vandal@gmail.com>

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
#include <QtCore/QMutexLocker>
#include <QtCore/QDateTime>
#include <QtCore/QTimer>
#include <assert.h>
#include <KDebug>
#include <new>
#include <Soprano/Model>
#include <Nepomuk/ResourceManager>
#include <Soprano/Vocabulary/NAO>
#include <Soprano/Vocabulary/NRL>
#include <Soprano/Node>
#include <Nepomuk/ResourceManager>
#include "webexqueries.h"
#include "webexcatscheduler.h"
#include "resourceservicedatamanager.h"
#include "resourceservicedata.h"
#include "resourceanalyzer.h"

namespace NW = Nepomuk::WebExtractor;
namespace NQ = Nepomuk::Query;

Nepomuk::WebExtractorCategoryScheduler::WebExtractorCategoryScheduler(
    const NQ::Query & category_query,
    QObject * parent,
    NW::ExtractParametersPtr params,
    bool optimizeForNepomuk,
    int maxResSimult
):
    QThread(parent),
    m_query(category_query),
    // Force to false because infrastructure is not ready yet
    m_optimizeNepomuk(false),
    m_respWaits(0),
    m_maxResSimult(maxResSimult),
    m_currentResProc(0),
    m_cacheSize(5),
    m_suspended(false),
    m_stopped(false),
    m_extracting(false),
    m_reducedSpeedDelay(500),
    m_snailPaceDelay(3000),
    m_speed(FullSpeed)
{
    m_extractParams = params;
    //kDebug() << *m_extractParams;

    // Restore defaults if incorrect caches size was passed
    if(m_cacheSize < 1) {
        kDebug() << "Cache size (" << m_cacheSize << ") is incorrect. Use 5 instead";
        m_cacheSize = 5;
    }

    m_factory = new Nepomuk::WebExtractor::ResourceAnalyzerFactory(m_extractParams, this);

    // Create necessary amount of analyzers
    for(int i = 0; i < m_maxResSimult; i++) {
        Nepomuk::WebExtractor::ResourceAnalyzer * analyzer = m_factory->newAnalyzer();
        if(!analyzer)
            break;

        connect(analyzer, SIGNAL(analyzingFinished()), this, SLOT(resourceProcessed()));

        m_availableAnalyzers.enqueue(analyzer);
    }
    //connect(this, SIGNAL(launchPls(QUrl)), this, SLOT(launch(const QUrl &)), Qt::QueuedConnection );

    // Wrap into section.
    if(!checkQuery())
        kDebug() << "select query is invalid";
    // Because somebody thinks that locking a model while iterating over
    // it is a good idea, it is necessary to cash results into queue
    // and delete iterator to unlock model
    //void cacheUrls();
}


Nepomuk::WebExtractorCategoryScheduler::~WebExtractorCategoryScheduler()
{
    if(isRunning()) {
        QMutexLocker locker(&m_resumeStopMutex);
        m_stopped = true;
        m_suspended = false;
        setExtractingStarted(false);
        m_resumeStopWc.wakeAll();
        wait();
    }
}

bool Nepomuk::WebExtractorCategoryScheduler::checkQuery()
{
    Soprano::QueryResultIterator it =
        Nepomuk::ResourceManager::instance()->mainModel()->executeQuery(
            m_query.toSparqlQuery(), Soprano::Query::QueryLanguageSparql
        );

    return it.isValid();
}
void Nepomuk::WebExtractorCategoryScheduler::cacheUrls()
{
    // If there are enough url, return
    if(m_urlQueue.size() == m_cacheSize)
        return;

    // First we will execute query to check that there is any resource
    QMap< QString, int> dataPPInfo = m_extractParams->dataPPInfo();
    NW::ResourceServiceDataManager * rsdManager = m_extractParams->resourceServiceDataManager();
    NQ::Query advancedQuery = rsdManager->queryUnparsedResources(m_query.term(), dataPPInfo, Nepomuk::ResourceManager::instance()->mainModel());

    if(advancedQuery.isValid()) {
        kDebug() << "Using advanced query";
        // Then we can execute this advanced query without checkings
        advancedQuery.setLimit(m_cacheSize - m_urlQueue.size() + 1);
        Soprano::QueryResultIterator it =
            Nepomuk::ResourceManager::instance()->mainModel()->executeQuery(
                advancedQuery.toSparqlQuery(), Soprano::Query::QueryLanguageSparql
            );

        while(it.next()) {
            m_urlQueue.enqueue(it.binding("r").uri());
        }
    } else { // This is hard way. There is no query that will check for examined DataPP info.
        // This checks must be done manually. For each resource uri, return by query,
        // the ResourceServiceData with aproprite backend must be created and the
        // resource should be checked whether there is any unexamined DataPP
        NW::ResourceServiceDataManager * rsdManager = m_extractParams->resourceServiceDataManager();

        kDebug() << "Category has query: " << m_query.toSparqlQuery();

        Soprano::QueryResultIterator it =
            Nepomuk::ResourceManager::instance()->mainModel()->executeQuery(
                m_query.toSparqlQuery(), Soprano::Query::QueryLanguageSparql
            );

        while(it.next() /*&& (m_urlQueue.size() < m_cacheSize)*/) {
            Nepomuk::Resource  res(it.binding("r").uri());
            kDebug() << "Checking resource: " << res.resourceUri();
            NW::ResourceServiceData rsd(res, rsdManager);
            if(!rsd.checkExaminedDataPPInfo(dataPPInfo))
                m_urlQueue.enqueue(res.resourceUri());
        }
    }
#warning IMPLEMENT THIS

}
bool Nepomuk::WebExtractorCategoryScheduler::startLaunch()
{
    kDebug() << "Starting category.";
    /*
    kDebug() << *(
        const_cast<Nepomuk::WebExtractor::ExtractParameters*>(m_extractParams.data())
        );
        */
    int i = 0;
    while(i < m_availableAnalyzers.size()) {
        if(!launchNext())
            break;
        i++;
    }

    if(i == 0)
        return false;
    else
        return true;

}

void Nepomuk::WebExtractorCategoryScheduler::launchOrFinish()
{
    kDebug() << m_currentResProc << " responces left";
    bool stp;
    //QMutexLocker locker(&m_resumeStopMutex);
    stp = m_stopped;
    //locker.unlock();

    if(stp) {
        kDebug() << "Fihishing. " << m_currentResProc << " responces left";
        // No launches more
        // Just wait untill all already launched
        // resource finishing and exit
        if(m_currentResProc == 0)
            this->__finish();
    } else {
        if(waitForContinue()) {
            // launch new resource
            if(!launchNext()) {
                // Finishing
                //      m_finishing = true;
                kDebug() << "Fihishing. " << m_currentResProc << " responces left";
                if(m_currentResProc == 0) {
                    this->__finish();
                }
            }
        } else {
            //  m_finishing = true;
            kDebug() << "Fihishing. " << m_currentResProc << " responces left";
            if(m_currentResProc == 0)
                this->__finish();
        }
    }
}

/*! \brief Try to launch analyzing of the next resource.
 * Return true if succeed, false otherwise. False mean that
 * there is no more resources to analyze
 */
bool Nepomuk::WebExtractorCategoryScheduler::launchNext()
{
    // check that there is any resourceanalyzer in queue
    if(!m_availableAnalyzers.size()) {
        kDebug() << "There is no free analyzer in queue. Please report this message as a bug";
        return false;
    }

    // if queue is empty then try to fill it
    if(m_urlQueue.isEmpty()) {
        cacheUrls();
        // if queue is still empty then there are no more
        // resource to process
        if(m_urlQueue.isEmpty())
            return false;
    }

    // Now there are elements in queue

    //emit launchPls(m_urlQueue.dequeue());
    launch(m_urlQueue.dequeue());


    return true;

}

void Nepomuk::WebExtractorCategoryScheduler::launch(const QUrl & resourceUri)
{
    Nepomuk::Resource res(resourceUri);
    // If resource doesn't exist(has already removed)
    if(!res.exists()) {
        kDebug() << "Resource " << resourceUri << " doesn't exist any more and will be skiped";
        launchOrFinish();
        return;
    }

    //NW::ResourceAnalyzer * resanal = m_factory->newAnalyzer();

    // Take resourceanalyzer from queue
    NW::ResourceAnalyzer * resanal = m_availableAnalyzers.dequeue();
    Q_ASSERT(resanal);

    resanal->setResource(resourceUri);


    kDebug() << "Start extracting for resource: " <<
             resourceUri;


    //connect(resanal, SIGNAL(analyzingFinished()), this, SLOT(resourceProcessed()));
    //QTimer::singleShot(3*1000, this, SLOT(resourceProcessed()));
    m_currentResProc++;
    resanal->analyze(res);
    //kDebug() << "Exit";
}

void Nepomuk::WebExtractorCategoryScheduler::resourceProcessingAborted()
{
#warning IMPLEMENT THIS!
    launchOrFinish();
}
void Nepomuk::WebExtractorCategoryScheduler::resourceProcessed()
{
    m_currentResProc--;
    NW::ResourceAnalyzer * res = qobject_cast< NW::ResourceAnalyzer *>(sender());
    if(!res) {
        kDebug() << "Recive signal not from ResourceAnalyzer";
    } else {
        //m_factory->deleteAnalyzer(res);

        // Add analyzer back to queue
        m_availableAnalyzers.enqueue(res);
    }
    launchOrFinish();
}
void Nepomuk::WebExtractorCategoryScheduler::suspend()
{
    if(isRunning())  {
        QMutexLocker locker(&m_resumeStopMutex);
        if(!m_suspended) {
            m_suspended = true;
            emit extractingSuspended(true);
        }
    }
}


void Nepomuk::WebExtractorCategoryScheduler::resume()
{
    if(isRunning())  {
        QMutexLocker locker(&m_resumeStopMutex);
        if(m_suspended) {
            m_suspended = false;
            m_resumeStopWc.wakeAll();
            emit extractingSuspended(false);
        }
    }
}


void Nepomuk::WebExtractorCategoryScheduler::setSuspended(bool suspended)
{
    if(suspended)
        suspend();
    else
        resume();
}


void Nepomuk::WebExtractorCategoryScheduler::stop()
{
    if(isRunning()) {
        /*
            QMutexLocker locker( &m_resumeStopMutex );
            m_stopped = true;
            m_suspended = false;
        */
        this->quit();
        m_resumeStopWc.wakeAll();
    }
}


void Nepomuk::WebExtractorCategoryScheduler::restart()
{
    stop();
    wait();
    clear();
    start();
}

void Nepomuk::WebExtractorCategoryScheduler::clear()
{
    if(isRunning())
        return;

    m_urlQueue.clear();
}

void Nepomuk::WebExtractorCategoryScheduler::setExtractingSpeed(ExtractingSpeed speed)
{
    kDebug() << speed;
    m_speed = speed;
}

void Nepomuk::WebExtractorCategoryScheduler::setExtractingStarted(bool started)
{
    if(started != m_extracting) {
        m_extracting = started;
        if(m_extracting)
            emit extractingStarted();
        else
            emit extractingStopped();
    }
}

void Nepomuk::WebExtractorCategoryScheduler::setReducedExtractingSpeed(bool reduced)
{
    if(reduced)
        setExtractingSpeed(ReducedSpeed);
    else
        setExtractingSpeed(FullSpeed);
}


bool Nepomuk::WebExtractorCategoryScheduler::isSuspended() const
{
    return isRunning() && m_suspended;
}


bool Nepomuk::WebExtractorCategoryScheduler::isExtracting() const
{
    return m_extracting;
}

bool Nepomuk::WebExtractorCategoryScheduler::isReady()
{
    // If category is running, then this method must not be called
    if(isRunning()) {
        kError() << "Category is running! Don't call isReady()";
        return false;
    }

    // The idea of checkings is:
    // First clear previously cached Url. This is necessary because since they
    // were collected, they may disappear from Nepomuk
    // Then, execute cacheUrls(). If after that m_urlQueue will be nonEmpty, then
    // there are resources to work with.
    m_urlQueue.clear();

    cacheUrls();

    return m_urlQueue.size();

}
void Nepomuk::WebExtractorCategoryScheduler::setMaxResSimult(int new_mrsm)
{
#warning Implement this function propertly
    if(new_mrsm < 1)
        new_mrsm = 1;
    m_maxResSimult = new_mrsm;
}

void Nepomuk::WebExtractorCategoryScheduler::run()
{

    /*
    if (m_impl)
    delete m_impl;
    */

    if(m_extractParams.isNull()) {
        kDebug() << "Extracting parameters is null. Ignoring launch";
        return;
    }

    //kDebug() << *m_extractParams;

    //m_impl = new (std::nothrow) WebExtractorCategorySchedulerImpl(m_query,this,m_extractParams, m_maxResSimult);
    /*
    if (!m_impl)
    return;
    */
    // set lowest priority for this thread
    setPriority(QThread::IdlePriority);

    setExtractingStarted(true);

    // Start query
    /*
    it = Nepomuk::ResourceManager::instance()->mainModel()->executeQuery(
        m_query,Soprano::Query::QueryLanguageSparql
        );
        */




#if 0
    // Do work here
    while(waitForContinue()) {
        // wait for more dirs to analyze in case the initial
        // indexing is done

        // wait for resume or stop (or simply continue)
        if(!waitForContinue()) {
            break;
        }

    }
#endif
    // start return true if there is anything to process
    // and false otherwise. If event loop is started when
    // start returned false then it will never end,
    // because nothing will call quit();
    if(startLaunch()) {
        kDebug() << "Starting scheduler main loop";
        exec();
        kDebug() << "Scheduler main loop finished";
    } else {
        kDebug() << "Category has no resource to process. Returning";
    }

    setExtractingStarted(false);

    // reset state
    m_suspended = false;
    m_stopped = false;
}

/*
void Nepomuk::WebExtractorCategoryScheduler::mseg()
{
    kDebug() << "Timer message";
}
*/

void Nepomuk::WebExtractorCategoryScheduler::__finish()
{
    m_urlQueue.clear();
    this->quit();
}

bool Nepomuk::WebExtractorCategoryScheduler::waitForContinue(bool disableDelay)
{
    kDebug() << "enter";
    QMutexLocker locker(&m_resumeStopMutex);
    if(m_suspended) {
        kDebug() << "Suspended";
        setExtractingStarted(false);
        m_resumeStopWc.wait(&m_resumeStopMutex);
        setExtractingStarted(true);
        kDebug() << "Resumed";
    } else if(!disableDelay && m_speed != FullSpeed) {
        msleep(m_speed == ReducedSpeed ? m_reducedSpeedDelay : m_snailPaceDelay);
    }

    if(m_stopped) {
        kDebug() << "Stopped";
    }

    return !m_stopped;
}

