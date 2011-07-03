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

#ifndef __simple_datapp_reply_h_
#define __simple_datapp_reply_h_

#include "executivereply.h"
#include "webextractor_export.h"
#include <Nepomuk/Resource>
#include <decision/decision.h>
#include <decision/decisionlist.h>

namespace Nepomuk
{
    namespace Decision {
	class DecisionFactory;
	class DecisionCreator;
    }


    namespace WebExtractor
    {
        class SimpleExecutiveReplyPrivate;
        class WEBEXTRACTOR_EXPORT SimpleExecutiveReply  : public ExecutiveReply
        {
                Q_OBJECT;
            public:
                SimpleExecutiveReply(Executive * parent, const  Decision::DecisionFactory * factory, const Nepomuk::Resource & res);
                Decision::DecisionCreator newDecision();
                Decision::DecisionList newDecisionList();
                void addDecision(const Decision::Decision &);
                void addDecision(const Decision::Decision &, Decision::MergePolitics politics, double coff = 1, bool noAuto = false);
                void addDecision(const Decision::DecisionCreator &);
                void addDecision(const Decision::DecisionCreator & , Decision::MergePolitics politics, double coff = 1, bool noAuto = false);
                double approximateThreshold() const;
                virtual Decision::DecisionList decisions() const;
                const Nepomuk::Resource & resource();
                //DecisionList & mainDecisionList();
                virtual ~SimpleExecutiveReply() {
                    ;
                }
            public Q_SLOTS:
                /*! \brief Will finish Executive
                 * This function correctly handle situations when it is called from
                 * constructor of the ExecutiveReply.
                 * It will call realFinish() internaly
                 */
                void finish();
            protected Q_SLOTS:
                /*! \brief This function perform actual finishing.
                 * This function \b must be called from Qt event loop!
                 */
                void realFinish();
            protected:
                SimpleExecutiveReply(Executive * parent, const Decision::DecisionFactory * factory, const Nepomuk::Resource & res , SimpleExecutiveReplyPrivate & p);
            private:
                Q_DECLARE_PRIVATE(SimpleExecutiveReply)

        };
    }
}


#endif
