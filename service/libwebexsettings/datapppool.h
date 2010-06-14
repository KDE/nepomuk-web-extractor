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

#ifndef _webexsettings_datapppool_h_
#define _webexsettings_datapppool_h_

#include "datappconfig.h"
#include "webexsettings_export.h"
#include <QStringList> 
#include <QSet> 

namespace Nepomuk {
class WEBEXSETTINGS_EXPORT DataPPPool: public QObject
    {
	Q_OBJECT;
	public:
	    static const QStringList &  plugins();

	    // Return all datapp that belong to category
	    static const QSet< QString > & categoryDataPPs(const QString & categoryName) ;
	    static DataPPPool * self(); 
	    //static QString  dataPPSource(const QString &); 
	    //static KSharedConfigPtr dataPPConfig(const QString & name);
	    friend QDebug operator<<( QDebug dbg,  const DataPPPool & cat);
	Q_SIGNALS:
	    void dataPPChanged();
	private:
	    DataPPPool();
	    void init();
	    // List of all available DataPP in system
	    QStringList m_plugins;
	    
	    // Hash CategoryName->List of all available datapp in system that belongs to
	    // this category(DataPP category, not WebEXtractor Categories)
	    // 
	    QHash< QString, QSet<QString> > m_categoryPlugins;

	    // Keep the source plugin for all datapp
	    QHash< QString, QString > m_dataPPSources;


	    static DataPPPool * m_self;
    };
    QDebug operator<<( QDebug dbg,  const DataPPPool & cat);
}

#endif
