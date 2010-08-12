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

#ifndef __webextractor_simple_datapp_reply_private_h_
#define __webextractor_simple_datapp_reply_private_h_

#include "datappreply_p.h"

namespace Nepomuk
{
    namespace WebExtractor
    {
        class SimpleDataPPReplyPrivate : public DataPPReplyPrivate
        {
            public:
                SimpleDataPPReplyPrivate(DecisionList list):
                    m_decisions(list) {
                    ;
                }
                const DecisionFactory * m_factory;
                Nepomuk::Resource m_res;
                DecisionList m_decisions;
                DataPPReply::DataPPReplyError m_error;
        };
    }
}


#endif