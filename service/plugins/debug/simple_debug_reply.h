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

#ifndef __simple_debug_reply_h_
#define __simple_debug_reply_h_

#include <webextractor/simple_reply.h>
namespace Nepomuk {
    class SimpleDebugReply : public Nepomuk::WebExtractor::SimpleDataPPReply
    {
	Q_OBJECT;
	public:
	    SimpleDebugReply(Nepomuk::WebExtractor::SimpleDataPP * parent , const WebExtractor::DecisionFactory * factory, const Nepomuk::Resource & res);
	    virtual void start();
	    void step();
	    virtual bool isValid() const { return true; }
	    virtual const WebExtractor::DecisionList & decisions() const { return m_decisions; } 
	    virtual void abort() {;}
	protected:
	    int m_state;
	    WebExtractor::DecisionList m_decisions;
	protected Q_SLOTS:
	    void requestFinished();
	    void requestError();

    };
}
#endif


