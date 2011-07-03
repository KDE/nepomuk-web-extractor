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

#ifndef _NEPOMUK_WEBEXTRCT_DEBUG_EXECUTIVE_H_
#define _NEPOMUK_WEBEXTRCT_DEBUG_EXECUTIVE_H_

#include "executive.h"
#include "webextractor_export.h"
#include <QtCore/QTimer>

namespace Nepomuk
{
    namespace WebExtractor
    {
        /*! \brief MUST be reentrant and thread safe
         */
        class WEBEXTRACTOR_EXPORT DebugExecutive: public Executive
        {
            public:
                DebugExecutive();
                virtual ExecutiveReply * requestDecisions(const Decision::DecisionFactory * factory, const Nepomuk::Resource & res);
                static QString  name();
                static int version();
        };

        class WEBEXTRACTOR_EXPORT DebugExecutiveReply : public ExecutiveReply
        {
                Q_OBJECT;
            public:
                DebugExecutiveReply(DebugExecutive*, const Decision::DecisionFactory *);
                virtual Decision::DecisionList decisions() const {
                    return m_decisions;
                }
                virtual ~DebugExecutiveReply();
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
                Decision::DecisionList m_decisions;
                //const DecisionFactory * m_fact;
                QTimer * m_timer;
        };
    }
}
#endif

