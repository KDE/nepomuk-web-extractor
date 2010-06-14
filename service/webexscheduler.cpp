/*
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

#include "webexscheduler.h"
#include <Soprano/QueryResultIterator>
#include <Soprano/Model>
#include <Soprano/Vocabulary/NAO>
#include <Soprano/Vocabulary/NRL>
#include <Soprano/Node>
#include <Nepomuk/ResourceManager>
#include <assert.h>
#include <QtGlobal> 
#include "webexqueries.h"


Nepomuk::WebExtractorScheduler::WebExtractorScheduler(WebExtractorSettings * conf, QObject * parent):
    QObject(parent),
    m_success(false),
    m_stopped(false)
{
    m_conf=conf;
    readConfig();
    if (m_success) {
	 m_sm = new QSignalMapper(this);
	 m_timerSM = new QSignalMapper(this);
	 m_launchTimer = new QTimer(this);
	 m_launchTimer->setSingleShot(false);
	 connect(m_sm, SIGNAL(mapped(const QString &)),
		     this, SLOT(categoryFinished( const QString &))
			 );
	 connect(m_timerSM, SIGNAL(mapped(const QString &)),
		     this, SLOT(addToQueue( const QString &))
			 );
	 connect(m_launchTimer, SIGNAL(timeout()),
		 this, SLOT(launchNext())
		 );
    }

}

void Nepomuk::WebExtractorScheduler::readConfig()
{
    m_success = false;
    // Get parameters
    m_maxCatSimult = m_conf->maxCategoriesSimult();
    kDebug()<<"Max categories simultaneously"<<m_maxCatSimult;
    if (m_maxCatSimult < 1)
	m_maxCatSimult = 1;
    // const QHash<QString,WebExCategory*> & cats = m_conf->categories();
    //m_queries = cats.size();
    //QHash<QString,WebExCategory*>::const_iterator it = cats.begin();
    /*
    for(; it != cats.end(); ++it)
    {
	m_askQueries[it.key()] = Nepomuk::WebExtractorQueries::ask_query(
		it.value()->queryPrefix(),
		it.value()->query()
		);
	m_selectQueries[it.key()] = Nepomuk::WebExtractorQueries::select_query(
		it.value()->queryPrefix(),
		it.value()->query()
		);
    }
    */
    QStringList cats = m_conf->categories();
    QString q, qp;
    foreach(const QString  & catname, cats)
    {
	// Check that this category has any assigned DataPP.
	// If it has not, then ignore it.
	WebExtractor::ExtractParametersPtr params = m_conf->extractParameters(catname);
	if (!params->hasAnyDataPP() ) {
	    kDebug() << "Category "<<catname<<" has no assigned DataPP";
	    continue;
	}
	else {
	    kDebug() << "Category "<<catname<< "has "<< params->dataPPCount() << "assigned DataPP";
	}

	q = m_conf->queryPrefix(catname);
	qp = m_conf->query(catname);

	QString aq = Nepomuk::WebExtractorQueries::ask_query(
		q,qp
		);

	Soprano::QueryResultIterator it = 
	    Nepomuk::ResourceManager::instance()->mainModel()->executeQuery( 
	    aq, Soprano::Query::QueryLanguageSparql 
	    );

	if (!it.isValid() ) {
	    kDebug() << "Query \" " << catname << " has invalid syntax";
	    continue;
	}

	m_askQueries[catname] = aq;

	m_selectQueries[catname] = Nepomuk::WebExtractorQueries::select_query(
		q,qp
		);

    }
    m_success = true;
}

