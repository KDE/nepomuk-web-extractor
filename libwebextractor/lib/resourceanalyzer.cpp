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
#include <stdint.h>

namespace NW = Nepomuk::WebExtractor;


class Nepomuk::WebExtractor::ResourceAnalyzer::Private
{
    public:
        Private(const DataPPKeeper & dataPPKeeper, DecisionFactory * fact);
        ~Private();
    public:
        //int tmp_count;
        int m_respWaits;
        const DataPPKeeper & m_dataPPKeeper;
        DataPPKeeper::const_iterator it;
        DecisionFactory * m_fact;
        //QMap< DataPPReply*, double > m_replyAndRanks;
        WE::LaunchPolitics m_launchPolitics;
        unsigned int m_step;
        DecisionList m_decisions;
        WE::MergePolitics m_mergePolitics;
        double m_mergeCoff;
        Nepomuk::Resource m_res;
        double m_acrit;
        double m_ucrit;
        QQueue<DataPPWrapper*> m_queue;
        bool m_applied;
        ResourceAnalyzer::AnalyzingError m_error;
        void enqueue(const QSet<const DataPP*> &);

};

Nepomuk::WebExtractor::ResourceAnalyzer::Private::Private(
    const DataPPKeeper & dataPPKeeper,
    DecisionFactory * fact
):
    m_respWaits(0),
    m_dataPPKeeper(dataPPKeeper),
    m_fact(fact),
    m_mergeCoff(1),
    m_applied(false)
{
    //DataPPKeeper::const_iterator it = m_dataPPKeeper.begin();

}

Nepomuk::WebExtractor::ResourceAnalyzer::Private::~Private()
{
    delete m_fact;
}

void Nepomuk::WebExtractor::ResourceAnalyzer::Private::enqueue(const QSet<const DataPP*> & targets)
{
    foreach(const DataPP * pp, targets) {
        DataPPWrapper * dpp = m_dataPPKeeper[pp];
        Q_CHECK_PTR(dpp);
        m_queue.enqueue(dpp);
    }
}


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
    d = new Nepomuk::WebExtractor::ResourceAnalyzer::Private(dataPPKeeper, fac);
    d->m_mergePolitics = mergePolitics;
    d->m_launchPolitics = launchPolitics;
    d->m_step = step;
    d->m_acrit = acrit;
    d->m_ucrit = ucrit;
    d->m_decisions = fac->newDecisionList();
    d->it = d->m_dataPPKeeper.begin();
}

Nepomuk::WebExtractor::ResourceAnalyzer::~ResourceAnalyzer()
{
    delete d;
}


void Nepomuk::WebExtractor/*::ResourceAnalyzer*/::ResourceAnalyzer::analyze(Nepomuk::Resource & res)
{
    // Make some work here
    d->m_res = res;

    // Add all datapp to queue
    foreach(DataPPWrapper * dppw, d->m_dataPPKeeper) {
        d->m_queue.enqueue(dppw);
    }

    // start processing
    kDebug() << "Extracting data from resource";

    if(!launchNext()) {
        // Can not analyze - no DataPP or any other problem
        // to avoid infinite recursion the analyzingFinished signal will
        // be called via QTimer::singleShot(0)
        QTimer::singleShot(0, this, SLOT(emitAnalyzingFinished()));
    }
}

bool Nepomuk::WebExtractor/*::ResourceAnalyzer*/::ResourceAnalyzer::launchNext()
{
    //kDebug() << "DISABLED";
    //return false;
    assert(d->m_respWaits == 0);
    //if (!tmp_count)
    //  return false;

    kDebug() << "Launching next portion of plugins";
    kDebug() << "Total plugins: " << d->m_dataPPKeeper.size();

    int substop = 0;
    if(d->m_launchPolitics == WE::All)
        substop = d->m_dataPPKeeper.size();
    else if(d->m_launchPolitics == WE::StepWise)
        substop = d->m_step;

    int i = 0;



    for(; ((d->m_queue.size()) and(i < substop)); i++) {
        const DataPPWrapper * dpp = d->m_queue.dequeue();

        // launch
        //DataPPReply * repl = dpp->requestDecisions(d->m_fact, d->m_res,this, SLOT( pluginFinished()), SLOT(pluginFinished()));
        DataPPReply * repl = dpp->requestDecisions(d->m_fact, d->m_res);
        // ATTENTION! repl object is executed in another thread!
        //repl->setParent(this);

        if(!repl) {
            kDebug() << "DataPP return 0 as reply. How should I handle it? Ignoring.";
            continue;
        }

        // Save this reply
        connect(repl, SIGNAL(finished()), this, SLOT(pluginFinished()));
        connect(repl, SIGNAL(error()), this, SLOT(pluginFinished()));

        //d->m_replyAndRanks[repl] = d->it->second;
        d->m_respWaits++;;

    }

    if(i == 0)
        return false;


    //tmp_count -= c;
    return true;
}

void Nepomuk::WebExtractor::ResourceAnalyzer::launchOrFinish()
{
    if(!launchNext()) {
        // No more plugins to launch and all plugins launched before
        // returned their data


        kDebug() << "Extracting for resource finished";
        kDebug() << "Total decisions count: " << d->m_decisions.size();
        emitAnalyzingFinished();
    }
}


void Nepomuk::WebExtractor/*::ResourceAnalyzer*/::ResourceAnalyzer::pluginFinished()
{
    d->m_respWaits--;
    kDebug() << "Recived answer from plugin.";

    // Process data plugin has returned
    DataPPReply * repl = qobject_cast<DataPPReply*>(QObject::sender());
    if(repl) {
        const DataPP * parent = repl->parentDataPP();
        // Delete it from map and call deleteLater
        //if (!d->m_replyAndRanks.contains(repl)) {
        if(d->m_dataPPKeeper.contains(parent)) {
            //double repl_rank = d->m_replyAndRanks[repl];
            //double repl_rank = d->m_dataPPKeeper[parent]->rank();


            if(repl->isValid()) {
                // Process Decision list
                d->m_decisions.mergeWith(repl->decisions(), d->m_mergePolitics, d->m_mergeCoff);
            }

            repl->deleteLater();
        } else {
            kDebug() << "Recived answer from unregistred DataPP";
        }
    } else {
        kDebug() << "Recive answer not from DataPPReply object";
    }




    if(d->m_respWaits == 0) {
        // All launched plugins return data
        // Process it
        QSet< const DataPP*> set = d->m_decisions.filterObsolete();
        if(set.size()) {
            d->enqueue(set);
        }

        // Launching other plugins if necessary
        launchOrFinish();
    }
    /*
    else {
       kDebug() << "Only " << m_respWaits << " answers remaining";
    }
    */
}

void NW::ResourceAnalyzer::apply()
{
    if(d->m_applied)
        return;

    // Process data
    if(d->m_decisions.hasAutoApplicable()) {
        d->m_decisions.best().apply();
    } else {
        d->m_decisions.addToUserDiscretion();
    }

    d->m_applied = true;
}

void NW::ResourceAnalyzer::clear()
{
    d->m_decisions.clear();
}

NW::ResourceAnalyzer::AnalyzingError NW::ResourceAnalyzer::error() const
{
    return d->m_error;
}

void NW::ResourceAnalyzer::emitAnalyzingFinished()
{
    emit analyzingFinished();
}

void NW::ResourceAnalyzer::finishWithError(AnalyzingError code)
{
    d->m_error = code;
    emit error(code);
    emit analyzingFinished();
}
