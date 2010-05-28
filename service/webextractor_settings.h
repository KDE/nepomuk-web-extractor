/*
   Copyright (C) 2010 by Serebriyskiy Artem <v.for.vandal@gmail.com>

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
#ifndef _NEPOMUK_WEBEXTRCT_SETTINGS_H_
#define _NEPOMUK_WEBEXTRCT_SETTINGS_H_
#include <webextractor/parameters.h>
#include <QtCore/QHash>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QtDebug>
#include "webextractor_config.h"
namespace Nepomuk {
    class WebExtractorSettings : public WebExtractorConfig
    {
	public:
	    WebExtractorSettings();
	    ~WebExtractorSettings();
	    WebExtractor::ExtractParametersPtr  extractParameters(const QString categoryName) const;
	    int maxPluginsLaunched( const QString & categoryName);
	    int maxResSimult( const QString & categoryName);

	    int interval( const QString & categoryName);
	    QString query( const QString categoryName);
	    QString queryPrefix( const QString categoryName);
	    void update();
	    void clear();
	    friend QDebug operator<<( QDebug dbg,  const WebExtractorSettings & conf);
	protected:
	    QHash< QString, WebExtractor::ExtractParametersPtr > m_parameters; 
	    QHash< QString, Nepomuk::WebExtractor::DataPP*> m_datapp;
    };
	QDebug operator<<( QDebug dbg,  const WebExtractorSettings & conf);
}



#endif
