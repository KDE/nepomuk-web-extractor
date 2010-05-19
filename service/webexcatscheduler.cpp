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
    m_suspended(false),
    m_stopped(false),
    m_extracting(false),
    m_speed(FullSpeed),
    m_reducedSpeedDelay(500),
    m_snailPaceDelay(3000),
    m_impl(0),
    m_query(category_query),
    m_maxResSimult(maxResSimult)
{
    m_extractParams = params;
    //kDebug() << *m_extractParams;
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
    delete m_impl;
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
    
    if (m_impl)
	delete m_impl;

    if (m_extractParams.isNull() ) {
	kDebug() << "Extracting parameters is null. Ignoring launch";
    }

    //kDebug() << *m_extractParams;
    
    m_impl = new (std::nothrow) WebExtractorCategorySchedulerImpl(m_query,this,m_extractParams, m_maxResSimult);
    if (!m_impl)
	return;
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
    if ( m_impl->start()) {
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
    m_impl->m_finishing = false;
}

bool Nepomuk::WebExtractorCategoryScheduler::waitForContinue( bool disableDelay )
{
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

