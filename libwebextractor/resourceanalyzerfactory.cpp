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
#include "ontologyloader.h"
#include <KDebug>
#include <Soprano/Global>
#include <Nepomuk/Tag>
#include <Soprano/StorageModel>
#include "executive.h"

namespace NW = Nepomuk::WebExtractor;

class NW::ResourceAnalyzerFactory::Private
{
    public:
        Private();
        ExecutiveKeeper  dataPPKeeper;

#warning trueg: Instead of copying every valus why not simply store an ExtractParameters instance?
        LaunchPolitics launchPolitics;
        MergePolitics mergePolitics;
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

        // Model for storing Decisions in memory
        //Soprano::StorageModel * decisionsStorageModel;
        // ResourceManager that manages this model. This pointer is non-null
        // only if extractParams.manager() was non-null.
        // ResourceManager * decisionsResourceManager;
        Soprano::Model * decisionsMainModel;
        OntologyLoader * decisionsMainModelOntologyLoader;
        const Soprano::Backend * backend;
        Soprano::BackendSettings backendSettings;
	ResourceServiceDataManager * rsdManager;
};


NW::ResourceAnalyzerFactory::Private::Private():
    launchPolitics(StepWise),
    mergePolitics(Highest),
    step(10),
    //decisionsResourceManager(0),
    decisionsMainModel(0),
    decisionsMainModelOntologyLoader(0),
    backend(0),
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
    d->autoManageOntologies = extractParams.autoManageOntologies();
    // We need backend only if model for Decisions is not provided.
    this->d->decisionsMainModel = extractParams.decisionsModel();
    if(!this->d->decisionsMainModel) {
        QString backendName = extractParams.backendName();
        if(backendName.isEmpty()) {
            d->backend = Soprano::discoverBackendByFeatures(Soprano::BackendFeatureStorageMemory);
        } else { // Trying to found specified backend
            d->backend = Soprano::discoverBackendByName(backendName);
            // If not found, then trying to found default backend
            if(!d->backend)
                d->backend = Soprano::discoverBackendByFeatures(Soprano::BackendFeatureStorageMemory);
        }

        //kDebug() << "ACrit: " << d->acrit;
        //kDebug() << "UCrit: " << d->ucrit;
        d->backendSettings = extractParams.backendSettings();
    } else {
        // Model is provided. Check for ontologies.
        // If autoManageOntologies is set, then we should
        // load ontologies to the model
        if(d->autoManageOntologies) {
            // Init main ontology loader
            this->d->decisionsMainModelOntologyLoader =
                    new OntologyLoader(this->d->decisionsMainModel, this);
            this->d->decisionsMainModelOntologyLoader->updateLocalOntologies(true);
        }
    }

    //decisionsStorageModel = 0;
    //decisionsResourceManager = 0;
    this->d->autoDeleteModelData = extractParams.autoDeleteModelData();
    this->d->rsdManager = extractParams.resourceServiceDataManager();
}

NW::ResourceAnalyzerFactory::~ResourceAnalyzerFactory()
{
    delete d->decisionsMainModelOntologyLoader;
    delete d;
}

Nepomuk::WebExtractor::ResourceAnalyzer * Nepomuk::WebExtractor::ResourceAnalyzerFactory::newAnalyzer()
{
    DecisionFactory * fct = 0;
    if(this->d->decisionsMainModel) {
        /*
         * DecisionFactory(
         * double ucrit,
         * double acrit,
         * Soprano::Model * decisionsModel,
         * bool autoDeleteModelData,
         * Soprano::StorageModel * model, // Set this if DecisionFactory own the model
         * Soprano::BackendSettings settings)
         */
        fct = new DecisionFactory(d->ucrit, d->acrit, d->decisionsMainModel, false, 0);
    } else {
        const Soprano::Backend * b = d->backend;
        if(b) {
            Nepomuk::ResourceManager * adrm = 0;
            // Initialize model
            Soprano::StorageModel * decisionsStorageModel = b->createModel(d->backendSettings);
            if(!decisionsStorageModel) {
                return 0;
            }

            // Initialize ResourceManager
            //adrm = ResourceManager::createManagerForModel((Soprano::Model*)(decisionsStorageModel));

            // Check the model
            /*
            #warning DISABLE THIS CODE! IT POLLUTES MODEL
            Nepomuk::Tag t(QString("test_tag"),adrm);
            t.setDescription("tag to test redland model");
            kDebug() << "Tag successfuly generated: " <<t.resourceUri();
            QUrl uu = adrm->generateUniqueUri("res");
            */

            // Load ontologies
            OntologyLoader * loader = new OntologyLoader(decisionsStorageModel);
            loader->updateLocalOntologies(true);
            delete loader;

            fct = new DecisionFactory(d->ucrit, d->acrit, decisionsStorageModel, d->autoDeleteModelData, decisionsStorageModel, d->backendSettings);
        } else {
            return 0;
        }
    }

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

const Soprano::Backend * NW::ResourceAnalyzerFactory::backend() const
{
    return d->backend;
}

Soprano::BackendSettings NW::ResourceAnalyzerFactory::backendSettings() const
{
    return d->backendSettings;
}

/*
const Soprano::Backend * NW::ResourceAnalyzerFactory::usedBackend()
{
    //Soprano::BackendSettings settings;
    //settings << Soprano::BackendOptionStorageMemory;
    static const Soprano::Backend * backend = Soprano::discoverBackendByFeatures(Soprano::BackendFeatureStorageMemory);
    return backend;
}
*/
