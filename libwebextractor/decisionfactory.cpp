#include "decisionfactory.h"
#include "identsetmanager.h"
//#include "datapp.h"
#include "global.h"
#include <Soprano/BackendSettings>
#include <Soprano/StorageModel>

namespace NW = Nepomuk::WebExtractor;

namespace Soprano
{
    class Model;
}

class NW::DecisionFactory::Private
{
    public:
        Private();
        double threshold; // ucrit;
        double acrit;

        // Model where all Decisions are
        // stored
        Soprano::Model * decisionsModel;
        //Nepomuk::ResourceManager * manager;

        // If set to true, then model data
        // will be removed in DecisionFactory
        // destructor
        bool autoDeleteModelData;

        Soprano::BackendSettings settings;
        // Storage model is the same as decisionsModel
        // in most cases. But decisionsModel is
        // always based on the storageModel.
        // This member is necessary only for clearing
        // the model data.
        Soprano::StorageModel * storageModel;
        IdentificationSetManager * identsetManager;
};

NW::DecisionFactory::Private::Private():
    threshold(0),
    decisionsModel(0),
    storageModel(0),
    identsetManager(0)
{
    ;
}




Nepomuk::WebExtractor::DecisionFactory::DecisionFactory(double ucrit, double acrit, Soprano::Model * decisionsModel, bool autoDeleteModelData, Soprano::StorageModel * model, Soprano::BackendSettings settings):
    d(new Private())
{
    Q_ASSERT(acrit == Nepomuk::WebExtractor::boundACrit(acrit));
    Q_ASSERT(ucrit == Nepomuk::WebExtractor::boundUCrit(ucrit));
    d->threshold = ucrit;
    d->acrit = acrit;
    Q_ASSERT(decisionsModel);
    this->d->decisionsModel = decisionsModel;
    this->d->autoDeleteModelData = autoDeleteModelData;
    this->d->settings = settings;
    this->d->storageModel = model;
    this->d->identsetManager = new IdentificationSetManager();

    // Debug decisionsModel - list all statements
#if 0
    kDebug() << "Decisions model dump";
    Soprano::StatementIterator stit = decisionsModel->listStatements(Soprano::Node(), Soprano::Node(), Soprano::Node(), Soprano::Node(Nepomuk::Vocabulary::backupsync::backupsyncNamespace()));
    while(stit.next()) {
        kDebug() << *stit;
    }
#endif
    // Create ResourceManager atop of d->decisionsModel
    // TODO Add log filter model
    //this->d->manager = ResourceManager::createManagerForModel(d->decisionsModel);
}

void Nepomuk::WebExtractor::DecisionFactory::setThreshold(double threshold)
{
    d->threshold = threshold;
}

Nepomuk::WebExtractor::Decision Nepomuk::WebExtractor::DecisionFactory::newDecision(const DataPP * parent) const
{
    return Decision(parent, d->decisionsModel, d->identsetManager);
}

Nepomuk::WebExtractor::DecisionList  Nepomuk::WebExtractor::DecisionFactory::newDecisionList(const DataPP*) const
{
    Nepomuk::WebExtractor::DecisionList lst;
    lst.m_threshold = d->threshold;
    lst.m_acrit = d->acrit;
    return lst;
}



Nepomuk::WebExtractor::DecisionList  Nepomuk::WebExtractor::DecisionFactory::newDecisionList() const
{
    Nepomuk::WebExtractor::DecisionList lst;
    lst.m_threshold = d->threshold;
    lst.m_acrit = d->acrit;
    return lst;
}

Nepomuk::WebExtractor::DecisionFactory * NW::DecisionFactory::debugFactory(double ucrit, double acrit)
{
    // TODO Fixme
    static DecisionFactory * debugFactory = new DecisionFactory(ucrit, acrit, ResourceManager::instance()->mainModel(), false, 0);
    return debugFactory;
}

/*
void NW::DecisionFactory::setResourceManager( ResourceManager * m)
{
    Q_ASSERT(m);
    this->d->manager = m;
};
*/

NW::DecisionFactory::~DecisionFactory()
{

    //kDebug() << "Delete DecisionFactory";
    if(d->storageModel) {
        const Soprano::Backend * b = d->storageModel->backend();
        // Remove model if requested
        delete d->storageModel;

        // Clear model data if requested
        if(this->d->autoDeleteModelData) {
            kDebug() << "Delete model";
            b->deleteModelData(d->settings);
        }
    }

    delete d;



}
