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

#include "propertiesgroup.h"
#include <QtCore/QSet>
#include <QtCore/QSharedData>
#include "decisiondata.h"
#include "nepomuk/changelog.h"
#include "changelogfiltermodel.h"
#include <Soprano/Model>
#include <Nepomuk/ResourceManager>


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
        Sync::ChangeLogFilterModel * filterModel;
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
    d->parent->data.insert(*this);

}

NW::PropertiesGroup::PropertiesGroup()
{
    this->d = QSharedPointer<Private>(new NW::PropertiesGroup::Private());
}

NW::PropertiesGroup::~PropertiesGroup()
{
    ;
}

QUrl NW::PropertiesGroup::proxyUrl(const Nepomuk::Resource & res)
{
    if(!d->filterModel)
        initFilterModel();

    return d->parent->proxyUrl(res);
}

Nepomuk::Resource NW::PropertiesGroup::proxyResource(const Nepomuk::Resource & res)
{
    // Call proxyUrl. This call is important, becaus
    // in proxyUrl filterModel will be initialize if
    // necessary
    QUrl answer = proxyUrl(res);

    if(!d->manager)
        initFilterManager();
    // Create resoruce and return it.
    return Nepomuk::Resource(answer, QUrl(), d->manager);
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

    if(!d->manager) {
        // This is first call. Create  a manager and a filter model
        initFilterManager();
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
        initFilterModel();
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

NS::ChangeLogFilterModel * NW::PropertiesGroup::filterModel() const
{
    return d->filterModel;
}

void NW::PropertiesGroup::initFilterModel()
{
    d->filterModel = new NS::ChangeLogFilterModel(&d->log, d->parent->decisionsModel, QSet<QUrl>(), NS::ChangeLogFilterModel::Decline);
    // Now take current proxy url and add them as targets
    foreach(const QUrl & proxyUrl, d->parent->resourceProxyMap) {
        d->filterModel->addTarget(proxyUrl);
    }

}

void NW::PropertiesGroup::initFilterManager()
{
    if(!d->filterModel)
        initFilterModel();

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
