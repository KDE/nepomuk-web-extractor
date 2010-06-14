#include <QThread>
#include <QTimer>
#include <QCoreApplication>
#include <QSignalMapper>
#include <Nepomuk/Resource>
#include <webextractor/simple_datapp.h>
#include <webextractor/simple_reply_factory.h>
#include "simple_debug_reply.h"
#include <QObject>
#include <stdint.h>


#define TESTERS_COUNT 5
#define REPLS_COUNT 5

using namespace Nepomuk;
using namespace Nepomuk::WebExtractor;


class SimpleTester : public QThread
{
    Q_OBJECT;
    public:
	SimpleTester( SimpleDataPP * dpp)
	{
	    this->dpp = dpp;
	    mapper = new QSignalMapper();
	    f = 0;
	    kDebug() << "Created new SimpleTester" /*<< int(QThread::currentThreadId())*/;
	}
    private:
	SimpleDataPP * dpp;
	DataPPReply * repl[REPLS_COUNT];
	QSignalMapper * mapper;
	int f;
    protected:
	virtual void run()
	{
	    kDebug() << "SimpleTester" << (uintptr_t)(QThread::currentThread()) << "started";
	    for ( int i = 0; i < REPLS_COUNT; i++)
	    {
		repl[i] = dpp->requestDecisions(DecisionFactory::debugFactory(),Nepomuk::Resource());
		connect(repl[i], SIGNAL(finished()), this->mapper, SLOT(map()));
		this->mapper->setMapping(repl[i],i);
	    }
	    connect(this->mapper, SIGNAL(mapped(int)), this, SLOT(finished(int)) );

	    exec();

	}

	protected Q_SLOTS:
	    void finished(int num) 
	    {
		kDebug() << num << " finished " << (uintptr_t )(QThread::currentThread());
		f++;
		if ( f == 5)
		    quit();
	    }
};

class Tester: public QObject
{
    Q_OBJECT;
    public:
	Tester() 
	{;}
	void init()
	{
	    SimpleDataPP * dpp = new SimpleDataPP(
		    "0.1",
		    "http://www.example.org",
		new Nepomuk::WebExtractor::SimpleReplyFactoryTemplate<SimpleDebugReply>()
		);
	    // Generate 4 threads
	    for ( int i = 0; i < TESTERS_COUNT; i++)
	    {
		testers[i] = new SimpleTester(dpp);
		//testers[i]->start();
	    }
	}
    public Q_SLOTS:
	void start() 
	{
	    init();
	    Nepomuk::Resource res;
	    for ( int i = 0; i < TESTERS_COUNT; i++)
	    {
		testers[i]->start();
	    }
	}
    private:
	SimpleTester * testers[TESTERS_COUNT];
};

int main(int argc, char ** argv)
{
    QCoreApplication app(argc,argv);
    Tester * t = new Tester();
    QTimer::singleShot(0,t, SLOT(start()));
    
    app.exec();    
}

#include "moc_test_simple.cxx"