bool Nepomuk::WebExtractorScheduler::addToQueue(const QString & name)
{
    // If signal come after restarting
    if (!m_categories.contains(name) )
	return false;

    // If category is already scheduled for execution
    if (m_launchQueue.contains(name))
	return false;
    
    // If category is already launched then ignore it
    if (m_launchedQueries.contains(name))
	if (m_launchedQueries[name]->isRunning())
	    return false;

    // Check thar category has any resource to process
    Soprano::QueryResultIterator it = 
	Nepomuk::ResourceManager::instance()->mainModel()->executeQuery( 
	m_askQueries[name],Soprano::Query::QueryLanguageSparql 
	);
    if (it.boolValue()) {
	// Add to launch queue
	m_launchQueue.insert(name);
	return true;
    }
    else {
	// launch timer again
	QHash<QString, QTimer*>::iterator tit = m_timers.find(name);
	if (tit != m_timers.end()) {
	    // launch timer
	    tit.value()->start(m_conf->interval(name));
	}
    }
    return false;
}

bool Nepomuk::WebExtractorScheduler::isSuspended() const
{
    clearLaunched();
    if (m_launchedQueries.isEmpty())
	return false;

    QHash<QString,WebExtractorCategoryScheduler*>::iterator qit = m_launchedQueries.begin();
    for(; qit != m_launchedQueries.end(); ++qit)
    {
	if (!qit.value()->isSuspended() ) {
	    return false;
	}
    }

    return true;
}

bool Nepomuk::WebExtractorScheduler::isExtracting() const
{
    clearLaunched();
    if (m_launchedQueries.isEmpty())
	return false;
    QHash<QString,WebExtractorCategoryScheduler*>::iterator qit = m_launchedQueries.begin();
    for(; qit != m_launchedQueries.end(); ++qit)
    {
	if (qit.value()->isExtracting() ) {
	    return true;
	}
    }

    return false;
    
}
void Nepomuk::WebExtractorScheduler::clearLaunched() const
{
    QHash<QString,WebExtractorCategoryScheduler*>::iterator qit = m_launchedQueries.begin();
    while( qit != m_launchedQueries.end())
    {
	if (qit.value()->isFinished() ) {
	    qit = m_launchedQueries.erase(qit);
	}
	else {
	    ++qit;
	}
    }
}
void Nepomuk::WebExtractorScheduler::launchNext()
{
    kDebug() << "Atempt to launch";
    // First clear from m_launchedQueries queries that has finished
    clearLaunched();

    // If there is nothing in launchQueue
    if (m_launchQueue.isEmpty())
	return;

    //int stop = qMin(m_launchQueue.size(),m_maxCatSimult - m_launchedQueries.size());
    int stop = m_maxCatSimult - m_launchedQueries.size();
    
    assert(stop >= 0);
    
    // If there is no free slot to launch new category, return
    if (!stop)
	return;

    int i = 0;
    QSet<QString>::iterator sit = m_launchQueue.begin();
    while( 
	    (sit != m_launchQueue.end()) && (i < stop)
       )
    {
	// Take element from launch queue
	// and launch it if it is not already launched
	if (!m_launchedQueries.contains(*sit) ) {
	    // launch
	    kDebug() << "Category "<< *sit << " started";
	    Q_ASSERT(m_categories[*sit]);
	    m_categories[*sit]->start();
	    m_launchedQueries[*sit] = m_categories[*sit];
	    sit = m_launchQueue.erase(sit);
	    i++;
	}
	else {
	    kDebug() << "Category "<< *sit << " already launched";
	    ++sit;
	}

    }
}

