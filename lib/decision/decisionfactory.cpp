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

};

ND::DecisionFactory::Private::Private():
    threshold(0)
{
    ;
}




Nepomuk::Decision::DecisionFactory::DecisionFactory(double ucrit, double acrit):
    d(new Private())
{
    Q_ASSERT(acrit == Nepomuk::Decision::boundACrit(acrit));
    Q_ASSERT(ucrit == Nepomuk::Decision::boundUCrit(ucrit));
    d->threshold = ucrit;
    d->acrit = acrit;
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
    static DecisionFactory * debugFactory = new DecisionFactory(ucrit, acrit);
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

    delete d;



}
