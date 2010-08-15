#include "propertiesgroup.h"
#include "soprano_statement_qhash.h"
#include "decision.h"
#include <QtCore/QSet>
#include <QtCore/QSharedData>
#include "decisiondata.h"
#include "nepomuk/changelog.h"
#include "changelogfiltermodel.h"


namespace NW = Nepomuk::WebExtractor;
namespace NS = Nepomuk::Sync;

class NW::PropertiesGroup::Private : public QSharedData
{
    public:
        Private();
        ~Private();
        QSet< Soprano::Statement > statements;
        //Decision * parent;
        //const Soprano::Node mainTarget;
        double rank;
        unsigned int hash;
        /*Private(const Soprano::Node & mainTarget ):
        mainTarget(mainTarget)
        {;}
        */
        // This is the model where PropertiesGroup store all data. It is the
        // decisions model, wrapped into logging filter model
        Soprano::Model * filterModel;
        // Manager of the filter model
        ResourceManager * manager;
        // This is the url of the context where properties group store all statements
        //QUrl url;
        // Pointer to parent
        NW::DecisionData*  parent;

        // Human readble description
        QString description;

        // ChangeLog. All changes are written here
        NS::ChangeLog log;

};

NW::PropertiesGroup::Private::Private():
    rank(-100),
    filterModel(0),
    manager(0),
    parent(0)
{
}

NW::PropertiesGroup::Private::~Private()
{
    delete filterModel;
    if(manager)
        manager->deleteInstance();
}

NW::PropertiesGroup::PropertiesGroup(/*QUrl mainResourceUrl, ResourceManager * manager*/ DecisionData * parent)
{
    //Q_ASSERT(manager);
    Q_ASSERT(parent);
    this->d = QSharedPointer<Private>(new NW::PropertiesGroup::Private());
    this->d->rank = 0;
    this->d->hash = 0;
    //this->d->url = url;
    //this->d->mainProxyResourceUrl = mainResourceUrl;
    //this->d->manager = manager;
    //d = new QSharedDataPointer<Private>(res);
    //d->parent = parent;
    //d->mainTarget = ;
    d->parent = parent;
    // By default filter model will be set to 0. When first time a manager function
    // will be accessed, this member will be initialized
    //d->filterModel = new ChangeLogFilterModel(parent->model);
    d->filterModel = 0;
    d->manager = 0;

}

NW::PropertiesGroup::PropertiesGroup()
{
    this->d = QSharedPointer<Private>(new NW::PropertiesGroup::Private());
}

NW::PropertiesGroup::~PropertiesGroup()
{
    ;
}

/*
void NW::PropertiesGroup::setUrl( const QUrl & url)
{
    this->d->url = url;
}

void NW::PropertiesGroup::registerGroup()
{
    if(this->d->url.isEmpty()) {
        // Register
        this->d->parent->registerGroup(this);
    }
}
*/
/*
void NW::PropertiesGroup::addStatement(Soprano::Statement  st)
{
    if(!d->parent) {  // PropertiesGroup is invalid
        kDebug() << "PropertiesGroup is invalid";
        return;
    }

    if(d->parent->isFreezed()) {  // Any changes to this Decision is now forbiden
        kDebug() << "PropertiesGroup is freezed";
        return;
    }

    registerGroup();
    if(d->url.isEmpty()) {
        kDebug() << "Failed to init PropertiesGroup";
        return;
    }
    //d->statements << st;

    // increase hash
    // TODO Analyze and implement more advanced hash function
    //d->hash ^= qHash(st);

    // Change the context of the statement
    // TODO Implement storing the original contexts
    st.setContext(d->url);
    d->model->addStatement(st);
}
*/

/*
QSet< Soprano::Statement > NW::PropertiesGroup::data() const
{
    return d->statements;
}
*/
/*
void NW::PropertiesGroup::addStatement(const Soprano::Node & predicate, const Soprano::Node & second, bool subject = true )
{
    if (subject) {
    d->statements << Soprano::Statement(d->mainTarget, predicate, second);
    }
    else {
    d->statements << Soprano::Statement(second, predicate, d->mainTarget);
    }


}
*/

double NW::PropertiesGroup::rank() const
{
    return d->rank;
}

void NW::PropertiesGroup::setRank(double newRank)
{
    d->rank = Decision::truncateRank(newRank);

}

/*
QUrl NW::PropertiesGroup::uri() const
{
    return d->url;
}
*/

Nepomuk::ResourceManager * NW::PropertiesGroup::manager()
{
    if(!d->parent)
        return 0;

    if(d->parent->isFreezed()) {  // Any changes to this Decision is now forbiden
        kDebug() << "PropertiesGroup is freezed";
        return 0;
    }

    if(!d->filterModel) {
        // This is first call. Create  a manager and a filter model
        initFilterModelManager();
    }
    return d->manager;
}

Soprano::Model * NW::PropertiesGroup::model()
{
    if(!d->parent)
        return 0;

    if(d->parent->isFreezed()) {  // Any changes to this Decision is now forbiden
        kDebug() << "PropertiesGroup is freezed";
        return 0;
    }

    if(!d->filterModel) {
        // This is first call. Create  a manager and a filter model
        initFilterModelManager();
    }
    return d->filterModel;
}

bool NW::PropertiesGroup::isValid() const
{
    return d->parent;
}

QString NW::PropertiesGroup::description() const
{
    return d->description;
}

void NW::PropertiesGroup::setDescription(const QString & description)
{
    d->description = description;
}

NS::ChangeLog NW::PropertiesGroup::log() const
{
    return d->log;
}


void NW::PropertiesGroup::makeCurrent()
{
    // Return if we are invalid
    if(!d->parent)
        return;

    //registerGroup();

    d->parent->setCurrentGroup(*this);
}

void NW::PropertiesGroup::resetCurrent()
{
    if(!d->parent)
        return;

    d->parent->resetCurrentGroup();
}

Nepomuk::Sync::ChangeLog * NW::PropertiesGroup::logPtr() const
{
    return &(d->log);
}

void NW::PropertiesGroup::initFilterModelManager()
{
    d->filterModel = new NS::ChangeLogFilterModel(&d->log, d->parent->decisionsModel);
    d->manager = ResourceManager::createManagerForModel(d->filterModel);
}
/*
QUrl NW::PropertiesGroup::mainProxyResourceUrl()
{
    return d->mainProxyResourceUrl;
}
*/

NW::PropertiesGroup::PropertiesGroup(const PropertiesGroup & rhs)
{
    this->d = rhs.d;
}
const NW::PropertiesGroup & NW::PropertiesGroup::operator=(const PropertiesGroup & rhs)
{
    if(this == &rhs)
        return *this;
    this->d = rhs.d;
    return *this;
}

bool NW::PropertiesGroup::operator==(const PropertiesGroup & rhs) const
{
    if(this == &rhs)
        return true;

    if(this->d == rhs.d)
        return true;

#warning IMPLEMENT THIS!
    return false; /*(this->d->mainTarget == rhs.d.mainTarget ) and */
    //(this->d->statements == rhs.d->statements);
}


bool NW::PropertiesGroup::operator!=(const PropertiesGroup & rhs) const
{
    return !(*this == rhs);
}


NW::PropertiesGroup & NW::operator<<(NW::PropertiesGroup & grp, const Soprano::Statement & st)
{

    if(!grp.model())
        return grp;

    grp.model()->addStatement(st);
    return grp;
}

unsigned int NW::qHash(const Nepomuk::WebExtractor::PropertiesGroup & g)
{
    return g.d->hash;
}
