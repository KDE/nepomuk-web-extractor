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


#ifndef __webexsettings_global_h_
#define __webexsettings_global_h_

#include "webexsettings_export.h"
#include "webextractor_plugin.h"
#include "webextractor_kcm.h"
#include <QtCore/QHash>
#include <KService>

namespace Nepomuk {
    class WEBEXSETTINGS_EXPORT GlobalSettings
    {
	public:
	    static int pluginCount();
	// FIXME Make all members private and open them to some config classes:
	// DataPP , may be category config
	public:
	    /*! \brief Return plugin for given DppExecutive
	     */
	    static WebExtractorPlugin * plugin(const QString & name );
	    /*! \brief Return KCM for given plugin
	     * \param pluginName It is name of the plugin. <b>Not name of the Executive </b>
	     */
	    static WebExtractorPluginKCM::Ptr kcm( const QString & pluginName, bool forceNew = false );
	    // Temporaly desabled as unused
	    static QString pluginServiceType();
	    static QString pluginQueryByName(const QString & name);
	    static QString kcmQueryByName(const QString & name);
        static KService::List availablePlugins();
	private:
	    static QString pluginQueryTemplate();
	    static QHash< QString, WebExtractorPlugin*> & m_plugins();
	    static QHash< QString, WebExtractorPluginKCM::Ptr>& m_kcms();
		

    };
}
#endif
