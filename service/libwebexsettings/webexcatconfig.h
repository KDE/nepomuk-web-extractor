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
#ifndef __webexcatconfig_h_
#define __webexcatconfig_h_

#include "webexcategory.h"
#include <QMap>
#include <QSet>
#include "datappdescr.h"

namespace Nepomuk {
    class WebExCategoryConfig : public WebExCategory
    {
	Q_OBJECT;
	public:
	    WebExCategoryConfig(const QString & name);
	    WebExCategoryConfig( KSharedConfigPtr ptr);
	    void clearPluginList();
	    const QMap<QString,DataPPDescr> & plugins() const;
	    const QSet<QString> & pluginsNames() const;
	    void addPlugin( const QString & pluginName, double rank, double coff, bool trusted = true);
	    void addPlugin( const DataPPDescr & pluginDescr);
	    //static const QString & categoryConfigDir();
	    friend QDebug operator<<( QDebug dbg,  const WebExCategoryConfig & cat);
	private:
	    void init();
	    void readPluginsList();
	    static QString path;
	    QMap<QString,DataPPDescr> m_plugins;
	    QSet<QString> m_pluginsNames;
	    QString m_name;
    };
    QDebug operator<<( QDebug dbg,  const WebExCategoryConfig & cat);

}

#endif
