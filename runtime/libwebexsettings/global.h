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


#ifndef __webexsettings_global_h_
#define __webexsettings_global_h_

#include "webextractor_plugin.h"
#include <QHash>

namespace Nepomuk {
    class GlobalSettings
    {
	public:
	    static int pluginCount();
	// FIXME Make all members private and open them to some config classes:
	// DataPPConfig , may be category config
	public:
	    static WebExtractorPlugin * plugin(const QString & name );
	    // Temporaly desabled as unused
	    //static KServicePtr pluginService( const QString & name );
	    static QString pluginServiceType();
	    static QString pluginQueryByName(const QString & name);
	private:
	    static QString pluginQueryTemplate();
	    static QHash< QString, WebExtractorPlugin*> & m_plugins();
		

    };
}
#endif