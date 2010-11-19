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
#include "resourceanalyzerfactory.h"
#include "parameters.h"
#include <Nepomuk/Query/Query>


namespace Nepomuk
{
    namespace WE = WebExtractor;

    /*! \brief Class that analyze all resource in category
     *
     * This class is slowly analyzing all resource that match
     * category query. It can be stopped,suspende, resumed  in any moment
     */
    class WebExtractorCategoryScheduler : public QThread
    {
            Q_OBJECT;

        public:
            /*! \brief Construct new scheduler.
             *
             * This is constructor.
             * \param category_query This is query for category. All resources
             * that match this query will be analyzed during work of this scheduler
             * \param examinedInfoInNepomuk If true, then information about DataPP
             * is stored inside Nepomuk. Then checks for examined DataPP info
             * are build inside category_query and additional check can be skiped
             * \param parent No comments - just simple QObject parent
             * \param params This is settings for all analyzing stuff
             * \param maxResSimult Maximum number of resources launched simultaneously
             */
            WebExtractorCategoryScheduler(
                const Nepomuk::Query::Query & category_query,
                QObject * parent,
                const WebExtractor::ExtractParameters& params,
                bool optimizeForNepomuk = false,
                int maxResSimult = 5
            );
            ~WebExtractorCategoryScheduler();

            bool isSuspended() const;
            bool isExtracting() const;
            int maxResSimult() const {
                return m_maxResSimult;
            }

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

            ExtractingSpeed currentSpeed() const {
                return m_speed;
            }

            /*! \brief Return true if category is ready for launch
             * This method is not thread-safe. Do not call it when
             * category is already running
             */
            bool isReady() ;


            /*! \brief This method clear all data that are left after query finish it work.
             * This is mostly necessary if query was interupted with stop().
             * Calling this method while category is running is dangerous
             */
            void clear();

        public Q_SLOTS:
            void suspend();
            void resume();
            void stop();
            void restart();

            void setExtractingSpeed(ExtractingSpeed speed);

            /**
             * A convinience slot which calls setExtractingSpeed
             * with either FullSpeed or ReducedSpeed, based on the
             * value of \p reduced.
             */
            void setReducedExtractingSpeed(bool reduced = false);

            void setSuspended(bool);
            void setMaxResSimult(int new_mrsm);

            /*! \brief Called when resource processing fineshed
             */
            void resourceProcessed();
            /*! \brief Called when resource processing aborted
             */
            void resourceProcessingAborted();

        private Q_SLOTS:
            /*! This method start analyzing of givenr resourceUri
             */
            void launch(const QUrl & resourceUri);
            // Debug only
            //void mseg();
        Q_SIGNALS:
            void extractingStarted();
            void extractingStopped();
            void extractingFolder(const QString&);
            void extractingSuspended(bool suspended);
            //void launchPls(QUrl);

        private:
            void run();
            // emits indexingStarted or indexingStopped based on parameter. Makes sure
            // no signal is emitted twice
            void setExtractingStarted(bool started);

            // Make a delay and return. Suspending is implemented with this method.
            // Return true if thread is not stopped
            bool waitForContinue(bool disableDelay = false);


            // Launch next resource from queue. If queue is empty,
            // then makes an attempt to fill it. It queue is still empty,
            // return false. Otherwise start analyzing of  next resource
            // via launch(QUrl) and return true.
            bool launchNext();

            // This method will launch next resource (via launchNext() )
            // if the system has not been stoped ( via stop()  or destructor).
            // When system is stopped or all resource in category has been analyzed,
            // it will call quit() and the thread will finish.
            void launchOrFinish();

            // This method is used once to start analyzing of initial portion of
            // resources. If there is no resouce to start, will return false.
            // Otherwise return true.
            bool startLaunch();

            // Add new urls to cache/queue
            void cacheUrls();

            // Check that given (in constructor) query is valid.
            bool checkQuery();

            // clear and finish
            void __finish();


            // This variable has same meaning as m_stopped
            // but it is internal and can not be modificated
            // by another thread
            //bool m_finishing;

            // Category query
            Nepomuk::Query::Query m_query;

            bool m_optimizeNepomuk;

            // Number of Resources that has been started, but has not finished
            int m_respWaits;

            // Maximum number of resources that can be processed simultaneously
            int m_maxResSimult;

            // Number of currently processed resources( instances of ResourceAnalyzer
            // currently running)
            int m_currentResProc;
            int m_cacheSize;
            //Soprano::QueryResultIterator it;

            WebExtractor::ResourceAnalyzerFactory * m_factory;

            QQueue< WebExtractor::ResourceAnalyzer* > m_availableAnalyzers;

            // Queue of resource url to analyze
            mutable QQueue<QUrl> m_urlQueue;

            // Parameters necessary for analyzers
            WebExtractor::ExtractParameters m_extractParams;

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
