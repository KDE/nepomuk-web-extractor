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
#include <Soprano/Backend>
#include "webextractor_export.h"
#include "datappwrapper.h"
#include "datappwrapper.h"
#include "global.h"
#include "decisionlist.h"

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

                /*! \brief Mark all finished DataPP as examined for this resource
                 * After executing this function  will mark finished DataPP as
                 * examined for this resource
                 */
                //void markExaminedDataPP() const;
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

                    /*! \brief There is no necessary model
                     * There is no model installed in the system that supports
                     * in-memory models
                     */
                    NoModelAvailable,

                    /*! Unknown resource.
                     * This is usually a bug in system
                     */
                    UnknownError
                };

                enum AnalyzingPolitics {
                    /*! \brief Extract metadata untill no good Decision can be produced
                     * When this politics is selected, ResourceAnalyze will automatically
                     * apply the best decision after extracting session is finished and
                     * start new session. The process will stop if no more decisions
                     * can be generated or when none of decisions generated can not be
                     * applied automatically.
                                 * There is one more benefit  in this mode - if in previous iteration
                                 * some DataPP was examined, then in this iteration it will be exmained
                                 * too. The problem that it solves is: what if changes, introduced
                                 * with applied (at the end of the iteration) Decision has not changed
                                 * nao:lastModified date of the resource. For example Decision change
                                 * the information about author of the song. Then we have a situation
                                 * where information is actually changed, but there is no records
                                 * about it. In SingleStep politics all DataPP will be marked as
                                 * examined - because to detect non-examined DataPP system compares
                                 * 2 dates - last modification date of the resource and extraction
                                 * date of the DataPP. In Iterative politics all DataPP from previous
                                 * iteration  will be executed in current iteration.
                     * Choosing this politics help decrease request to Nepomuk because
                     * auxilary data(DataPP name, version, time of extracting) will
                     * be written only once - at the end.
                     */
                    Iterative,
                    /*! \brief Stop after each analyzing step
                     * When this politics is selected, analyzing process will stop
                     * after the extracting session finished (All selected DataPP
                     * generate Decisions or report an error). After that necessary
                     * metadata will be added to Nepomuk - list of DataPP that succeeded,
                     * their version, time extracting finished.
                     * After analyzing process finished, you can call apply() to apply
                     * the best decision or do what ever you want.
                     * Because applying Decision will change modification date of
                     * target resource, you can call analyze() after applying any of
                     * the Decision and new portion of Decisions will be generated.
                     * This is equal to Iterative politics.
                     */
                    SingleStep
                };

                /*! \brief Return list of generated decisions
                 */
                DecisionList decisions() const;

                /*! \brief Apply any Decision with rank > a_crit or store Decision for the user discrition
                 *
                 * This function can be called only once. Next calls of this method
                         * will do nothing. It is not very usefull method. In all cases, except
                         * the simplest one you should take DecisionList ( via decisions() )
                         * and work with it.
                         * \return True if applying the best decision was successfull
                 */
                bool apply();

                /*! Remove list of decisions.
                 *
                 * Call this method if you want to delete collected information.
                         * Calling this method when analyzing process is running will result
                         * in ignoring this function. Call abort() first, then clear()
                         * \return True if clearing was succesfull, false otherwise. False
                         * means that analyzer was running
                 */
                bool clear();

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
                         * No information about examined DataPP  will be written back
                         * to storage.
                         * The already generated Decisions still can be retrived
                         * with decisions() method.
                 */
                void abort();

                /*!  \brief Destructor
                 * TODO May be it is necessary to remember parent factory inside
                 * ResourceAnalyzer and call factory->deleteAnalyzer in destructor
                 * or something like this. Or may be it is a bad idea
                 */
                ~ResourceAnalyzer();

                /*! \brief Return the current analyzing politics
                         * \return Analyzing politics of the current analyzing process
                 */
                AnalyzingPolitics analyzingPolitics() const;

                /*! \brief Set new analyzing politics
                         * Changing politics while analyzer is running cause ignoring
                         * this function. Call abort() first.
                         * \sa analyzingPolitics
                 */
                void setAnalyzingPolitics(AnalyzingPolitics politics);


                //QMap<QString, QString> finishedPlugins

                /*! \brief Interrupter is function pointer that will be called between iterations
                 * This object is necessary only in Iterative mode. It will be called
                 * before each new iteration, startig from second one.
                 * This is only for debugging proposes.
                 */
                //void setDebugInterrupter( void (*newInterrupter)() );



                /*! \brief Return list of emamined DataPP for given resource
                 * Be aware that calling this function while given resource is being
                 * analyzed may return obsolete results
                 */
                //static QMap<QString,QString > examinedDataPPInfo(const Nepomuk::Resource & res);
                //static void setExaminedDataPPInfo( Nepomuk::Resource & res, const QMap< QString, QString > & info);
                /*! \brief Mark given DataPP ( name, version) as finished for given resource
                 * Resource must exists
                 */
                //static void setExaminedDataPPInfo( Nepomuk::Resource & res, const  QString & name,const QString & info);

                /*! \brief Clear obsolete DataPP info for given resource
                 */
                //static void clearObsoleteExaminedDataPPInfo( Nepomuk::Resource & res, int expirationInterval = 0);
                //static void clearExaminedDataPPInfo( Nepomuk::Resource & res );

            Q_SIGNALS:
                /*! \brief This signal is emmited when analyzing is finished
                 * In SingleStep mode it is after iteration is finished.
                 * In Iterative mode, analyzing is finished when there is no more
                 * Decisions with automatically applicable rank generated.
                 */
                void analyzingFinished();

                /*! \brief This signal is emited before next iteration start
                 * This signal will be emited only in Iterative mode. It will
                 * be emited before any Iteration, starting from second one
                 * The signal is emmited before the best decision is applied.
                 * Do not modificate analyzer or target resource from the slots,
                 * connected to this signal. This signal should be used only for
                 * debugging or introspection
                 */
                void nextIteration();

                /*! \brief indicates an error during analyzer
                         * This signal will be emited when any fatal error occure.
                         * Fatal errors are errors that make impossible further analyzing
                         * process. For example resource doesn't exist.
                 * The analyzingFinished signal will folow this one
                 */
                void error(AnalyzingError error);

                // Implementation API
            private:
                ResourceAnalyzer(
                    const DataPPKeeper & ,
                    DecisionFactory * fact, // Take ownership
                    MergePolitics mergePolitics,
                    LaunchPolitics launchPolitics,
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
                /*! \brief This slot is called when DataPP has finished without error
                 */
                void pluginFinished();
                /*! \brief This slot is callde when DataPP has finished with error
                      */
                void pluginError();

                //  This is service slot. It tries to launch next pack of the DataPP.
                //  It return true if any DataPP was launched and false - otherwise.
                //  The only reason for returning false is that there is no more
                //  DataPP to launch.
                bool launchNext();

                // This is service slot. It is just a shorthand for
                // if ( canLaunch )
                //    launch
                // else
                //    finish analyzig.
                // This method was provide to increase readability of the program
                // logic. Unfortunately, looks like it serves the oposite goal
                void launchOrFinish();

                // Behaviour depends from selected politics
                // emit analyzingFinished signal
                void analyzingSessionFinished();

                // This method will emit error signal and then analyzingFinished signal
                // error must be set BEFORE calling this slot.
                void exitWithError();



                // Perform actual analyzing
                void doAnalyze();
            private:
                // This method is called when all launched plugins returns their data.
                // In this method this data are handled. Then if there are more plugins
                // in the queue, these plugins are launched.
                void pluginPackFinished();


                // This method set m_error variable to code, then
                // emit error signal and then analyzingFinished signal.
                // This method should be used only when analyzer is not
                // running. To exit from running analyzer call
                // abortWithError()
                void exitWithError(AnalyzingError code);

                // This method will abort execution of analyzer,
                // set error to given one and exit. Use this method
                // if analyzer was launched, but encounter a serious
                // error and can not work anymore
                // You should not perform any actions after you call
                // this method and return directly back to Qt main loop
                void abortWithError(AnalyzingError code);

                // This method will return DataPPReply * pointer retrived from
                // QObject::sender(). If the sender is not DataPPReply, 0 will
                // be returned. Is reply is from unregistered DataPP, then
                // 0 will be returned.
                //  The reply  will be deleted from the m_replies variable.
                //  If reply is registered, then counter of replies will be
                //  decreased.
                // This method is necessary only to avoid code duplicates in
                // pluginFinished() and pluginError()
                // Again, ATTENTION: Reply will be deleted form the m_replies
                // For unregistered DataPPReply, deleteLater will also be called,
                DataPPReply * acceptReply();

            public:
                friend class ResourceAnalyzerFactory;
                class Private;
                //QSharedDataPointer<Private> d;
                Private* d;
        };
    }
}
#endif
