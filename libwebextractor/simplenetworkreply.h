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

#ifndef __simple_network_datareply_h_
#define __simple_network_datareply_h_

#include "executivereply.h"
#include "webextractor_export.h"
#include <Nepomuk/Resource>

namespace Nepomuk {
    namespace WebExtractor {
	class SimpleNetworkExecutive;
	class DecisionFactory;
	class WEBEXTRACTOR_EXPORT SimpleNetworkExecutiveReply : public ExecutiveReply
	{
	    Q_OBJECT;
	    public:
		SimpleNetworkExecutiveReply(SimpleNetworkExecutive * parent , const DecisionFactory * factory, const Nepomuk::Resource & res);
		/*! \brief main function for Reply. All work should be done here
		 * This methon must return 0 when all work is finished,
		 * and non-zero otherwise. The method will be called untill it
		 * return 0.
		 */
		virtual void start() = 0;
	};
    }
}
#endif


