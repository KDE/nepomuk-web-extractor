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

#ifndef _NEPOMUK_WEBEXTRCT_CORE_CAT_IMPL_H_
#define _NEPOMUK_WEBEXTRCT_CORE_CAT_IMPL_H_

#include <QtCore/QString>
#include <QtCore/QUrl>
#include <QtCore/QQueue>
#include <QtCore/QDateTime>
#include <Soprano/QueryResultIterator>
#include <webextractor/resourceanalyzerfactory.h>
#include "webexcatscheduler.h"

namespace Nepomuk{
    namespace WE = WebExtractor;
    class WebExtractorCategorySchedulerImpl : public QObject
    {
	Q_OBJECT;
	public:
	WebExtractorCategorySchedulerImpl(const QString & category_query,WebExtractorCategoryScheduler * par, int maxResSimult, int cacheSize = 5);

	public Q_SLOTS:
	    /*! \brief Called when resource processing fineshed
	     */
	    void resourceProcessed();
	    /*! \brief Called when resource processing aborted
	     */
	    void resourceProcessingAborted();
	    void launch(const QUrl & resourceUri);

	Q_SIGNALS:
	    void launchPls(QUrl);
	    //void launchingResourceAborted();

	public:
	    bool launchNext();
	    void launchOrFinish();
	    bool start();
	    // Add new urls to cache
	    void cacheUrls();

	    bool checkQuery();
	    WebExtractorCategoryScheduler * m_par;
	    // This variable has same meaning as m_stopped
	    // but it is internal and can not be modificated
	    // by another thread
	    bool m_finishing;
	    QString m_query;
	    int m_respWaits;
	    // Maximum number of resources that can be processed simultaneously
	    int m_maxResSimult;
	    // Number of currently processed resources
	    int m_currentResProc;
	    int m_cacheSize;
	    //int tmp_count;
	    //Soprano::QueryResultIterator it;
	    WebExtractor::ResourceAnalyzerFactory * m_factory;
	    QQueue<QUrl> m_urlQueue;
    };
}
#endif
