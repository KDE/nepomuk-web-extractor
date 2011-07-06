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

#ifndef _NEPOMUK_WEBEXTRCT_DESICION_CREATOR_H_
#define _NEPOMUK_WEBEXTRCT_DESICION_CREATOR_H_

#include <QString>
#include <QUrl>
#include <QList>
#include <QSet>
#include <QHash>
#include <QSharedPointer>

#include <Nepomuk/Resource>

#include "propertiesgroupcreator.h"
#include "decision.h"

#include "decision_export.h"
//#include "soprano_statement_qhash.h"

namespace Soprano
{
    class Model;
}

namespace Nepomuk
{
    class ResourceManager;

    namespace Sync {
        class ChangeLog;
    }
    
    namespace Decision
    {
        class DecisionFactory;
        class DecisionList;
        class DecisionAuthor;
        class PropertiesGroup;
        class IdentificationSetManager;

        class DecisionCreatorInternals;

        /*! \brief This class represent one DecisionCreator - set of statements that can be applied to resource
         * DecisionCreator consist of PropertiesGroup. PropertiesGroup is a set of statements
         * that can be applied only together. DecisionCreators are implicitly shared objects
	 * and changing copy of the DecisionCreator will change original too. Because of this,
	 * this class is not thread-safe.
         */
        class DECISION_EXPORT DecisionCreator /*: public QOb*/
        {
            public:
                /*! \brief Public constructor
                 * This constructor will create invalid DecisionCreator
                 */
                DecisionCreator();
                ~DecisionCreator();
                DecisionCreator(const DecisionCreator &);
		DecisionCreator & operator=( const DecisionCreator & );
                bool operator==(const DecisionCreator & rhs)const;
                bool operator!=(const DecisionCreator & rhs)const;

                double rank() const;
                void setRank(double rank);

                /*! \brief Create new group bounded to the DecisionCreator
                 * Return  new PropertiesGroup, invalid if any error has
                 * occured
                 */
                PropertiesGroupCreator newGroup();

		/*! \brief Return the default PropertiesGroupCreator
		 * Each decision creator has a default PGC that log all changes that
		 * are done
		 */
		PropertiesGroupCreator defaultGroup();


		/*! \brief Returns true if Decision doesn't conatins any statement
		 */
                bool isEmpty() const;

                /*! \brief Check the validness of the DecisionCreator.
                 * It is easier to detirmine invalid DecisionCreator. Decision created with
                 * DecisionCreator() constructor is invalid. Most others will be valid.
                 */
                bool isValid() const;

                int   dataPPVersion() const;
                /*! \brief Return information about all Executive that create this DecisionCreator
                 *\return Set of the Executive
                 */
                
		/*! \brief Return description of the DecisionCreator.
                 * It is rich text. The description is for displaying to user.
                 * \return Description of the DecisionCreator.
                 */
                QString description() const;
                
		void setDescription(const QString &);

		/*! \brief Return the Decision
		 * It will return a <b>copy</b> of the internal Decision.
		 * You can change it further as much as you want, it would not
		 * impact DecisionCreator internal structures.
		 */
		Decision data() const;

                /* ===== Decision creation section ========= */
                void setChanges( const SimpleResourceGraph & );



                friend class DecisionFactory;
                friend class DecisionList;
                friend class PropertiesGroup;
                friend unsigned int qHash(const DecisionCreator &);
            protected:
                /*! \brief Constructor
                 * \param parent The parent of the DecisionCreator - Executive that has created it
                 * \param manager The manager of the model where all information should
                 * be stored
                 */
                DecisionCreator(
                    const DecisionAuthor * parent
                );
                //void addAuthor(const Executive * author);
                //unsigned int timeStamp() const;
                //QSharedDataPointer<Private> d;
                // ATTENTION The d-pointer must ALWAYS be non-null.
                QSharedPointer<DecisionCreatorInternals> d;
                //QSharedPointer<Private> d;

        };
        //unsigned int qHash(const Nepomuk::Decision::DecisionCreator &);
    }
}

#endif
