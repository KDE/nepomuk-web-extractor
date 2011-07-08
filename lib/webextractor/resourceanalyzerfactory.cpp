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


#include "resourceanalyzerfactory.h"
#include "resourceanalyzer.h"
#include <KDebug>
#include <Soprano/Global>
#include <Nepomuk/Tag>
#include <Soprano/StorageModel>
#include "executive.h"


namespace NW = Nepomuk::WebExtractor;
namespace ND = Nepomuk::Decision;

class NW::ResourceAnalyzerFactory::Private
{
    public:
        Private();
        ExecutiveKeeper  dataPPKeeper;

#warning trueg: Instead of copying every valus why not simply store an ExtractParameters instance?
        LaunchPolitics launchPolitics;
        ND::MergePolitics mergePolitics;
        unsigned int step;
        double acrit;
        double ucrit;
        bool autoDeleteModelData;
        bool autoManageOntologies;

        /*! \brief The backend that support in-memory models and will be used for storing decisions
         * Currently it is redland, but in future releases I will add some
         * functions to allow user select different backends
         */
        //const Soprano::Backend * usedBackend();
        // This is function pointer that is used for introspection of analyzer between
        // iterations ( when politics is Iterative)
        //void (*debug_interrupter)();

        // The folowing 2 variables are used to store Decisions while processing
        // them. This is necessary because we need some sparql support and can't
        // use Soprano::Graph.
        // When starting analyzing new resource the model must be totall reset.
        // ResourceManager is manager for model. It is not very necessary now, but
        // will be defenetely used in the future.

	ResourceServiceDataManager * rsdManager;
};


NW::ResourceAnalyzerFactory::Private::Private():
    launchPolitics(StepWise),
    mergePolitics(ND::Highest),
    step(10),
    rsdManager(0)
{
    ;
}


Nepomuk::WebExtractor::ResourceAnalyzerFactory::ResourceAnalyzerFactory(
    const ExtractParameters& extractParams,
    /*
    ResourceAnalyzer::LaunchPolitics launchPolitics,
    DecisionList::MergePolitics mergePolitics,
    unsigned int step,
    double acrit,
    double ucrit,
    */
    QObject * parent
):
    QObject(parent),
    d(new Private())
{
    d->dataPPKeeper = extractParams.plugins();
    d->step = extractParams.pluginSelectStep();
    d->launchPolitics = extractParams.launchPolitics();
    d->mergePolitics = extractParams.mergePolitics() ;
    d->ucrit = extractParams.uCrit() ;
    d->acrit = extractParams.aCrit() ;
    this->d->rsdManager = extractParams.resourceServiceDataManager();
}

NW::ResourceAnalyzerFactory::~ResourceAnalyzerFactory()
{
    delete d;
}

Nepomuk::WebExtractor::ResourceAnalyzer * Nepomuk::WebExtractor::ResourceAnalyzerFactory::newAnalyzer()
{
    ND::DecisionFactory * fct = 0;
        /*
         * DecisionFactory(
         * double ucrit,
         * double acrit,
         */
    fct = new ND::DecisionFactory(d->ucrit, d->acrit);

    fct->setThreshold(d->ucrit);
    return new Nepomuk::WebExtractor::ResourceAnalyzer(
               d->dataPPKeeper,
               fct,
	       this->d->rsdManager,
               this->d->mergePolitics,
               this->d->launchPolitics,
               d->acrit,
               d->ucrit,
               this->d->step
           );
}

Nepomuk::WebExtractor::ResourceAnalyzer * Nepomuk::WebExtractor::ResourceAnalyzerFactory::newAnalyzer(const Nepomuk::Resource & res)
{
    ResourceAnalyzer * resanal = newAnalyzer();
    if(resanal) {
        resanal->setResource(res);
    }
    return resanal;
}

void Nepomuk::WebExtractor::ResourceAnalyzerFactory::deleteAnalyzer(Nepomuk::WebExtractor::ResourceAnalyzer * res)
{
    if(res) res->deleteLater();
}

