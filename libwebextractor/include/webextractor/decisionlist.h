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

#ifndef _NEPOMUK_WEBEXTRCT_DESICION_LIST_H
#define _NEPOMUK_WEBEXTRCT_DESICION_LIST_H

#include <webextractor/decision.h>
#include <KDebug>
#include <webextractor/webextractor_export.h>
#include <webextractor/global.h>
#include <QtCore/QSet>

namespace Nepomuk {
    namespace WebExtractor {
	class DataPP;
	class WEBEXTRACTOR_EXPORT DecisionList : private QSet<Decision>
	{
	    public:
		friend class ResourceAnalyzer;
		friend class DecisionFactory;
		void addDecision( const Decision &, bool noAuto = false );
		void addDecision( const Decision & , WE::MergePolitics politics, double coff = 1, bool noAuto = false);
		void mergeWith( const DecisionList & rhs, WE::MergePolitics policis = WE::Highest, double coff = 1);
		//void scale( double coff);
		/*! \brief return names of plugins that produce obsolete decisions
		 */
		/*
		using QList<Decision>::size;
		
		using QList<Decision>::iterator;
		using QList<Decision>::begin;
		*/
	    private:
		DecisionList(double threshold = 0);
		// Leave only unique instances
		//void unique( WE::MergePolitics policis = WE::Highest, double coff = 1 );
		//void sort();
		bool hasAutoApplicable() const;
		QSet<const DataPP*> filterObsolete();
		void addDecisionUnscaled( Decision & , WE::MergePolitics politics, double coff = 1 );
		void filter( double threshold);
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
