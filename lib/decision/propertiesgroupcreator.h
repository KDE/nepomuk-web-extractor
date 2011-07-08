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

#ifndef __properties_group_h_
#define __properties_group_h_

#include <QSharedPointer>
#include <QUrl>
#include <QString>
#include <Nepomuk/Resource>

#include "propertiesgroup.h"
#include "decision_export.h"


namespace Soprano
{
    class Model;
    class Statement;
}

namespace Nepomuk
{
    class ResourceManager;

    namespace Sync
    {
        class ChangeLog;
        class ChangeLogFilterModel;
    }

    namespace Decision
    {
        class Decision;
        class DecisionFactory;
	class DecisionCreatorInternals;
	class PropertiesGroupCreatorPrivate;
        /*! \brief PropertiesGroupCreator is a unit of changes that can be applied to the resource
         */
        class DECISION_EXPORT PropertiesGroupCreator
        {
            public:
		PropertiesGroupCreator();
		// The copy constructor
                PropertiesGroupCreator(const PropertiesGroupCreator &);
                ~PropertiesGroupCreator();
                const PropertiesGroupCreator & operator=(const PropertiesGroupCreator &);
                /*! \brief add statements to
                 */
                //void addStatement(Soprano::Statement);

                //void addStatement(const Soprano::Node & predicate, const Soprano::Node & second, bool subject = true );

		/* ==== Methods for adding changes to PG ==== */


                void setChanges( const SimpleResourceGraph & );


		/* ==== Method for manipulating current group ==== */
                
		/*! \brief Make this PropertiesGroupCreator current.
                 * All changes that will be done after that call and befor another
                 * PropertiesGroupCreator will become current with decision's ResourceManager
                 * will be added to this PropertiesGroupCreator
                 */
                void makeCurrent();

                /*! \brief This function will reset current group.
                 * This method will reset current group even if current group is not
                 * this one.
                 */
                void resetCurrent();

		/* ==== Methods for setting/getting parameters ==== */
                
		QString description() const;
                void setDescription(const QString &);


                /*! \brief This function will return log of all changes in this PropertiesGroupCreator
                 * \note Disabled as API is unstable
                 */
                //SimpleResourceGraph changes() const;

                /*! \brief Return rank of the PropertiesGroupCreator
                 * See setRank for detalis
                 */
                double rank() const;

                /*! \brief This function set the rank of this PropertiesGroupCreator
                 * The rank of the group is taken into account when automatically applying Decision.
                 * Greater rank will increase the chance that this PropertiesGroupCreator will be applied.
                 * Rank must meet common requirements to rank. Incorrect rank will be automaticaly
                 * trucated
                 */
                void setRank(double newRank);

		PropertiesGroup data() const;
                
		/* ==== Methods for accessing state of the object ==== */

                /*! \brief Return true if this PropertiesGroupCreator is valid
                 * The PropertiesGroupCreator is valid if the parent DecisionCreator
		 * still exists.
                 * PropertiesGroupCreator created with default constructor are invalid.
                 */
                bool isValid() const;

		bool isEmpty() const;

                bool operator==(const PropertiesGroupCreator & rhs)const;
                bool operator!=(const PropertiesGroupCreator & rhs)const;
                friend PropertiesGroupCreator & operator<<(PropertiesGroupCreator &, const Soprano::Statement &);
                friend class DecisionCreator;
		// DecisionCreatorInternals as friend class is needed for correct
		// updateModels() call working
                friend class DecisionCreatorInternals;
                friend unsigned int qHash(const PropertiesGroupCreator &);
            protected:
                SimpleResourceGraph changes() const;
                PropertiesGroupCreator( QWeakPointer<DecisionCreatorInternals> data);
		/*! \brief Return the collected data in form of PropertiesGroup
		 */
            private:
                QSharedPointer<PropertiesGroupCreatorPrivate> d;
		
		// Weak pointer to the main decision creator internal object
		// This main object is necessary for creating proxies of 
		// the real resources
		QWeakPointer<DecisionCreatorInternals> internals;	

        };
        PropertiesGroupCreator & operator<<(PropertiesGroupCreator &, const Soprano::Statement &);
    }
}

unsigned int qHash(const Nepomuk::Decision::PropertiesGroupCreator &);

#endif
