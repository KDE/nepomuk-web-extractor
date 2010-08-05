#include "decisionfactory.h"
//#include "datapp.h"
#include "global.h"

namespace NW = Nepomuk::WebExtractor;

Nepomuk::WebExtractor::DecisionFactory::DecisionFactory(double ucrit, double acrit, Soprano::Model * decisionsModel, bool autoDeleteModelData, Soprano::StorageModel * model, Soprano::BackendSettings settings):
    m_threshold(0)
{
    Q_ASSERT(acrit == Nepomuk::WebExtractor::WE::boundACrit(acrit));
    Q_ASSERT(ucrit == Nepomuk::WebExtractor::WE::boundUCrit(ucrit));
    m_threshold = ucrit;
    m_acrit = acrit;
    Q_ASSERT(decisionsModel);
    this->m_decisionsModel = decisionsModel;
    this->m_autoDeleteModelData = autoDeleteModelData;
    this->m_settings = settings;
    this->m_storageModel = model;
    this->m_identsetManager = new IdentificationSetManager();

    // Create ResourceManager atop of m_decisionsModel
    // TODO Add log filter model
    //this->m_manager = ResourceManager::createManagerForModel(m_decisionsModel);
}

void Nepomuk::WebExtractor::DecisionFactory::setThreshold(double threshold)
{
    m_threshold = threshold;
}

Nepomuk::WebExtractor::Decision Nepomuk::WebExtractor::DecisionFactory::newDecision(const DataPP * parent) const
{
    return Decision(parent, m_decisionsModel, m_identsetManager);
}

Nepomuk::WebExtractor::DecisionList  Nepomuk::WebExtractor::DecisionFactory::newDecisionList(const DataPP*) const
{
    Nepomuk::WebExtractor::DecisionList lst;
    lst.m_threshold = m_threshold;
    lst.m_acrit = m_acrit;
    return lst;
}

/*
Nepomuk::WebExtractor::PropertiesGroup Nepomuk::WebExtractor::DecisionFactory::newPropertiesGroup(const DataPP * parent) const
{
     return PropertiesGroup();
}
*/


Nepomuk::WebExtractor::DecisionList  Nepomuk::WebExtractor::DecisionFactory::newDecisionList() const
{
    Nepomuk::WebExtractor::DecisionList lst;
    lst.m_threshold = m_threshold;
    lst.m_acrit = m_acrit;
    return lst;
}

Nepomuk::WebExtractor::DecisionFactory * NW::DecisionFactory::debugFactory(double ucrit, double acrit)
{
    // TODO Fixme
    static DecisionFactory * m_factory = new DecisionFactory(ucrit, acrit, ResourceManager::instance()->mainModel(), false, 0);
    return m_factory;
}

/*
void NW::DecisionFactory::setResourceManager( ResourceManager * m)
{
    Q_ASSERT(m);
    this->m_manager = m;
};
*/

NW::DecisionFactory::~DecisionFactory()
{

    //kDebug() << "Delete DecisionFactory";
    if(m_storageModel) {
        const Soprano::Backend * b = m_storageModel->backend();
        // Remove model if requested
        delete m_storageModel;

        // Clear model data if requested
        if(this->m_autoDeleteModelData) {
            kDebug() << "Delete model";
            b->deleteModelData(m_settings);
        }
    }



}
