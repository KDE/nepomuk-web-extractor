/*
   Copyright (C) 2010 by Serebriyskiy Artem <v.for.vandal at gmail.com>

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#ifndef _NEPOMUK_WEBEXTRCT_DESICION_FACTORY_H_
#define _NEPOMUK_WEBEXTRCT_DESICION_FACTORY_H_

#include "webextractor_export.h"
#include "decision.h"
#include "decisionlist.h"
#include <Soprano/BackendSettings>

namespace Soprano
{
    class StorageModel;
}

namespace Nepomuk
{
    namespace  WebExtractor
    {
        class ResourceAnalyzerFactory;
        class DataPPReply;
        class IdentificationSetManager;
        class WEBEXTRACTOR_EXPORT DecisionFactory
        {
            public:
                friend class ResourceAnalyzerFactory;
                friend class ResourceAnalyzer;
                friend class DataPPReply;
                Decision newDecision(const DataPP * author) const;
                DecisionList  newDecisionList(const DataPP *) const;
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
            private:
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
                DecisionFactory(double ucrit, double acrit, Soprano::Model * decisionsModel, bool autoDeleteModelData, Soprano::StorageModel * model, Soprano::BackendSettings  settings = Soprano::BackendSettings());
                ~DecisionFactory();
                DecisionList  newDecisionList() const;
                void setThreshold(double);
                class Private;
                Private * const d;
        };
    }
}

#endif
