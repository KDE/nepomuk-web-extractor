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

#ifndef _NEPOMUK_WEBEXTRCT_DATA_PP_REPL_H_
#define _NEPOMUK_WEBEXTRCT_DATA_PP_REPL_H_

#include <QtCore/QObject>
#include <QtCore/QString>
#include <webextractor/decisionlist.h>
#include <webextractor/webextractor_export.h>

namespace Nepomuk {
    namespace WebExtractor {
	class DataPP;
	class WEBEXTRACTOR_EXPORT DataPPReply : public QObject
	{
	    Q_OBJECT;
	    public:
		DataPPReply(DataPP *, DecisionFactory * /*const QString &, const QString &*/);
		virtual const DecisionList & decisions() const = 0;
		virtual ~DataPPReply();
		virtual const QString & pluginName() const; 
	    public Q_SLOTS:
		/*! \brief Abort execution 
		 */
		virtual void abort() = 0;
	    	virtual bool isValid() = 0;
		//const QString & pluginVersion() const; 
	    Q_SIGNALS:
		void finished();
		void error();
	    private:
		void setPluginName(const QString & pluginName);
		class Private;
		Private * d;
	};
    }
}

#endif

