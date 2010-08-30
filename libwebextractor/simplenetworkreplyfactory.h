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

#ifndef __simple_datareply_factory_h_
#define __simple_datareply_factory_h_

#include "webextractor_export.h"
#include <Nepomuk/Resource>

namespace Nepomuk {
    namespace WebExtractor {

	class SimpleNetworkDataPPReply;
	class SimpleNetworkDataPP;
	class DecisionFactory;

	class WEBEXTRACTOR_EXPORT SimpleReplyFactory 
	{
	    public:
		virtual SimpleNetworkDataPPReply * newReply(SimpleNetworkDataPP * parent, const DecisionFactory * factory, const Nepomuk::Resource  & res) = 0;
		virtual ~SimpleReplyFactory();
	};

	template < typename T >
	class SimpleReplyFactoryTemplate : public SimpleReplyFactory
	{
	    public:
		virtual SimpleNetworkDataPPReply * newReply(SimpleNetworkDataPP * parent, const DecisionFactory * factory, const Nepomuk::Resource  & res)
		{
		    return new T(parent, factory, res);
		}
		virtual ~SimpleReplyFactoryTemplate() {;}
	};
    }
}
#endif

