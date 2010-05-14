/*
   Copyright (c) 2008-2010 Sebastian Trueg <trueg@kde.org>
   Copyright (C) 2010 by Serebriyskiy Artem <v.for.vandal at kde.org>

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
#include <Soprano/Model>
#include <Nepomuk/ResourceManager>
#include "webexscheduler.h"
#include "webexschedulerimpl.h"

namespace NW = Nepomuk::WebExtractor;

Nepomuk::WebExtractorScheduler::WebExtractorScheduler(const QString & category_query, QObject * parent):
    QThread(parent),
    m_suspended(false),
    m_stopped(false),
    m_extracting(false),
    m_speed(FullSpeed),
    m_reducedSpeedDelay(500),
    m_snailPaceDelay(3000),
    m_impl(0),
    m_query(category_query)
{;
}


Nepomuk::WebExtractorScheduler::~WebExtractorScheduler()
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

void Nepomuk::WebExtractorScheduler::suspend()
{
    if ( isRunning() )  {
        QMutexLocker locker( &m_resumeStopMutex );
	if (!m_suspended) { 
	    m_suspended = true;
	    emit extractingSuspended( true );
	}
    }
}


void Nepomuk::WebExtractorScheduler::resume()
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


void Nepomuk::WebExtractorScheduler::setSuspended( bool suspended )
{
    if ( suspended )
        suspend();
    else
        resume();
}


void Nepomuk::WebExtractorScheduler::stop()
{
    if ( isRunning() ) {
        QMutexLocker locker( &m_resumeStopMutex );
        m_stopped = true;
        m_suspended = false;
        m_resumeStopWc.wakeAll();
    }
}


void Nepomuk::WebExtractorScheduler::restart()
{
    stop();
    wait();
    start();
}

void Nepomuk::WebExtractorScheduler::setExtractingSpeed( ExtractingSpeed speed )
{
    kDebug() << speed;
    m_speed = speed;
}

void Nepomuk::WebExtractorScheduler::setExtractingStarted( bool started )
{
    if ( started != m_extracting ) {
        m_extracting = started;
        if ( m_extracting )
            emit extractingStarted();
        else
            emit extractingStopped();
    }
}

void Nepomuk::WebExtractorScheduler::setReducedExtractingSpeed( bool reduced )
{
    if ( reduced )
        setExtractingSpeed( ReducedSpeed );
    else
        setExtractingSpeed( FullSpeed );
}


bool Nepomuk::WebExtractorScheduler::isSuspended() const
{
    return isRunning() && m_suspended;
}


bool Nepomuk::WebExtractorScheduler::isExtracting() const
{
    return m_extracting;
}


void Nepomuk::WebExtractorScheduler::run()
{
    m_impl = new WebExtractorSchedulerImpl(m_query, this);
    // set lowest priority for this thread
    setPriority( QThread::IdlePriority );

    setExtractingStarted( true );

    // Start query
    /*
    it = Nepomuk::ResourceManager::instance()->mainModel()->executeQuery( 
	    m_query,Soprano::Query::QueryLanguageSparql 
	    );
	    */

    connect(this->m_impl, SIGNAL(launchPls()), this->m_impl, SLOT(launchNext()), Qt::QueuedConnection );

    for ( int i = 0; i < m_impl->m_maxResSimult; i++)
	m_impl->launchNext();


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
    kDebug() << "Starting scheduler main loop";
    exec();
    kDebug() << "Scheduler main loop finished";

    setExtractingStarted( false );

    // reset state
    m_suspended = false;
    m_stopped = false;
    m_impl->m_finishing = false;
}

bool Nepomuk::WebExtractorScheduler::waitForContinue( bool disableDelay )
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

    return !m_stopped;
}

