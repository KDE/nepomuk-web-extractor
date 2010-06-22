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
#include <QSharedDataPointer>
#include <Nepomuk/Resource>
#include <webextractor/webextractor_export.h>
#include <webextractor/datappwrapper.h>
#include <webextractor/datappwrapper.h>
#include <webextractor/global.h>
#include <webextractor/decisionlist.h>

namespace Nepomuk
{
    namespace WebExtractor
    {
        class ResourceAnalyzerFactory;
        /*! \brief This is the main class for analyzing resources.
         *
         * Analyzing is started with analyze() method.
         *
         * After analyzing is finished generated decisions can be retrived with
         * decisions() method.
         *
         * To apply decisionlist - apply the best decision with rank > a_crit or
         * add decisions with rank > u_crit to the storage call apply() method
         *
         * TODO Add counter for counting how often decisions become obsolete right
         * after the retriving. If too often then refuse from analyzing.
         */
        class WEBEXTRACTOR_EXPORT ResourceAnalyzer : public QObject
        {
                Q_OBJECT;
                // Public API
            public Q_SLOTS:
                /*! \brief Analyze given resource.
                 * If resource is incorrect then error() signal will be send.
                 * All signals are sent <b>after</b> the control returns to event loop.
                 */
                void analyze(Nepomuk::Resource & res);

                /*! \brief Analyze current resource
                      * If resource is incorrect then error() signal will be send.
                      * All signals are sent <b>after</b> the control returns to event loop.
                      */
                void analyze();
            public:
                enum AnalyzingError {
                    /*! \brief No error
                     */
                    NoError = 0,

                    /*! I forget :(
                     */
                    ActiveUsage = 1 ,

                    /*! Resource doesn't exist
                     */
                    UnexistingResource,

                    /*! Resource is invalid
                     */
                    InvalidResource,

                    /*! Unknown resource.
                     * This is usually a bug in system
                     */
                    UnknownError
                };

                /*! \brief Return list of generated decisions
                 */
                DecisionList decisions() const;

                /*! \brief Apply any Decision with rank > a_crit or store Decision for the user discrition
                 *
                 * This function can be called only once. Next calls of this method will do nothing
                 */
                void apply();

                /*! Remove list of decisions.
                 *
                 * Call this method if you want to delete collected information.
                         * Calling this method when analyzing process is running will result
                         * in ignoring this function. Call abort() first, then clear()
                         * TODO Make this function bool instead of void
                 */
                void clear();

                /*! \brief return error code
                 */
                AnalyzingError error() const;

                /*! \brief Set current analyzing resource.
                 * This method should not be called while ResourceAnalyzer
                 * is processing prevous resource. It will <b>always</b> abort analyzing.
                 * If res is invalid then nothing will happens ( except aborting
                 * previous analyzing, of course). If resource is valid (even if
                 * it is not exists) then all previously collected data will be cleared
                 * and target resource will be replaced with given one.
                 * The analyzing process will <b>not</b> restart even if it was running
                 * when this method was called.
                 */
                void setResource(const Nepomuk::Resource & res);

                /*! \brief Return true if ResourceAnalyzer is analyzing some resource
                 */
                bool isRunning() const;

                /*! \brief Abort analyzing. No signal will be send
                 * The method will abort analyzing but will not clear already
                 * collected data. Call clear() to do this.
                 * TODO Add overloaded method that will send error() and finished()
                 * signals after aborting
                 */
                void abort();

                /*!  \brief Destructor
                 * TODO May be it is necessary to remember parent factory inside
                 * ResourceAnalyzer and call factory->deleteAnalyzer in destructor
                 * or something like this. Or may be it is a bad idea
                 */
                ~ResourceAnalyzer();
            Q_SIGNALS:
                void analyzingFinished();
                /*! \brief indicates an error during analyzer
                 * The analyzingFinished signal will folow this one
                 */
                void error(AnalyzingError error);

                // Implementation API
            private:
                ResourceAnalyzer(
                    const DataPPKeeper & ,
                    DecisionFactory * fact, // Take ownership
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
                const ResourceAnalyzer & operator=(const ResourceAnalyzer &);
            private Q_SLOTS:
                void pluginFinished();
                //void pluginError();
                bool launchNext();
                void launchOrFinish();

                // emit analyzingFinished signal
                void emitAnalyzingFinished();

                // This method will emit error signal and then analyzingFinished signal
                // error must be set BEFORE calling this slot.
                void finishWithError();

            private:
                // emit error signal and then analyzingFinished signal.
                // Also set m_error variable to code
                void finishWithError(AnalyzingError code);
                //void clearPreviousData();
            public:
                friend class ResourceAnalyzerFactory;
                class Private;
                //QSharedDataPointer<Private> d;
                Private* d;
        };
    }
}
#endif
