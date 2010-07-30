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

#ifndef _NEPOMUK_WEBEXTRCT_DATA_PP_REPL_H_
#define _NEPOMUK_WEBEXTRCT_DATA_PP_REPL_H_

#include <QtCore/QObject>
#include <QtCore/QString>
#include "decisionlist.h"
#include "webextractor_export.h"

namespace Nepomuk
{
    namespace WebExtractor
    {
        class DataPP;
        class DataPPReplyPrivate;
        class WEBEXTRACTOR_EXPORT DataPPReply : public QObject
        {
                Q_OBJECT;
            public:
                DataPPReply(DataPP *);
                virtual DecisionList decisions() const = 0;
                virtual ~DataPPReply();
                virtual QString pluginName() const;
                DataPP * parentDataPP() const;
                enum DataPPReplyError {
                    /*! Everything is ok
                     */
                    NoError,
                    /*! Resource has incorrect type.
                     * For example Contact as input for DataPP that works with files
                     */
                    ResourceTypeIncorrect,

                    /*! \brief Resource doesn't match some internal  conditions
                     * Some DataPP put some restrictions on the input resource. If the input resource
                     * doesn't satisfy these conditions, DataPP may exit with this error.
                     * This error should not be used when Resource has incorrect type - use ResourceTypeIncorrect
                     * instead. Also this error shouldn't be used when resource doesn't contain enough information
                     * to extract anything - this is valid situation. Set error to NoError and exit silently.
                     */
                    ResourceInternalConditionsFailed

                };
            public Q_SLOTS:
                /*! \brief Abort execution
                 * Calling abort <b>must</b> prevent finished() and error() signals from comming
                 */
                virtual void abort() = 0;
                virtual bool isValid() const = 0;
                virtual DataPPReplyError error() const = 0;
                //const QString & pluginVersion() const;
            Q_SIGNALS:
                void finished();
                void error(DataPPReply::DataPPReplyError errorCode);
            protected:
                DataPPReply(DataPPReplyPrivate & p, DataPP*);
                DataPPReplyPrivate * d_ptr;
            private:
                //void setPluginName(const QString & pluginName);
                Q_DECLARE_PRIVATE(DataPPReply)
        };
    }
}

#endif

