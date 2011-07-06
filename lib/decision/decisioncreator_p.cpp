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


#include "algorithm.h"
#include <KDebug>
#include "decisioncreator_p.h"
#include "changelogfiltermodel.h"

#include "identificationset.h"

namespace ND = Nepomuk::Decision;
namespace NS = Nepomuk::Sync;

ND::DecisionCreatorInternals::DecisionCreatorInternals()
{
}

ND::DecisionCreatorInternals::DecisionCreatorInternals(
    const DecisionAuthor * parent
    )
{
    Q_CHECK_PTR(parent);

    this->authorsData.insert(parent);

    this->m_data.setTimeStamp(QTime::currentTime());
}

ND::DecisionCreatorInternals::~DecisionCreatorInternals()
{
    ;
    //kDebug() << "DecisionCreatorInternals is destroyed";
    /*
    if(manager)
        manager->deleteInstance();
    delete filterModel;
    */
}


/* ==== Constructing methods ==== */
ND::Decision ND::DecisionCreatorInternals::data()
{
    Decision d = m_data;
    // Add groups
    kDebug() << "Group creators count: " << groupCreators.size();
    foreach(const PropertiesGroupCreator & grp, groupCreators) {
	PropertiesGroup pg = grp.data();
	Q_ASSERT(pg.isValid());
	Q_ASSERT(pg.isEmpty() == grp.isEmpty());
	d.addGroup(pg);
    }

    // Set target resources


    Q_ASSERT(d.isValid());
    return d;
}

/* ==== Working with groups ==== */
ND::PropertiesGroupCreator ND::DecisionCreatorInternals::newGroup(QWeakPointer<DecisionCreatorInternals> weakRef)
{
    if ( !isValid() ) {
	kDebug() << "Create dummy PGC";
	return ND::PropertiesGroupCreator();
    }

    kDebug() << "Create real PGC";
    Q_ASSERT(!weakRef.isNull());

    // First create new property group
    PropertiesGroupCreator answer = PropertiesGroupCreator(weakRef);
    Q_ASSERT(answer.isValid());
    // Add it to the list
    groupCreators.append(answer);

    // Return it
    return answer;
}

bool ND::DecisionCreatorInternals::isValid() const
{
    //return (!contextUrl.isEmpty()) and(!authorsData.isEmpty());
    return (!authorsData.isEmpty());
}

/* ==== Editing Decision ==== */

#if 0
QString ND::DecisionCreatorInternals::description() const
{
    return data->description();
}


void ND::DecisionCreatorInternals::setDescription(const QString & description)
{
    data->setDescription(description);
}

#endif 



