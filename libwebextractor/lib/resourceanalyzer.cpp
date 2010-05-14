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
#include "resourceanalyzer.h"
#include <KDebug>
#include <QtCore/QTimer>
#include <assert.h>
#include "resourceanalyzerimpl.h"

/*
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

*/

Nepomuk::WebExtractor::ResourceAnalyzer::ResourceAnalyzer(QObject * parent):
    QObject(parent)
{
    m_analyzer = new Nepomuk::WebExtractor/*::ResourceAnalyzer*/::ResourceAnalyzerImpl(this);
    connect(m_analyzer,SIGNAL(analyzingFinished()),this, SIGNAL(analyzingFinished()));
}

void Nepomuk::WebExtractor::ResourceAnalyzer::analyze( Nepomuk::Resource & res)
{
    m_analyzer->analyze(res);
}

Nepomuk::WebExtractor/*::ResourceAnalyzer*/::ResourceAnalyzerImpl::ResourceAnalyzerImpl(QObject * parent):
    QObject(parent),
    tmp_count(15),
    m_respWaits(0)
{;}

void Nepomuk::WebExtractor/*::ResourceAnalyzer*/::ResourceAnalyzerImpl::analyze(Nepomuk::Resource & res)
{
    // Make some work here
    
    // start processing
    kDebug() << "Extracting data from resource";
    launchNext();
}
bool Nepomuk::WebExtractor/*::ResourceAnalyzer*/::ResourceAnalyzerImpl::launchNext()
{
    assert(m_respWaits == 0);
    if (!tmp_count)
	return false;

    kDebug() << "Launching next portion of plugins";

    int c = std::min(5,tmp_count);
    for (int i = 0; i < c; i++)
	QTimer::singleShot((i+1)*(i+1)*1000,this, SLOT(pluginFinished()));

    m_respWaits = c;
    tmp_count -= c;
    return true;
}
void Nepomuk::WebExtractor/*::ResourceAnalyzer*/::ResourceAnalyzerImpl::pluginFinished()
{
    m_respWaits--;
    kDebug() << "Recived answer from plugin.";

    // Process data plugin has returned
    
    if (m_respWaits == 0) {
	// All launched plugins return data
	// Process it
	// <bzzzzz>
	// Launching other plugins if necessary
	if ( !launchNext() ) {
	    // No more plugins to launch and all plugins launched before
	    // returned their data
	    kDebug() << "Extracting for resource finished";
	    emit analyzingFinished();
	}
    }
    /*
    else {
       kDebug() << "Only " << m_respWaits << " answers remaining";
    }
    */
}
