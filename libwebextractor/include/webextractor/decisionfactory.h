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

#include <webextractor/webextractor_export.h>
#include <webextractor/decision.h>
#include <webextractor/decisionlist.h>

namespace Nepomuk {
    namespace  WebExtractor {
	class ResourceAnalyzerFactory;
	class DataPPReply;
	class WEBEXTRACTOR_EXPORT DecisionFactory
	{
	    public:
		friend class ResourceAnalyzerFactory;
		friend class ResourceAnalyzer;
		friend class DataPPReply;
		Decision newDecision(const DataPP * author) const;
		DecisionList  newDecisionList( const DataPP * ) const;
	    private:
		DecisionList  newDecisionList( ) const;
		void setThreshold(double);
		DecisionFactory(double ucrit, double acrit);
		double m_threshold; // ucrit;
		double m_acrit;
	};
    }
}

#endif
