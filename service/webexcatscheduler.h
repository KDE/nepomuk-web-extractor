/*
   Copyright (c) 2008-2010 Sebastian Trueg <trueg@kde.org>
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

#ifndef _NEPOMUK_WEBEXTRCT_CAT_SCH_H_
#define _NEPOMUK_WEBEXTRCT_CAT_SCH_H_

#include <QtCore/QThread>
#include <QtCore/QMutex>
#include <QtCore/QWaitCondition>
#include <QtCore/QSet>
#include <QtCore/QUrl>
#include <QtCore/QQueue>
#include <QtCore/QDateTime>
#include <Soprano/QueryResultIterator>
#include <webextractor/resourceanalyzerfactory.h>
#include <webextractor/parameters.h>

namespace Nepomuk{
    namespace WE = WebExtractor;
   // class WebExtractorCategorySchedulerImpl;
    class WebExtractorCategoryScheduler : public QThread
    {
	Q_OBJECT;

	public:
	    WebExtractorCategoryScheduler( 
		    const QString & category_query,
		    QObject * parent, 
		    WebExtractor::ExtractParametersPtr params,
		    int maxResSimult = 5
		    );
	    ~WebExtractorCategoryScheduler();

	    bool isSuspended() const;
	    bool isExtracting() const;
	    int maxResSimult() const { return m_maxResSimult; }

	    enum ExtractingSpeed {
		/**
		 * Index at full speed, i.e. do not use any artificial
		 * delays.
		 *
		 * This is the mode used if the user is "away".
		 */
		FullSpeed = 0,

		/**
		 * Reduce the extracting speed mildly. This is the normal
		 * mode used while the user works. The indexer uses small
		 * delay between extracting two files in order to keep the
		 * load on CPU and IO down.
		 */
		ReducedSpeed,

		/**
		 * Like ReducedSpeed delays are used but they are much longer
		 * to get even less CPU and IO load. This mode is used for the
		 * first 2 minutes after startup to give the KDE session manager
		 * time to start up the KDE session rapidly.
		 */
		SnailPace
	    };

	    ExtractingSpeed currentSpeed() const { return m_speed; }
	public Q_SLOTS:
	    void suspend();
	    void resume();
	    void stop();
	    void restart();

	    void setExtractingSpeed( ExtractingSpeed speed );

	    /**
	     * A convinience slot which calls setExtractingSpeed
	     * with either FullSpeed or ReducedSpeed, based on the
	     * value of \p reduced.
	     */
	    void setReducedExtractingSpeed( bool reduced = false );

	    void setSuspended( bool );
	    void setMaxResSimult(int new_mrsm);

	    /*! \brief Called when resource processing fineshed
	     */
	    void resourceProcessed();
	    /*! \brief Called when resource processing aborted
	     */
	    void resourceProcessingAborted();
	    void launch(const QUrl & resourceUri);
	    // Debug only
	    void mseg();
	Q_SIGNALS:
	    void extractingStarted();
	    void extractingStopped();
	    void extractingFolder( const QString& );
	    void extractingSuspended( bool suspended );
	    void launchPls(QUrl);

	private:
	    void run();
	    // emits indexingStarted or indexingStopped based on parameter. Makes sure
	    // no signal is emitted twice
	    void setExtractingStarted( bool started );
	    // Return true if thread is not stopped
	    bool waitForContinue( bool disableDelay = false );

	    // Start extracting for next resource in query


	    bool launchNext();
	    void launchOrFinish();
	    bool startLaunch();
	    // Add new urls to cache
	    void cacheUrls();

	    bool checkQuery();


	    // This variable has same meaning as m_stopped
	    // but it is internal and can not be modificated
	    // by another thread
	    //bool m_finishing;
	    QString m_query;
	    int m_respWaits;
	    // Maximum number of resources that can be processed simultaneously
	    int m_maxResSimult;
	    // Number of currently processed resources
	    int m_currentResProc;
	    int m_cacheSize;
	    //Soprano::QueryResultIterator it;
	    WebExtractor::ResourceAnalyzerFactory * m_factory;
	    QQueue<QUrl> m_urlQueue;
	    WebExtractor::ExtractParametersPtr m_extractParams;

	    bool m_suspended;
	    bool m_stopped;
	    bool m_extracting;
	    QMutex m_resumeStopMutex;
	    QWaitCondition m_resumeStopWc;

	    int m_reducedSpeedDelay ; // ms
	    int m_snailPaceDelay ;   // ms
	    ExtractingSpeed m_speed;
	    //friend class WebExtractorCategorySchedulerImpl;
    };
}

#endif
