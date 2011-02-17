/*
   Copyright (C) 2010 by Serebriyskiy Artem <v.for.vandal at gmail.com>

   This library is free software; you can redistribute it and/or modify
   it under the terms of the GNU Library General Public License as published by
   the Free Software Foundation; either version 3, or (at your option)
   any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#include  "simplenetworkexecutive.h"
#include  "stdint.h"
#include "simplenetworkreply.h"
#include "simplenetworkreplyfactory.h"
#include <QThread>
#include <stdint.h>

namespace NW = Nepomuk::WebExtractor;

NW::SimpleNetworkExecutive::SimpleNetworkExecutive(int dataPPVersion, const QString & serverName, const QString & endpointUrl):
    Executive(dataPPVersion),
    m_timeout(true),
    m_factory(0),
    m_interval(1500),
    m_endpointUrl(endpointUrl)
{
    __init();
}

NW::SimpleNetworkExecutive::SimpleNetworkExecutive(int dataPPVersion, const QString & endpointUrl, SimpleReplyFactory * factory):
    Executive(dataPPVersion),
    m_timeout(true),
    m_factory(factory),
    m_interval(1500),
    m_endpointUrl(endpointUrl)
{
    __init();
    kDebug() << "New SimpleNetworkExecutive created. ID: " << uintptr_t(this);
}

NW::SimpleNetworkExecutive::~SimpleNetworkExecutive()
{
    //TODO Implement QWaitCondition that will free when queue becomes empty
    delete m_net;
    foreach(SimpleNetworkExecutiveRequest * req, m_queue) {
        delete req;
    }
    delete m_reqTimer;
    delete m_factory;
    kDebug() << "SimpleNetworkExecutive deleted. ID: " << uintptr_t(this);
}

void NW::SimpleNetworkExecutive::__init()
{
    m_net = new QNetworkAccessManager();
    m_reqTimer = new QTimer();
    m_reqTimer->setInterval(m_interval);
    m_reqTimer->setSingleShot(true);
    connect(m_reqTimer, SIGNAL(timeout()), this, SLOT(timeout()));
    m_skipped = 0;
    m_maxSkipped = 5;
}


NW::SimpleNetworkExecutiveReply * NW::SimpleNetworkExecutive::newReply(const DecisionFactory * factory, const Nepomuk::Resource & res)
{
    if(!m_factory)
        return 0;

    return m_factory->newReply(this, factory, res);
}


/*
void NW::SimpleNetworkExecutive::launchNext()
{
    QMutexLocker locker_timer(&m_timeoutMutex);
    QMutexLocker locker_queue(&m_queueMutex);
    if (m_queue.isEmpty() )
    return;
    // Take next element and start it
    SimpleReply * repl = m_queue.dequeue();
    locker_queue.unlock();

    repl.step();
}
*/

void NW::SimpleNetworkExecutive::startTimer()
{
    kDebug() << "Start timer";
    //QMutexLocker locker(&m_timeoutMutex);
    if(!m_reqTimer->isActive()) {
        m_reqTimer->start();
    }
}

// This function is executed in thread requesting reply. Most other functions executed in thread where Executive lives
//NW::ExecutiveReply * NW::SimpleNetworkExecutive::requestDecisions(const DecisionFactory * factory, const Nepomuk::Resource & res, QObject* target, const char * finishedSlot, const char * errorSlot)
NW::ExecutiveReply * NW::SimpleNetworkExecutive::requestDecisions(const DecisionFactory * factory, const Nepomuk::Resource & res)
{
    // Create SimpleReply with this information
    SimpleNetworkExecutiveReply * repl = this->newReply(factory, res);
#if 0
    connect(repl, SIGNAL(finished()), target, finishedSlot, Qt::QueuedConnection);
    connect(repl, SIGNAL(error()), target, errorSlot, Qt::QueuedConnection);
#endif
    kDebug() << "ID: " << uintptr_t(this);
    repl->start();



    return repl;

}

void NW::SimpleNetworkExecutive::get(SimpleNetworkExecutiveRequest * request)
{
    // This function can be called from parent thread or by caller thread

    // QNetwork is not thread-safe. what a pity.
    if(thread() == QThread::currentThread()) {    // We can avoid queuing request
        kDebug() << "Native thread";
        QMutexLocker locker_time(&m_timeoutMutex);
        if(m_last_request_time.msecsTo(QTime::currentTime()) > m_interval) {
            // Make request immediately
            // getRequest only run actual request. It MUST never pass control to
            // event loop!
            getRequest(request);
            // Update time
            m_last_request_time = QTime::currentTime();

            // Return
            return;
        }
        // Unlock timer
        locker_time.unlock();
    } else {
        // Thread is not a parent thread but a caller thread. Then push object to parent
        // thread
        request->moveToThread(thread());
        kDebug() << "Foreign thread";
    }
    // This is not parent thread or network is not ready for next request
    // Add to queue.

    // In this case the control move from calling thread to
    // thread where SimleExecutive lives. ( In case this is 2 different threads.
    // Such situation occur when get is called indirectly from requestDecisions via
    // SimpleNetworkExecutiveReply->start() )

    enqueue(request);
    // And start timer. invokeMethod() is used because timer should be called
    // from it's owner thread and, as mentioned above, this function can be executed
    // by different thread


    if(thread() == QThread::currentThread()) {
        // We can avoid queued method invocation
        // if current thread is our parent thread
        kDebug() << "Native invocation";
        startTimer();
    } else {
        kDebug() << "Queued invocation";
        QMetaObject::invokeMethod(this, "startTimer", Qt::QueuedConnection);
    }
}

int NW::SimpleNetworkExecutive::runNext()
{
    QMutexLocker locker_queue(&m_queueMutex);
    if(m_queue.isEmpty())
        return 0;
    SimpleNetworkExecutiveRequest * req = m_queue.dequeue();
    locker_queue.unlock();

    getRequest(req);
    return 1;

}

void NW::SimpleNetworkExecutive::getRequest(SimpleNetworkExecutiveRequest * request)
{
    request->clear();
    QNetworkReply * repl = m_net->get(
                               QNetworkRequest(m_endpointUrl + request->requestUrlPart())
                           );
    request->setReply(repl);
}

void NW::SimpleNetworkExecutive::timeout()
{
    kDebug() << "Timeout";
    // Lock timer
    QMutexLocker locker(&m_timeoutMutex);
    if(!runNext()) {  // Queue was empty. Sounds like a bug.
        kDebug() << "runNext return 0-code. Queue was empty. Bug or Timer events caching. Ignoring";
        return;
    } else { // Check that queue has any elements.
        QMutexLocker(&(this->m_queueMutex));
        if(m_queue.size() > 0) {
            // Run timer again
            m_reqTimer->start();
        }
    }
#if 0
    QMutexLocker locker_queue(&m_queueMutex);
    if(m_queue.isEmpty()) {
        m_skipped++;

        // If maxSkipped limit reached then stop timer
        if(m_skipped == m_maxSkipped)
            return;

    } else {
        SimpleNetworkExecutiveReply * repl = m_queue.dequeue();
        // Call step for repl
        repl->step();
    }

    m_reqTimer->start();
#endif
}

//void NW::SimpleNetworkExecutive::enqueue( SimpleNetworkExecutiveReply * repl)
void NW::SimpleNetworkExecutive::enqueue(SimpleNetworkExecutiveRequest * request)
{
    QMutexLocker locker_queue(&m_queueMutex);
    m_queue.enqueue(request);
    locker_queue.unlock();

}
