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

namespace Nepomuk {
    namespace WebExtractor {
	class ResourceAnalyzerImpl : public QObject
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