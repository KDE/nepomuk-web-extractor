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

#include "webexcatschedulerimpl.h"
#include <assert.h>
#include <KDebug>
#include <Soprano/Model>
#include <Soprano/Vocabulary/NAO>
#include <Soprano/Vocabulary/NRL>
#include <Soprano/Node>
#include <Nepomuk/ResourceManager>
#include "webexqueries.h"
#include <webextractor/parameters.h>

namespace NW = Nepomuk::WebExtractor;

Nepomuk::WebExtractorCategorySchedulerImpl::WebExtractorCategorySchedulerImpl(
	const QString & category_query, 
	WebExtractorCategoryScheduler * parent, 
	QSharedPointer< const WE::ExtractParameters > extractParams,
	int maxResSimult, 
	int cacheSize
	):
    // DO not pass parent as QObject parent - 
    //  QThread( and WebExtractorCategoryScheduler is a
    //  sublcass of QThread) 
    //  doesn't live in the thread it executes.
    QObject(0),
    m_query(category_query),
    m_respWaits(0),
    m_maxResSimult(maxResSimult),
    m_currentResProc(0),
    m_cacheSize(cacheSize),
    m_extractParams(extractParams)
    //tmp_count(100)
{
    // Restore defaults if incorrect caches size was passed
    if (m_cacheSize < 1) {
	kDebug() << "Cache size ("<<m_cacheSize<<") is incorrect. Use 5 instead";
	m_cacheSize = 5;
    }

    m_factory = new Nepomuk::WebExtractor::ResourceAnalyzerFactory(m_extractParams,this);
    m_par = parent;
    connect(this, SIGNAL(launchPls(QUrl)), this, SLOT(launch(const QUrl &)), Qt::QueuedConnection );

    // Wrap into section.
    if (!checkQuery())
	    kDebug() << "select query is invalid";
    // Because somebody thinks that locking a model while iterating over
    // it is a good idea, it is necessary to cash results into queue 
    // and delete iterator to unlock model
    //void cacheUrls();
}


bool Nepomuk::WebExtractorCategorySchedulerImpl::checkQuery()
{
	Soprano::QueryResultIterator it =  
	    Nepomuk::ResourceManager::instance()->mainModel()->executeQuery( 
	    m_query,Soprano::Query::QueryLanguageSparql 
	    );

	return it.isValid();
}
void Nepomuk::WebExtractorCategorySchedulerImpl::cacheUrls()
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
bool Nepomuk::WebExtractorCategorySchedulerImpl::start()
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

void Nepomuk::WebExtractorCategorySchedulerImpl::launchOrFinish()
{
    if (m_finishing) {
	kDebug()<<"Fihishing. "<<m_currentResProc << " responces left";
	// No launches more
	// Just wait untill all already launched
	// resource finishing and exit
	if (m_currentResProc == 0)
	    m_par->quit();
    }
    else {
	    if (m_par->waitForContinue() ) {
		// launch new resource
		if (!launchNext()){
		    // Finishing
		    m_finishing = true;
		}
	    }
	    else {
		m_finishing = true;
		kDebug()<<"Fihishing. "<<m_currentResProc << " responces left";
		if (m_currentResProc == 0)
		    m_par->quit();
	    }
    }
}

bool Nepomuk::WebExtractorCategorySchedulerImpl::launchNext()
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

void Nepomuk::WebExtractorCategorySchedulerImpl::launch(const QUrl & resourceUri)
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
    m_currentResProc++;
    resanal->analyze(res);
}

void Nepomuk::WebExtractorCategorySchedulerImpl::resourceProcessingAborted()
{
    launchOrFinish();
}
void Nepomuk::WebExtractorCategorySchedulerImpl::resourceProcessed()
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
