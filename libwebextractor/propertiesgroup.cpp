#include "propertiesgroup.h"
#include "soprano_statement_qhash.h"
#include "decision.h"
#include <QtCore/QSet>
#include <QtCore/QSharedData>
#include "decisiondata.h"


namespace NW = Nepomuk::WebExtractor;

class NW::PropertiesGroup::Private : public QSharedData
{
    public:
        QSet< Soprano::Statement > statements;
        //Decision * parent;
        //const Soprano::Node mainTarget;
        double rank;
        unsigned int hash;
        /*Private(const Soprano::Node & mainTarget ):
        mainTarget(mainTarget)
        {;}
        */
        // Manager of the model
        //ResourceManager * manager;
        // This is the model where PropertiesGroup store all data. It is the
        // same as parent->model, extracted only for speed
        Soprano::Model * model;
        // This is the url of the context where properties group store all statements
        QUrl url;
        // Pointer to parent
        NW::DecisionData*  parent;

        // Human readble description
        QString description;
};

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
    d->model = parent->model;

}

NW::PropertiesGroup::~PropertiesGroup()
{
    ;
}

void NW::PropertiesGroup::createContext()
{
    if(this->d->url.isEmpty()) {
        // Register
        this->d->url = this->d->parent->createPropertiesGroupUrl();
    }
}

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

    createContext();
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

QUrl NW::PropertiesGroup::uri() const
{
    return d->url;
}

const Nepomuk::ResourceManager * NW::PropertiesGroup::manager() const
{
    return d->parent->manager;
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
    grp.addStatement(st);
    return grp;
}

unsigned int NW::qHash(const Nepomuk::WebExtractor::PropertiesGroup & g)
{
    return g.d->hash;
}
