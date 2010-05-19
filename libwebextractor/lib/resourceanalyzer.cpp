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
#include "resourceanalyzer.h"
#include <KDebug>
#include <QtCore/QTimer>
#include <assert.h>
#include "resourceanalyzerimpl.h"

/*
namespace Nepomuk {
    namespace WebExtractor {
	class ResourceAnalyzerImpl : public QObject
	{
	    Q_OBJECT;
	    public:
		ResourceAnalyzerImpl(QObject * parent = 0);
		void analyze( Nepomuk::Resource & res);
	    Q_SIGNALS:
		void analyzingFinished();
	    private:
		int tmp_count;
		int m_respWaits;
	    private Q_SLOTS:
		void pluginFinished();
		bool launchNext();
	};
    }
}

*/

Nepomuk::WebExtractor::ResourceAnalyzer::ResourceAnalyzer(
	const DataPPKeeper & dataPPKeeper,
       	DecisionFactory * fac,
       	WE::MergePolitics mergePolitics,
       	WE::LaunchPolitics launchPolitics,
	double acrit,
	double ucrit,
	unsigned int step,
       	QObject * parent
	):
    QObject(parent)
{
    m_analyzer = new Nepomuk::WebExtractor/*::ResourceAnalyzer*/::ResourceAnalyzerImpl(dataPPKeeper, fac, this);
    m_analyzer->m_mergePolitics = mergePolitics;
    m_analyzer->m_launchPolitics = launchPolitics;
    m_analyzer->m_step = step;
    m_analyzer->m_acrit = acrit;
    m_analyzer->m_ucrit = ucrit;
    connect(m_analyzer,SIGNAL(analyzingFinished()),this, SIGNAL(analyzingFinished()));
}

void Nepomuk::WebExtractor::ResourceAnalyzer::analyze( Nepomuk::Resource & res)
{
    m_analyzer->analyze(res);
}

Nepomuk::WebExtractor/*::ResourceAnalyzer*/::ResourceAnalyzerImpl::ResourceAnalyzerImpl(
	const DataPPKeeper & dataPPKeeper,
       	DecisionFactory * fact,
       	QObject * parent
	):
    QObject(parent),
    tmp_count(15),
    m_respWaits(0),
    m_dataPPKeeper(dataPPKeeper),
    m_step(10),
    m_fact(fact),
    m_mergePolitics(WE::Average),
    m_mergeCoff(1),
    m_launchPolitics(WE::StepWise),
    m_decisions(fact->newDecisionList())
{
    this->it = m_dataPPKeeper.begin();
    //DataPPKeeper::const_iterator it = m_dataPPKeeper.begin();
    
}

void Nepomuk::WebExtractor/*::ResourceAnalyzer*/::ResourceAnalyzerImpl::analyze(Nepomuk::Resource & res)
{
    // Make some work here
    m_res = res;
    
    // start processing
    kDebug() << "Extracting data from resource";
    launchOrFinish();
}

bool Nepomuk::WebExtractor/*::ResourceAnalyzer*/::ResourceAnalyzerImpl::launchNext()
{
    assert(m_respWaits == 0);
    //if (!tmp_count)
    //	return false;

    kDebug() << "Launching next portion of plugins";
    kDebug() << "Total plugins: "<<m_dataPPKeeper.size();

    int substop = 0;
    if (m_launchPolitics == WE::All )
	substop = m_dataPPKeeper.size();
    else if (m_launchPolitics == WE::StepWise) 
	substop = m_step;

    int i = 0;

    for( ; ( (it != m_dataPPKeeper.end() ) and (i < substop) ); it++,i++ )
    {
	    
	// launch 
	DataPPReply * repl = it->first->requestDecisions(m_fact, m_res);

	if (!repl) {
	    kDebug() << "DataPP return 0 as reply. How should I handle it? Ignoring.";
	    continue;
	}

	// Save this reply
	connect( repl, SIGNAL(finished()), this, SLOT( pluginFinished() ) );
	connect( repl, SIGNAL(error()), this, SLOT( pluginFinished() ) );

	m_replyAndRanks[repl] = it->second;
	m_respWaits++;;

    }

    if ( i == 0 )
	return false;


    //tmp_count -= c;
    return true;
}

void Nepomuk::WebExtractor::ResourceAnalyzerImpl::launchOrFinish()
{
	if ( !launchNext() ) {
	    // No more plugins to launch and all plugins launched before
	    // returned their data
	    kDebug() << "Extracting for resource finished";
	    kDebug() << "Total decisions count: "<<m_decisions.size();
	    emit analyzingFinished();
	}
}


void Nepomuk::WebExtractor/*::ResourceAnalyzer*/::ResourceAnalyzerImpl::pluginFinished()
{
    m_respWaits--;
    kDebug() << "Recived answer from plugin.";

    // Process data plugin has returned
    DataPPReply * repl = qobject_cast<DataPPReply*>(QObject::sender() );
    if (repl) {
	// Delete it from map and call deleteLater
	if (!m_replyAndRanks.contains(repl)) {
		kDebug() << "Recived answer from unregistred DataPPReply";
		}
	double repl_rank = m_replyAndRanks[repl];

	repl->deleteLater();
	
	if (repl->isValid()) {
	    // Process Decision list
	    m_decisions.mergeWith(repl->decisions(), repl_rank,m_mergePolitics, m_mergeCoff );
	}
    }
    else {
	kDebug() << "Recive answer not from DataPPReply object";
    }



    
    if (m_respWaits == 0) {
	// All launched plugins return data
	// Process it
	// If there is any Decision that is applied automaticaly
	// then trancate list by acrit and change parameters of decisionfactory
	if ( m_decisions.hasAutoApplicable() )
	    m_fact->setThreshold(m_acrit);


	// Launching other plugins if necessary
	launchOrFinish();
    }
    /*
    else {
       kDebug() << "Only " << m_respWaits << " answers remaining";
    }
    */
}
