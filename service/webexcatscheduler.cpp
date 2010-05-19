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
#include "webexcatschedulerimpl.h"

namespace NW = Nepomuk::WebExtractor;

Nepomuk::WebExtractorCategoryScheduler::WebExtractorCategoryScheduler(
	const QString & category_query,
       	QObject * parent,
	NW::ExtractParametersPtr params,
       	int maxResSimult
	):
    QThread(parent),
    m_query(category_query),
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
    if (m_cacheSize < 1) {
	kDebug() << "Cache size ("<<m_cacheSize<<") is incorrect. Use 5 instead";
	m_cacheSize = 5;
    }

    m_factory = new Nepomuk::WebExtractor::ResourceAnalyzerFactory(m_extractParams,this);
    connect(this, SIGNAL(launchPls(QUrl)), this, SLOT(launch(const QUrl &)), Qt::QueuedConnection );

    // Wrap into section.
    if (!checkQuery())
	    kDebug() << "select query is invalid";
    // Because somebody thinks that locking a model while iterating over
    // it is a good idea, it is necessary to cash results into queue 
    // and delete iterator to unlock model
    //void cacheUrls();
}


Nepomuk::WebExtractorCategoryScheduler::~WebExtractorCategoryScheduler()
{
    if (isRunning()) {
        QMutexLocker locker( &m_resumeStopMutex );
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
	    m_query,Soprano::Query::QueryLanguageSparql 
	    );

	return it.isValid();
}
void Nepomuk::WebExtractorCategoryScheduler::cacheUrls()
{
	Soprano::QueryResultIterator it =  
	    Nepomuk::ResourceManager::instance()->mainModel()->executeQuery( 
	    m_query,Soprano::Query::QueryLanguageSparql 
	    );
    while( it.next() and ( m_urlQueue.size() < m_cacheSize) )
    {
	m_urlQueue.enqueue(it.binding( Nepomuk::WebExtractorQueries::resourceVariableName() ).uri());
    }
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
    while ( i < m_maxResSimult) {
	if (!launchNext())
	    break;
	i++;
    }

    if ( i == 0)
       return false;
    else 
	return true;
    
}

void Nepomuk::WebExtractorCategoryScheduler::launchOrFinish()
{
    kDebug()<<m_currentResProc << " responces left";
    bool stp;
    //QMutexLocker locker(&m_resumeStopMutex);
    stp = m_stopped;
    //locker.unlock();

    if (stp) {
	kDebug()<<"Fihishing. "<<m_currentResProc << " responces left";
	// No launches more
	// Just wait untill all already launched
	// resource finishing and exit
	if (m_currentResProc == 0)
	    this->quit();
    }
    else {
	    if (waitForContinue() ) {
		// launch new resource
		if (!launchNext()){
		    // Finishing
	//	    m_finishing = true;
		    kDebug()<<"Fihishing. "<<m_currentResProc << " responces left";
		    if (m_currentResProc == 0)
			this->quit();
		}
	    }
	    else {
	//	m_finishing = true;
		kDebug()<<"Fihishing. "<<m_currentResProc << " responces left";
		if (m_currentResProc == 0)
		    this->quit();
	    }
    }
}

bool Nepomuk::WebExtractorCategoryScheduler::launchNext()
{
    // if queue is empty then try to fill it
    if (!m_urlQueue.size()) {
	cacheUrls();
	// if queue is still empty then there are no more
	// resource to process
	if (!m_urlQueue.size()) 
	    return false;
    }

    // Now there are elements in queue

    emit launchPls(m_urlQueue.dequeue());


    return true;

}

void Nepomuk::WebExtractorCategoryScheduler::launch(const QUrl & resourceUri)
{
    Nepomuk::Resource res(resourceUri);
    // If resource doesn't exist(has already removed)
    if (!res.isValid()) {
	kDebug() << "Resource "<<resourceUri<<" doesn't exist any more and will be skiped";
	launchOrFinish();
	return;
    }

    NW::ResourceAnalyzer * resanal = m_factory->newAnalyzer();

    kDebug() << "Start extracting for resource: " << 
	resourceUri;
	

    connect(resanal, SIGNAL(analyzingFinished()), this, SLOT(resourceProcessed()));
    //QTimer::singleShot(3*1000, this, SLOT(resourceProcessed()));
    m_currentResProc++;
    resanal->analyze(res);
    kDebug() << "Exit";
}

void Nepomuk::WebExtractorCategoryScheduler::resourceProcessingAborted()
{
    launchOrFinish();
}
void Nepomuk::WebExtractorCategoryScheduler::resourceProcessed()
{
    m_currentResProc--;
    NW::ResourceAnalyzer * res = qobject_cast< NW::ResourceAnalyzer *>(sender() );
    if (!res) {
	kDebug() << "Recive signal not from ResourceAnalyzer";
    }
    else {
	m_factory->deleteAnalyzer(res);
    }
    launchOrFinish();
}
void Nepomuk::WebExtractorCategoryScheduler::suspend()
{
    if ( isRunning() )  {
        QMutexLocker locker( &m_resumeStopMutex );
	if (!m_suspended) { 
	    m_suspended = true;
	    emit extractingSuspended( true );
	}
    }
}


