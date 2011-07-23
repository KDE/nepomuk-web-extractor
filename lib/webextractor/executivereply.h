/*
   Copyright (C) 2010 by Serebriyskiy Artem <v.for.vandal at gmail.com>

   This library is free software; you can redistribute it and/or modify
   it under the terms of the GNU Library General Public License as published by
   the Free Software Foundation; either version 3, or (at your option)
   any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#ifndef _NEPOMUK_WEBEXTRCT_EXECUTIVE_REPL_H_
#define _NEPOMUK_WEBEXTRCT_EXECUTIVE_REPL_H_

#include <QtCore/QObject>
#include <QtCore/QString>
#include <decision/decisionlist.h>
#include "webextractor_export.h"

namespace Nepomuk
{
    namespace WebExtractor
    {
        class Executive;
        class ExecutiveReplyPrivate;
        /*! \brief This class is response for Decisions request send to Executive.
         * When System asks Executive for Decisions, it should return subclass of this
         * class. So, when you are writing your plugin, you should subclass ExecutiveReply or
         * SimpleExecutiveReply and implement necessary functionality.
         * ExecutiveReply provides some methods that are necessary for the system - like 
         * Executive::parentExecutive() const. You should implement at least all abstract methods.
         * But, before you start subclassing ExecutiveReply, consider using SimpleExecutiveReply instead-
         * it provieds convinient wrappers over some signals and functions that will prevent 
         * some common pitfalls.
         * <b> You should never reuse the same Reply instance - always create new one or you 
         * will never understand why your plugin doesn't work yet doesn't report an error
         * </b>
         *
         * \section general_algo  General algorithm
         * What is the main algorithm of any ExecutiveReply?
         * There are no limitations on design of your subclass of ExecutiveReply. It can be complex class that 
         * do all extraction, or a simple proxy class that only reports when extraction is finished. When your extraction
         * finished, you must emit either signal ExecutiveReply::finished() or signal ExecutiveReply::error(ExecutiveReply::Error). Do not emit this signals directly, 
         * use emitError(), emitError(ExecutiveReply::Error) and emitFinished() signals.
         * This signals <b>must not</b> be emited from constructor, because this way they will be lost and your Reply 
         * will be killed by timeout. If you emit an ExecutiveReply::error(ExecutiveReply::Error) signal, you should also 
         * set this error using   ExecutiveReply::setError(ExecutiveReply::Error). It will help later when debugging your plugin.
         * 
         * \section subclassing Subclassing
         * When subclassing ExecutiveReply you should provide implementation for the following methods:
         * - ExecutiveReply::decisions() const
         * - ExecutiveReply::abort()
         * - ExecutiveReply::isValid() const
         * Let see all of them in more details:
         * - ExecutiveReply::decisions() is kinda the main method of ExecutiveReply. This method should return a DecisionList
         *   containing all generated Decisions. (  You can create DecisionList <b>only</b> using Decision::DecisionFactory that
         *   is given to your Executive instance in 
         *   Executive::requestDecisions(const Decision::DecisionFactory *, const Nepomuk::Resource &) ). When implementing
         *   this method, don't do any hard stuff inside. All extracting work must be finished to the point when this method 
         *   would be called.
         * - ExecutiveReply::abort(). This method should abort your execution. Abort all non-finished network requests, all 
         *   lanuched programs etc
         * - ExecutiveReply::isValid(). Extra method. Not used right now, return true here
         *
         * \section timeout Timeouts
         * ExecutiveReply ( and thus SimpleExecutiveReply as it derived class ) offer to
         * the plugins subsystem for timeouts. ( Well, offer is not really correct here - 
         * you may ignore timeout subsystem, but timeout subsytem will ignore your 
         * ignorance and kill your plugin anyway after pre-defined limit of time ).
         *
         * Timeout is necessary when plugin hangs. It may be caused by several reasons -
         * the remote server doesn't send a reply, network suddenly fall down, the
         * launched application(QProcess) hungs etc. Anyway, we can not afford that 
         * such situation leads to stopping entire system. Because of this there is 
         * a special timer that is started in the ExecutiveReply construction and 
         * constantly countdown. When it reaches zero, it - by default - abort an
         * plugin execution with ExecutiveReply::abort() method, set error to
         * TimeExceeded and emit error signal. Yes, you have some influence on the
         * behavior of the timer:
         * - setTimeout(int) - this function will stop timer, set new timeout and 
         *   restart timer. You may use this function right in your constructor 
         *   or in aboutToTimeOut() function
         * - aboutToTimeOut() - this function is called when timer countdown to zero,
         *   but \e before plugin is actually aborted. In this function you may
         *   restart timer.
         * - You may restart timer in any your code \e before you send finished() or 
         *   error(ExecutiveReply::Error) signals. Restarting timer afterwards will
         *   result in some odd things and probably fail the system.
         *
         * General guidlines are:
         * - Try to approximate necessary amount of time right in the constructor and
         *   set timer. This is possible for e.g. plugins working with local images.
         *   Image size * some coefficient is a good approximation start
         * - If your plugin is working with network, you may wish to set timeout 
         *   before any network request you send. Set the timeout to some value that
         *   indicates maximum time before network considered failed.
         *
         */ 
        class WEBEXTRACTOR_EXPORT ExecutiveReply : public QObject
        {
                Q_OBJECT;
                Q_ENUMS(Error);
            public:
                /*! \brief Constructor.
                 * You should pass an Executive instance here. This instance will
                 * be used in several default methods implementation and is necessary
                 * for system to operates. You may retrieve you parent from subclassed
                 * code using ExecutiveReply::parentExecutive() function
                 */
                ExecutiveReply(Executive *);
                /*! \brief Return all Decisions extracted by you.
                 * This method is guaranteed to be called only after you ExecutiveReply
                 * report finishing
                 */
                virtual Decision::DecisionList decisions() const = 0;
                virtual ~ExecutiveReply();
                /*! \brief Return name of the plugin.
                 * Default implementation use parent Executive passed in the constructor.
                 * No real reason to reimplement this function.
                 */
                virtual QString pluginName() const;

                /*! \brief This function return an Executive passed in the constructor
                 * You should not do anything bad with this Executive ( like deleting it)
                 * or system will fail
                 */
                Executive * parentExecutive() const;

                /*! \brief These are various error codes that Reply may return.
                 */
                enum Error {
                    /*! Everything is ok
                     */
                    NoError,
                    /*! Resource has incorrect type.
                     * For example Contact as input for Executive that works with files
                     */
                    ResourceTypeIncorrect,

                    /*! \brief Resource doesn't match some internal  conditions
                     * Some Executive put some restrictions on the input resource. If the input resource
                     * doesn't satisfy these conditions, Executive may exit with this error.
                     * This error should not be used when Resource has incorrect type - use ResourceTypeIncorrect
                     * instead. Also this error shouldn't be used when resource doesn't contain enough information
                     * to extract anything - this is valid situation. Set error to NoError and exit silently.
                     */
                    ResourceInternalConditionsFailed,

                    /*! \brief This error is set if reply has exceeded it's time
                     */
                    TimeExceeded,

                    /*! \brief This error is set if datasource of plugin was unavailable
                     * E.g. remote server doesn't respond to requests or necessary 
                     * application doesn't found
                     */
                    DataSourceUnavailable,

                    /*! \brief If something goes wrong inside plugin, then this error will be set
                     * E.g. some necessary application suddenly failed, or remote server
                     * report an error and so on
                     */
                    DataSourceError,


                    /*! \brief This is the last error in predefined list. 
                     * Use it or it+some_number to report errors that are not covered
                     * by constans above.
                     */
                    UserDefinedError
                };

                /*! \brief Return the time interaval
                 */
                int timeout() const;
                /*! \brief Icrease the time allowed for execution
                 */
                void setTimeout(int newValue);
            public Q_SLOTS:
                /*! \brief Abort execution
                 * Calling abort <b>must</b> prevent finished() and error() signals from comming
                 */
                virtual void abort() = 0;
            public:
                virtual void aboutToTimeOut();
                virtual bool isValid() const = 0;
                ExecutiveReply::Error error() const;
                void setError(ExecutiveReply::Error errorCode);
                //const QString & pluginVersion() const;
            Q_SIGNALS:
                /*! This signal is emited after reply is finished and if no error occured.
                 */
                void finished();
                /*! This signal is emited if any error occured.
                 * The finished signal MUST NOT be called after it
                 */
                void error(ExecutiveReply::Error errorCode);
            protected:
                ExecutiveReply(ExecutiveReplyPrivate & p, Executive*);
                ExecutiveReplyPrivate * d_ptr;
                /*! \brief Emit finished() signal and set apropriate internal state
                 * This method checks that final signal ( finished() or 
                 * error(ExecutiveReply::Error) was not emited before and if not - 
                 * emit finished() signal.
                 * If final signal has already been emited, function print error message
                 * and may abort execution if compiled in Debug mode
                 */
                void emitFinished();

                /*! \brief Emit error(ExecutiveReply::Rerror) signal and set apropriate internal state
                 * This method checks that final signal ( finished() or 
                 * error(ExecutiveReply::Error) was not emited before and if not - 
                 * emit error(ExecutiveReply::Error) signal. The error set with 
                 * setError( ExecutiveReply::Error) is used as parameter.
                 * If final signal has already been emited, function print error message
                 * and may abort execution if compiled in Debug mode
                 */
                void emitError();

                /*! \brief Emit error(ExecutiveReply::Rerror) signal and set apropriate internal state
                 * This method checks that final signal  was not emited before and if not
                 * - emit error(ExecutiveReply::Error) signal. \p errorCode is set as 
                 * error with setError(ExecutiveReply::Error) method and passed
                 * as parameter to signal.
                 * If final signal has already been emited, function print error message
                 * and may abort execution if compiled in Debug mode
                 */
                void emitError(ExecutiveReply::Error errorCode);
            private:
                //void setPluginName(const QString & pluginName);
                Q_DECLARE_PRIVATE(ExecutiveReply)

                // This method perform commont initialization for all constructors
                void init(Executive * parent);
            private Q_SLOTS:
                // This slot is called when time is out. In this slot
                // the virtual function aboutToTimeOut is called. If in this
                // function timer is not restarted, then abort() is called.
                void finishByTimeout();
        };
    }
}

#endif

