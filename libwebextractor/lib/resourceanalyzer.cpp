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
#include <QSharedData>

namespace NW = Nepomuk::WebExtractor;


class Nepomuk::WebExtractor::ResourceAnalyzer::Private /*: public QSharedData*/
{
    public:
        Private(const DataPPKeeper & dataPPKeeper, DecisionFactory * fact);
        ~Private();
    public:
        //int tmp_count;
        // Number of responds that must be recived
        int m_respWaits;
        // THis is hash <DataPP*,DataPPWrapper*> where all DataPPWrappers are storeg
        // keys are corresponding DataPP*
        const DataPPKeeper & m_dataPPKeeper;
        //DataPPKeeper::const_iterator it;
        // DecisionFactory that will be passed to DataPP
        DecisionFactory * m_fact;
        //QMap< DataPPReply*, double > m_replyAndRanks;
        WE::LaunchPolitics m_launchPolitics;
        unsigned int m_step;
        // All collected Decisions are stored there
        DecisionList m_decisions;
        WE::MergePolitics m_mergePolitics;
        double m_mergeCoff;
        // This is stored resource we currently analyze. Or are going to analyze.
        Nepomuk::Resource m_res;
        double m_acrit;
        double m_ucrit;
        // This is queue where all DataPP that must be launched are stored
        QQueue<DataPPWrapper*> m_queue;
        // THis variable prevent executing apply() method more then one time
        bool m_applied;
        // This variable keep running/not-running state of ResourceAnalyzer
        bool m_running;
        // The last error that occure while analyzing
        ResourceAnalyzer::AnalyzingError m_error;
        // Convinience method to add set of DataPP* to queue
        void enqueue(const QSet<const DataPP*> &);
        // Store all replies there
        QSet<DataPPReply*> m_replies;

        // Convinience method to add set of DataPP* to queue
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
    m_applied(false),
    m_running(false),
    m_error(ResourceAnalyzer::NoError)
{
    //DataPPKeeper::const_iterator it = m_dataPPKeeper.begin();

}

Nepomuk::WebExtractor::ResourceAnalyzer::Private::~Private()
{
    kDebug() << "Deleted: " << uintptr_t(this);
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
    //d->it = d->m_dataPPKeeper.begin();
}

Nepomuk::WebExtractor::ResourceAnalyzer::~ResourceAnalyzer()
{
    kDebug() << "Deleted: " << uintptr_t(this);
    // delete d;
}


void Nepomuk::WebExtractor/*::ResourceAnalyzer*/::ResourceAnalyzer::analyze()
{

    analyze(d->m_res);
}

void Nepomuk::WebExtractor/*::ResourceAnalyzer*/::ResourceAnalyzer::analyze(Nepomuk::Resource & res)
{
    kDebug() << "This: " << uintptr_t(this) << "D-pointer: " << uintptr_t(this->d);
    if(isRunning()) {
        abort();
        clear();
    }

    // Reset error variable
    d->m_error = NoError;

    // Check that resource is valid
    if(!res.isValid()) {
        // Set error now to provide caller ability to check for errors immidiately
        // and delete ResourceAnalyzer, thus avoiding error signal(I hope Qt
        // doesn't pass signals from destroyed objects.)
        d->m_error = InvalidResource;
        QTimer::singleShot(0, this, SLOT(finishWithError()));
        return;
    }


    // Make some work here
    // Change current resource
    d->m_res = res;
    // Clear all previous data
    clear();

    // We check for resource existing AFTER switching from previously analyzed resource.
    // This is done for consistency with setResource() method that allow setting as
    // resource unexisting resource
    if(!res.exists()) {
        d->m_error = UnexistingResource;
        QTimer::singleShot(0, this, SLOT(finishWithError()));
        return;
    }

    // Add all datapp to queue
    foreach(DataPPWrapper * dppw, d->m_dataPPKeeper) {
        d->m_queue.enqueue(dppw);
    }

    // start processing
    kDebug() << "Extracting data from resource";

    d->m_running = true;
    if(!launchNext()) {
        // Can not analyze - no DataPP or any other problem
        // to avoid infinite recursion the analyzingFinished signal will
        // be called via QTimer::singleShot(0)
        if(d->m_error == NoError) {
            d->m_error = UnknownError;
        }
        d->m_running = false;
        kDebug() << "Failed to analyze resource. Check parameters.";
        QTimer::singleShot(0, this, SLOT(finishWithError()));
    }
}

bool NW::ResourceAnalyzer::isRunning() const
{
    return d->m_running;
}

void NW::ResourceAnalyzer::abort()
{
    if(d->m_running) {
        // This is the only method where m_replies is necessary :(
        // Abort each reply, delete it and clear m_replies
        foreach(DataPPReply * repl, d->m_replies) {
            disconnect(repl, SIGNAL(finished()), this, SLOT(pluginFinished()));
            // FIXME Change error() signal to pluginError slot
            disconnect(repl, SIGNAL(error()), this, SLOT(pluginFinished()));
            repl->abort();
            delete repl;
        }
        d->m_replies.clear();

        d->m_running = false;
    }
    return;
}

void NW::ResourceAnalyzer::setResource(const Nepomuk::Resource & res)
{
    if(d->m_running) {
        abort();
    }
    if(!res.isValid())
        return;

    // Clear previously collected data
    clear();
    // Set new resource and return
    d->m_res = res;

}


bool Nepomuk::WebExtractor/*::ResourceAnalyzer*/::ResourceAnalyzer::launchNext()
{
    //kDebug() << "DISABLED";
    //return false;
    assert(d->m_respWaits == 0);
    //if (!tmp_count)
    //  return false;

    kDebug() << "Launching next portion of plugins";
    kDebug() << "Total DataPP: " << d->m_dataPPKeeper.size();

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

        // Connect signals of this reply
        // FIXME Change error() signal to pluginError slot
        connect(repl, SIGNAL(finished()), this, SLOT(pluginFinished()));
        connect(repl, SIGNAL(error()), this, SLOT(pluginFinished()));

        Q_ASSERT(!d->m_replies.contains(repl));
        d->m_replies.insert(repl);

        //d->m_replyAndRanks[repl] = d->it->second;
        // Increase the number of active replies
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


// TODO I am not sure that even after deleting the reply there will not be any
// queued signals left. May be it is necessary to add filter that will ignore
// 'obsolete' replies
void Nepomuk::WebExtractor/*::ResourceAnalyzer*/::ResourceAnalyzer::pluginFinished()
{
    kDebug() << "This: " << uintptr_t(this) << "D-pointer: " << uintptr_t(this->d);
    d->m_respWaits--;
    kDebug() << "Recived answer from plugin.";

    // Process data plugin has returned
    DataPPReply * repl = qobject_cast<DataPPReply*>(QObject::sender());
    if(repl) {
        //Q_ASSERT(d->m_replies.contains(repl));
        // Remove reply from set
        d->m_replies.remove(repl);
        const DataPP * parent = repl->parentDataPP();
        // Error check - check that this reply is not from unknown DataPP
        if(d->m_dataPPKeeper.contains(parent)) {
            // Delete it from map and call deleteLater
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
        // Filter obsolete Decisions and add the DataPP that generate
        // this obsolete Decisions back to queue
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
    // This function can be called only once.
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
    if(d->m_running)
        return;

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

void NW::ResourceAnalyzer::finishWithError()
{
    emit error(d->m_error);
    emit analyzingFinished();
}

void NW::ResourceAnalyzer::finishWithError(AnalyzingError code)
{
    d->m_error = code;
    finishWithError();
}