void Nepomuk::WebExtractorCategoryScheduler::resume()
{
    if ( isRunning() )  {
        QMutexLocker locker( &m_resumeStopMutex );
	if ( m_suspended ) {
	    m_suspended = false;
	    m_resumeStopWc.wakeAll();
	    emit extractingSuspended( false );
	}
    }
}


void Nepomuk::WebExtractorCategoryScheduler::setSuspended( bool suspended )
{
    if ( suspended )
        suspend();
    else
        resume();
}


void Nepomuk::WebExtractorCategoryScheduler::stop()
{
    if ( isRunning() ) {
        QMutexLocker locker( &m_resumeStopMutex );
        m_stopped = true;
        m_suspended = false;
        m_resumeStopWc.wakeAll();
    }
}


void Nepomuk::WebExtractorCategoryScheduler::restart()
{
    stop();
    wait();
    start();
}

void Nepomuk::WebExtractorCategoryScheduler::setExtractingSpeed( ExtractingSpeed speed )
{
    kDebug() << speed;
    m_speed = speed;
}

void Nepomuk::WebExtractorCategoryScheduler::setExtractingStarted( bool started )
{
    if ( started != m_extracting ) {
        m_extracting = started;
        if ( m_extracting )
            emit extractingStarted();
        else
            emit extractingStopped();
    }
}

void Nepomuk::WebExtractorCategoryScheduler::setReducedExtractingSpeed( bool reduced )
{
    if ( reduced )
        setExtractingSpeed( ReducedSpeed );
    else
        setExtractingSpeed( FullSpeed );
}


bool Nepomuk::WebExtractorCategoryScheduler::isSuspended() const
{
    return isRunning() && m_suspended;
}


bool Nepomuk::WebExtractorCategoryScheduler::isExtracting() const
{
    return m_extracting;
}

void Nepomuk::WebExtractorCategoryScheduler::setMaxResSimult(int new_mrsm)
{
    if (new_mrsm < 1)
	new_mrsm = 1;
    m_maxResSimult = new_mrsm;
}

void Nepomuk::WebExtractorCategoryScheduler::run()
{
    
    /*
    if (m_impl)
	delete m_impl;
	*/

    if (m_extractParams.isNull() ) {
	kDebug() << "Extracting parameters is null. Ignoring launch";
    }

    //kDebug() << *m_extractParams;
    
    //m_impl = new (std::nothrow) WebExtractorCategorySchedulerImpl(m_query,this,m_extractParams, m_maxResSimult);
    /*
    if (!m_impl)
	return;
	*/
    // set lowest priority for this thread
    setPriority( QThread::IdlePriority );

    setExtractingStarted( true );

    // Start query
    /*
    it = Nepomuk::ResourceManager::instance()->mainModel()->executeQuery( 
	    m_query,Soprano::Query::QueryLanguageSparql 
	    );
	    */




#if 0
    // Do work here
    while ( waitForContinue() ) {
        // wait for more dirs to analyze in case the initial
        // indexing is done

        // wait for resume or stop (or simply continue)
        if ( !waitForContinue() ) {
            break;
        }

    }
#endif
    // start return true if there is anything to process
    // and false otherwise. If event loop is started when
    // start returned false then it will never end,
    // because nothing will call quit();
    QTimer * t = new QTimer();
    connect(t,SIGNAL(timeout()), this, SLOT(mseg()));
    t->start(1000*5);
    if ( startLaunch()) {
	kDebug() << "Starting scheduler main loop";
	exec();
	kDebug() << "Scheduler main loop finished";
    }
    else {
	kDebug() << "Category has no resource to process. Returning";
    }

    setExtractingStarted( false );

    // reset state
    m_suspended = false;
    m_stopped = false;
}

void Nepomuk::WebExtractorCategoryScheduler::mseg()
{
    kDebug() << "Timer message";
}

bool Nepomuk::WebExtractorCategoryScheduler::waitForContinue( bool disableDelay )
{
    kDebug()<<"enter";
    QMutexLocker locker( &m_resumeStopMutex );
    if ( m_suspended ) {
	kDebug() << "Suspended";
        setExtractingStarted( false );
        m_resumeStopWc.wait( &m_resumeStopMutex );
        setExtractingStarted( true );
	kDebug() << "Resumed";
    }
    else if ( !disableDelay && m_speed != FullSpeed ) {
        msleep( m_speed == ReducedSpeed ? m_reducedSpeedDelay : m_snailPaceDelay );
    }

    if (m_stopped) {
	kDebug() << "Stopped";
    }

    return !m_stopped;
}

