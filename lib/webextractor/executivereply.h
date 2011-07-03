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
        class WEBEXTRACTOR_EXPORT ExecutiveReply : public QObject
        {
                Q_OBJECT;
                Q_ENUMS(Error);
            public:
                ExecutiveReply(Executive *);
                virtual Decision::DecisionList decisions() const = 0;
                virtual ~ExecutiveReply();
                virtual QString pluginName() const;
                Executive * parentExecutive() const;
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
                    TimeExceeded

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

