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

#ifndef _NEPOMUK_WEBEXTRCT_DEBUG_DATA_PP_H_
#define _NEPOMUK_WEBEXTRCT_DEBUG_DATA_PP_H_

#include "datapp.h"
#include "webextractor_export.h"
#include <QtCore/QTimer>

namespace Nepomuk
{
    namespace WebExtractor
    {
        /*! \brief MUST be reentrant and thread safe
         */
        class WEBEXTRACTOR_EXPORT DebugDataPP: public DataPP
        {
            public:
                DebugDataPP();
                virtual DataPPReply * requestDecisions(const DecisionFactory * factory, const Nepomuk::Resource & res);
                static QString  name();
                static int version();
        };

        class WEBEXTRACTOR_EXPORT DebugDataPPReply : public DataPPReply
        {
                Q_OBJECT;
            public:
                DebugDataPPReply(DebugDataPP*, const DecisionFactory *);
                virtual DecisionList decisions() const {
                    return m_decisions;
                }
                virtual ~DebugDataPPReply();
            public Q_SLOTS:
                /*! \brief Abort execution
                 */
                virtual void abort() ;
                virtual bool isValid() const ;
                /*
                Q_SIGNALS:
                void finished();
                void error();
                */
            private Q_SLOTS:
                void ready();
            private:
                DecisionList m_decisions;
                //const DecisionFactory * m_fact;
                QTimer * m_timer;
        };
    }
}
#endif

