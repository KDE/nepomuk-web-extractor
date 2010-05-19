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

#ifndef _NEPOMUK_WEBEXTRCT_RESOURCE_ANALYZER_IMPL_H_
#define _NEPOMUK_WEBEXTRCT_RESOURCE_ANALYZER_IMPL_H_

#include <QtCore/QObject>
#include <Nepomuk/Resource>
#include <webextractor/resourceanalyzer.h>

namespace Nepomuk {
    namespace WebExtractor {
	class ResourceAnalyzerImpl : public QObject
	{
	    Q_OBJECT;
	    public:
		ResourceAnalyzerImpl(const DataPPKeeper & dataPPKeeper, DecisionFactory * fact, QObject * parent = 0);
		void analyze( Nepomuk::Resource & res);
	    Q_SIGNALS:
		void analyzingFinished();
	    public:
		int tmp_count;
		int m_respWaits;
		const DataPPKeeper & m_dataPPKeeper;
		DataPPKeeper::const_iterator it;
		DecisionFactory * m_fact;
		QMap< DataPPReply*, double > m_replyAndRanks;
		WE::LaunchPolitics m_launchPolitics;
		unsigned int m_step;
		DecisionList m_decisions;
		WE::MergePolitics m_mergePolitics;
		double m_mergeCoff;
		Nepomuk::Resource m_res;
		double m_acrit;
		double m_ucrit;

	    private Q_SLOTS:
		void pluginFinished();
		//void pluginError();
		bool launchNext();
		void launchOrFinish();
	};
    }
}
/*
class Nepomuk::WebExtractor::ResourceAnalyzer::ResourceAnalyzerImpl : public QObject
{
    Q_OBJECT;
    public:
	ResourceAnalyzerImpl(QObject * parent = 0);
	void analyze( Nepomuk::Resource & res);
    Q_SIGNALS:
	void analyzingFinished();
    private:
	int tmp_count;
	int m_respWaits;
    private Q_SLOTS:
	void pluginFinished();
    	bool launchNext();
};
*/

#endif
