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

#ifndef __simple_datapp_reply_h_
#define __simple_datapp_reply_h_

#include <webextractor/datappreply.h>
#include <webextractor/decision.h>
#include <webextractor/decisionlist.h>
#include <webextractor/webextractor_export.h>
#include <Nepomuk/Resource>

namespace Nepomuk
{
    namespace WebExtractor
    {
        class SimpleDataPPReplyPrivate;
        class WEBEXTRACTOR_EXPORT SimpleDataPPReply  : public DataPPReply
        {
                Q_OBJECT;
            public:
                SimpleDataPPReply(DataPP * parent, const  DecisionFactory * factory, const Nepomuk::Resource & res);
                Decision newDecision();
                DecisionList newDecisionList();
                virtual DecisionList decisions() const;
                const Nepomuk::Resource & resource();
                DecisionList & mainDecisionList();
                virtual ~SimpleDataPPReply() {
                    ;
                }
            protected:
                SimpleDataPPReply(DataPP * parent, const DecisionFactory * factory, const Nepomuk::Resource & res , SimpleDataPPReplyPrivate & p);
            private:
                Q_DECLARE_PRIVATE(SimpleDataPPReply)

        };
    }
}


#endif
