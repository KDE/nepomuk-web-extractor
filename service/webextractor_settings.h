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
#ifndef _NEPOMUK_WEBEXTRCT_CONFIG_H_
#define _NEPOMUK_WEBEXTRCT_CONFIG_H_
#include "webexconfigbase.h"
#include "webexcategory.h"
#include "plugin_interface.h"
#include <QtCore/QHash>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QtDebug>
#include <webextractor/parameters.h>

namespace Nepomuk {
	class WebExtractorConfig : public WebExConfigBase
	{
	    Q_OBJECT;
	    public:
		WebExtractorConfig();
		//const QHash<QString,WebExCategory*> & categories() const { return m_categories ;}
		int maxPluginsLaunched( const QString & categoryName);
		int maxResSimult( const QString & categoryName);

		int interval( const QString & categoryName);

		QStringList categories() const;
		WebExtractor::ExtractParametersPtr  extractParameters(const QString categoryName) const;

		QString query( const QString categoryName);
		QString queryPrefix( const QString categoryName);

		//WebExtractor::DataPPKeeper & datapp( const QString categoryName);

		void update();
		void clear();
	    private:
		QHash<QString,WebExCategory*> m_categories;
		//QHash< QString, QSharedPointer<WebExtractor::ExtractParameters> > m_parameters; 
		QHash< QString, WebExtractor::ExtractParametersPtr > m_parameters; 
		QHash< QString, WebExtractorPlugin *> m_plugins;
		QHash< QString, Nepomuk::WebExtractor::DataPP*> m_datapp;
		//QHash< QString, DataPPWrapper*> m_datappwrappers;

		friend QDebug operator<<( QDebug dbg,  const WebExtractorConfig & cat);

	};
	QDebug operator<<( QDebug dbg,  const WebExCategory & cat);
	QDebug operator<<( QDebug dbg,  const WebExtractorConfig & conf);


}

#endif
