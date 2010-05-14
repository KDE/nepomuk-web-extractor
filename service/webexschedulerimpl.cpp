#include "webexschedulerimpl.h"
#include <assert.h>
#include <KDebug>

namespace NW = Nepomuk::WebExtractor;

Nepomuk::WebExtractorSchedulerImpl::WebExtractorSchedulerImpl(const QString & category_query, WebExtractorScheduler * parent):
    QObject(parent),
    m_query(category_query),
    m_respWaits(0),
    m_maxResSimult(5),
    m_currentResProc(0),
    tmp_count(100)
{
    m_factory = new Nepomuk::WebExtractor::ResourceAnalyzerFactory(this);
    m_par = parent;
    connect(this, SIGNAL(launchPls()), this, SLOT(launch()), Qt::QueuedConnection );
}

void Nepomuk::WebExtractorSchedulerImpl::launchFirst()
{
    for ( int i = 0; i < m_maxResSimult; i++)
	launchNext();
}
bool Nepomuk::WebExtractorSchedulerImpl::launchNext()
{
    if (!tmp_count)
	return false;

    tmp_count--;
    emit launchPls();


    return true;

}

void Nepomuk::WebExtractorSchedulerImpl::launch()
{
    NW::ResourceAnalyzer * resanal = m_factory->newAnalyzer();
    Nepomuk::Resource res;
    connect(resanal, SIGNAL(analyzingFinished()), this, SLOT(resourceProcessed()));
    m_currentResProc++;
    resanal->analyze(res);
}

void Nepomuk::WebExtractorSchedulerImpl::resourceProcessed()
{
    m_currentResProc--;
    NW::ResourceAnalyzer * res = qobject_cast< NW::ResourceAnalyzer *>(sender() );
    if (!res) {
	kDebug() << "Recive signal not from ResourceAnalyzer";
    }
    m_factory->deleteAnalyzer(res);
    if (m_finishing) {
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
	    }
    }
}
