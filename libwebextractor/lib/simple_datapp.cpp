#include  "simple_datapp.h"
#include  "stdint.h"
#include "simple_reply.h"
#include "simple_reply_factory.h"

namespace NW=Nepomuk::WebExtractor;

NW::SimpleDataPP::SimpleDataPP(const QString & dataPPVersion, const QString & serverName ):
    DataPP(dataPPVersion),
    m_timeout(true),
    m_factory(0),
    m_interval(1500)
{
    __init();
}

NW::SimpleDataPP::SimpleDataPP(const QString & dataPPVersion, SimpleReplyFactory * factory):
    DataPP(dataPPVersion),
    m_timeout(true),
    m_factory(factory),
    m_interval(1500)
{
    __init();
    kDebug() << "New SimpleDataPP created. ID: "<<uintptr_t(this);
}

void NW::SimpleDataPP::__init()
{
    m_net = new QNetworkAccessManager();
    m_reqTimer = new QTimer();
    m_reqTimer->setInterval(m_interval);
    m_reqTimer->setSingleShot(true);
    connect(m_reqTimer, SIGNAL(timeout()),this, SLOT(timeout()));
    m_skipped = 0;
    m_maxSkipped = 5;
}

NW::SimpleDataPP::~SimpleDataPP()
{
    delete m_net;
    delete m_reqTimer;
    delete m_factory;
}

NW::SimpleDataPPReply * NW::SimpleDataPP::newReply(const DecisionFactory * factory, const Nepomuk::Resource & res)
{
    if (!m_factory)
	return 0;

    return m_factory->newReply(this,factory,res);
}


/*
void NW::SimpleDataPP::launchNext()
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

void NW::SimpleDataPP::startTimer()
{
    kDebug() << "Start timer";
    //QMutexLocker locker(&m_timeoutMutex);
    if (!m_reqTimer->isActive() ) {
	m_reqTimer->start();
    }
}

NW::DataPPReply * NW::SimpleDataPP::requestDecisions(const DecisionFactory * factory, const Nepomuk::Resource & res)
{
    // Create SimpleReply with this information
    SimpleDataPPReply * repl = this->newReply(factory,res);

    kDebug() << "ID: "<< uintptr_t(this);
    QMutexLocker locker_time(m_timeoutMutex);
    if ( m_last_request_time.msecsTo(QTime::currentTime) > m_interval ) {
	// Make request immideately
	if ( repl.step() ) { 
	    // Add it to queue
	    QMutexLocker locker_queue(&m_queueMutex);
	    m_queue.enqueue(repl);
	    locker_queue.unlock();
	}
	// Set new request time again
	m_last_request_time = QTime::currentTime();

	

    // Because this method is called not from our parent thread
    // we can not start timers directly
    // Instead we will call QMetaObject::invokeMethod with
    // Qt::QueuedConnection type
    

    return repl;

}

void NW::SimpleDataPP::timeout()
{
    kDebug() << "Timeout";
    QMutexLocker locker(&m_timeoutMutex);
    QMutexLocker locker_queue(&m_queueMutex);
    if (m_queue.isEmpty()) {
	m_skipped++;
	
	// If maxSkipped limit reached then stop timer
	if (m_skipped == m_maxSkipped)
	    return;

    }
    else {
	SimpleDataPPReply * repl = m_queue.dequeue();
	// Call step for repl
	repl->step();
    }

    m_reqTimer->start();
}

void NW::SimpleDataPP::enqueue( SimpleDataPPReply * repl)
{
    QMutexLocker locker_queue(&m_queueMutex);
    m_queue.enqueue(repl);

    // If timer have stopped, restart it
    if (!m_reqTimer->isActive()) {
	m_reqTimer->start();
    }
}