void Nepomuk::WebExtractorScheduler::start()
{
    if (!m_success)
	return;

    // If there is no category, then quit
    if (m_askQueries.isEmpty()) {
	kDebug() << "There is no category enabled and without mistakes.";
	return;
    }

    //qDebug() << m_askQueries;

    // Fill launch queue with category schedulers
    QHash<QString,QString>::iterator qit = m_askQueries.begin();
    while ( qit != m_askQueries.end() )
    {
	const QString & name = qit.key();
	if (m_categories.contains(qit.key()))
		continue;

	
	// Create scheduler for category
	Nepomuk::WebExtractorCategoryScheduler * sh = 
	    new Nepomuk::WebExtractorCategoryScheduler(
		m_selectQueries[name],
		this, 
		m_conf->extractParameters(name)
		);
	sh->setMaxResSimult(m_conf->maxResSimult(name));
	connect( sh, SIGNAL(finished()), this->m_sm, SLOT(map()), Qt::QueuedConnection );
	m_sm->setMapping(sh,name);
	m_categories[qit.key()] = sh;

	QTimer * ttm = new QTimer(this);
	m_timers[name] = ttm;

	connect( ttm, SIGNAL(timeout()), this->m_timerSM, SLOT(map()));
	m_timerSM->setMapping(ttm,name);


	// If category is ready, then add it to launch queue
	Soprano::QueryResultIterator it = 
	    Nepomuk::ResourceManager::instance()->mainModel()->executeQuery( 
	    qit.value(), Soprano::Query::QueryLanguageSparql 
	    );
	if (it.boolValue() ) {
	    // add scheduler to queue
	    kDebug() << "Category "<< name << " ready for extracting";
	    m_launchQueue.insert(name);
	}

	qit++;
    }
    // Start timer
    m_launchTimer->start(1000*60*2);
    // Start first portion of categories
    launchNext();
}

void Nepomuk::WebExtractorScheduler::stop()
{
    //clearLaunched();
    m_launchTimer->stop();
    QHash<QString,WebExtractorCategoryScheduler *>::iterator qit; 
    for(qit = m_launchedQueries.begin(); qit != m_launchedQueries.end(); ++qit)
    {
	kDebug() << "Stopping "<<qit.key();
	qit.value()->stop();
    }
    for(qit = m_launchedQueries.begin(); qit != m_launchedQueries.end(); ++qit)
    {
	kDebug() << "Waiting for "<<qit.key();
	qit.value()->wait();
    }
#if 0
    kDebug() << "All cats finished";
    for(qit = m_categories.begin(); qit != m_categories.end(); ++qit)
    {
	kDebug() << "Deleting for "<<qit.key();
	assert(!qit.value()->isRunning());
	delete qit.value();
    }

    // Clear add data
    foreach(WebExtractorCategoryScheduler * sh, m_categories)
    {
	assert(!sh->isRunning() );
	delete sh;
    }

    foreach(QTimer * t, m_timers)
    {
	delete t;
    }


    m_launchedQueries.clear();
    m_categories.clear();
    m_launchQueue.clear();
    m_timers.clear();
    m_askQueries.clear();
    m_selectQueries.clear();
#endif
}

void Nepomuk::WebExtractorScheduler::suspend()
{
    QHash<QString,WebExtractorCategoryScheduler *>::iterator qit; 
    for(qit = m_launchedQueries.begin(); qit != m_launchedQueries.end(); ++qit)
    {
	kDebug() << qit.key() << "suspended";
	qit.value()->suspend();
    }
}

void Nepomuk::WebExtractorScheduler::resume()
{
    QHash<QString,WebExtractorCategoryScheduler *>::iterator qit; 
    for(qit = m_launchedQueries.begin(); qit != m_launchedQueries.end(); ++qit)
    {
	kDebug() << qit.key() << "resumed";
	qit.value()->resume();
    }
}

void Nepomuk::WebExtractorScheduler::reconfigure()
{
    // Stop all running categories
    stop();
    // Read config file again
    m_success = false;
    readConfig();

    if (!m_success) {
	kDebug() << "Incorrect config file after reconfiguring";
	return;
    }
    start();
}

void Nepomuk::WebExtractorScheduler::categoryFinished(const QString & name)
{
    kDebug() << "Category "<<name<<" finished";
    // Start timer
    QHash<QString, QTimer*>::iterator tit = m_timers.find(name);
    if (tit != m_timers.end()) {
	// launch timer
	tit.value()->start(m_conf->interval(name));
    }
    //QHash<QString,QString>::iterator it = m_launchedQueries.find(name);
    // If stop was called then all data are already cleared. 
    // Because signal is queued it can come after main scheduler is stopped
    // and even after it is started again.
    
    /*
    if ( it != m_launchedQueries.end() ) {
	delete it.value();
	erase(it);
    }
    */
}

