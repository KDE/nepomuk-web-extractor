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

#ifndef _NEPOMUK_WEBEXTRCT_DESICION_LIST_H
#define _NEPOMUK_WEBEXTRCT_DESICION_LIST_H

#include "decision.h"
#include <KDebug>
#include "webextractor_export.h"
#include "global.h"
#include <QList>

namespace Nepomuk
{
    namespace WebExtractor
    {
        class Executive;
        class WEBEXTRACTOR_EXPORT DecisionList : private QList<Decision>
        {
            public:
                friend class ResourceAnalyzer;
                friend class DecisionFactory;
                DecisionList(double threshold = 0);
                /*! \brief Add decision to the list
                 * The given Decision will be added to the list. If there is the same Decision in this list, then
                 * these Decisions will be merget into one using default merging politics.
                 * \param noAuto If true, then this Decision will be forced to have rank less than a_crit
                 */
                void addDecision(const Decision &, bool noAuto = false);

                /*! \brief Add decision to the list using given mereg politics
                 * The given Decision will be added to the list. If there is the same Decision in this list, then
                 * these Decisions will be merget into one using given merging politics.
                 * \param noAuto If true, then this Decision will be forced to have rank less than a_crit
                 * \param politics Merging politics
                 * \param coff Ignored
                 */
                void addDecision(const Decision & , MergePolitics politics, double coff = 1, bool noAuto = false);
                /*! \brief Merge one DecisionLis with another
                 * The same as add all Decisions from one list to another using given settings
                 */
                void mergeWith(const DecisionList & rhs, MergePolitics policis = Highest, double coff = 1);
                using QList<Decision>::constBegin ;
                using QList<Decision>::constEnd ;
                using QList<Decision>::const_iterator;
                using QList<Decision>::size;
                using QList<Decision>::clear;
                const_iterator begin() const;
                const_iterator end() const;
                double approximateThreshold() const;
            private:
                // Leave only unique instances
                //void unique( MergePolitics policis = Highest, double coff = 1 );
                //void sort();
                bool hasAutoApplicable() const;
                QSet<const Executive*> filterObsolete();
                void addDecisionUnscaled(Decision & , MergePolitics politics, double coff = 1);
                void filter(double threshold);
                void addToUserDiscretion() const;
                const Decision & best() const;
                double m_threshold;
                double m_acrit;
                bool m_trusted;
                double m_scaleCoff;
                bool m_hasAutoApplicable;
                Decision m_best;
                static double scaledRank(double rank, double coff);


        };
    }
}

#endif
