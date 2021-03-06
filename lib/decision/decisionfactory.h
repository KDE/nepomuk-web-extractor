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

#ifndef _NEPOMUK_WEBEXTRCT_DESICION_FACTORY_H_
#define _NEPOMUK_WEBEXTRCT_DESICION_FACTORY_H_

#include <Soprano/BackendSettings>

#include "decisionlist.h"
#include "decisioncreator.h"

#include "decision_export.h"
namespace Soprano
{
    class StorageModel;
}

namespace Nepomuk
{
    namespace  Decision
    {
        class ResourceAnalyzerFactory;
        class IdentificationSetManager;
        class DecisionAuthor;
        class DECISION_EXPORT DecisionFactory
        {
            public:
                DecisionCreator newDecision(const DecisionAuthor * author) const;
                DecisionList  newDecisionList(const DecisionAuthor *) const;
                /*! \brief Debugging function
                 * Return DecisionFactory that can be used for debugging propose
                 * to avoid creation on unnecessary
                 * ResourceAnalyzers/ResourceAnalyzerFactory instances
                 */
                static DecisionFactory * debugFactory(double ucrit = 0, double acrit = 1);
                /*! \brief This function will return the aproximate threshold value.
                 * You can use this value to decide whether it make sense to generate more Decisions
                 * or not
                 */
                double approximateThreshold() const;
            public:
                /*! \brief Private constructor
                 * \param ucrit Value of the user threshold
                 * \param acrit Value of the auto applicable threshold
                 * \param autoDeleteModelData If set to true, then model must be
                 * non-Null value. In this case the model will be cleared ( via
                 * Backend::deleteModelData() method)
                         * \param decisionsModel This the the model that is used for working
                         * with Decisions.
                 * \param model This is storage model used for storing Decisions data.
                 * It may differ from decisionsModel, for example
                 * decisionsModel can be a filter model built atop of this storage
                 * model. But anyway it should decisionsModel must rely on this
                 * given storage model for storing statements.
                 * \param settings Settings that were used to create storage model. This is necessary to correctly remove it's data ( If requested)
                 */
                DecisionFactory(double ucrit, double acrit);
                ~DecisionFactory();
                DecisionList  newDecisionList() const;
                void setThreshold(double);
            private:
                class Private;
                Private * const d;
        };
    }
}

#endif
