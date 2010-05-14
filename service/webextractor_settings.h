/*
   Copyright (C) 2010 by Serebriyskiy Artem <v.for.vandal at kde.org>

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
#ifndef _NEPOMUK_WEBEXTRCT_CONFIG_H_
#define _NEPOMUK_WEBEXTRCT_CONFIG_H_
#include "webexconfigbase.h"
#include "webexcategory.h"
#include <QtCore/QMap>
#include <QtCore/QString>
#include <QtCore/QtDebug>

namespace Nepomuk {
    namespace WebExtractor {
	class WebExtractorConfig : public WebExConfigBase
	{
	    Q_OBJECT;
	    public:
		WebExtractorConfig();
	    private:
		QMap<QString,WebExCategory*> m_categories;

	};
	//QDebug operator<<( QDebug dbg,  const WebExCategory & cat);
	//QDebug operator<<( QDebug dbg,  const WebExtractorConfig & conf);


    }
}

#endif
