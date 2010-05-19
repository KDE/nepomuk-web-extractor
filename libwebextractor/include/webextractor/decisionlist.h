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

namespace Nepomuk {
    namespace WebExtractor {
	class WEBEXTRACTOR_EXPORT DecisionList : private QList<Decision>
	{
	    public:
		friend class ResourceAnalyzerImpl;
		friend class DecisionFactory;
		void addDecision( const Decision & );
		void addDecision( const Decision & , WE::MergePolitics politics, double coff = 1);
		void mergeWith( const DecisionList & rhs, double scale, WE::MergePolitics policis = WE::Highest, double coff = 1);
		void scale( double coff);
		/*
		using QList<Decision>::size;
		
		using QList<Decision>::iterator;
		using QList<Decision>::begin;
		*/
	    private:
		// Leave only unique instances
		DecisionList(double threshold = 0);
		void unique( WE::MergePolitics policis = WE::Highest, double coff = 1 );
		void sort();
		bool hasAutoApplicable() const;
		double m_threshold;

	};
    }
}

#endif
