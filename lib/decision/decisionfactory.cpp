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

#include "decisionfactory.h"
#include "identsetmanager.h"
//#include "executive.h"
#include "global.h"
#include <Soprano/BackendSettings>
#include <Soprano/StorageModel>

namespace ND = Nepomuk::Decision;

namespace Soprano
{
    class Model;
}

class ND::DecisionFactory::Private
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

ND::DecisionFactory::Private::Private():
    threshold(0),
    decisionsModel(0),
    storageModel(0),
    identsetManager(0)
{
    ;
}




Nepomuk::Decision::DecisionFactory::DecisionFactory(double ucrit, double acrit, Soprano::Model * decisionsModel, bool autoDeleteModelData, Soprano::StorageModel * model, Soprano::BackendSettings settings):
    d(new Private())
{
    Q_ASSERT(acrit == Nepomuk::Decision::boundACrit(acrit));
    Q_ASSERT(ucrit == Nepomuk::Decision::boundUCrit(ucrit));
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

void Nepomuk::Decision::DecisionFactory::setThreshold(double threshold)
{
    d->threshold = threshold;
}

Nepomuk::Decision::DecisionCreator Nepomuk::Decision::DecisionFactory::newDecision(const DecisionAuthor * parent) const
{
    DecisionCreator dc(parent);
    Q_ASSERT(dc.isValid());
    return dc;
}

Nepomuk::Decision::DecisionList  Nepomuk::Decision::DecisionFactory::newDecisionList(const DecisionAuthor*) const
{
    Nepomuk::Decision::DecisionList lst;
    lst.m_threshold = d->threshold;
    lst.m_acrit = d->acrit;
    return lst;
}



Nepomuk::Decision::DecisionList  Nepomuk::Decision::DecisionFactory::newDecisionList() const
{
    Nepomuk::Decision::DecisionList lst;
    lst.m_threshold = d->threshold;
    lst.m_acrit = d->acrit;
    return lst;
}

Nepomuk::Decision::DecisionFactory * ND::DecisionFactory::debugFactory(double ucrit, double acrit)
{
    // TODO Fixme
    static DecisionFactory * debugFactory = new DecisionFactory(ucrit, acrit, ResourceManager::instance()->mainModel(), false, 0);
    return debugFactory;
}

/*
void ND::DecisionFactory::setResourceManager( ResourceManager * m)
{
    Q_ASSERT(m);
    this->d->manager = m;
};
*/

double ND::DecisionFactory::approximateThreshold() const
{
    // TODO Add more comprehansive way of determining
    // aproximate threshold
    return d->threshold;
}

ND::DecisionFactory::~DecisionFactory()
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
