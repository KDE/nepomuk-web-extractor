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

#ifndef _NEPOMUK_WEBEXTRCT_RESOURCE_ANALYZER_H_
#define _NEPOMUK_WEBEXTRCT_RESOURCE_ANALYZER_H_

#include <QtCore/QObject>
#include <QtCore/QQueue>
#include <Nepomuk/Resource>
#include <webextractor/webextractor_export.h>
#include <webextractor/datappwrapper.h>
#include <webextractor/datappwrapper.h>
#include <webextractor/global.h>

namespace Nepomuk {
    namespace WebExtractor {
	class ResourceAnalyzerFactory;
	class WEBEXTRACTOR_EXPORT ResourceAnalyzer : public QObject
	{
	    Q_OBJECT;
	    public:
		void analyze(Nepomuk::Resource & res);
	    Q_SIGNALS:
		void analyzingFinished();
	    private:
		ResourceAnalyzer(
			const DataPPKeeper & ,
		       	DecisionFactory * fact,
			WE::MergePolitics mergePolitics,
			WE::LaunchPolitics launchPolitics,
			double acrit,
			double ucrit,
			unsigned int step,
		       	QObject * parent = 0
			);
		// Only defenition, no implementation.
		// Copying is forbidden
		ResourceAnalyzer(const ResourceAnalyzer &);
		const ResourceAnalyzer & operator=( const ResourceAnalyzer &);
	    private Q_SLOTS:
		void pluginFinished();
		//void pluginError();
		bool launchNext();
		void launchOrFinish();
	    public:
		friend class ResourceAnalyzerFactory;
		class Private;
		Private * d;
	};
    }
}

#endif
