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

#ifndef __simple_datapp_request_h_
#define __simple_datapp_request_h_

#include <QNetworkReply>
#include <QObject>
#include <webextractor/webextractor_export.h>


namespace Nepomuk {
    namespace WebExtractor { 
	class SimpleDataPP;
	/*! \brief Class representing one reques to servegr, guarded by SimpleDataPP 
	 * Class usage: Create instance, connect to it signals, send it to SimpleDataPP
	 * Attention: To prevent problems in multithreaded applications connect to signals
	 * before sending to SimleDataPP
	 */
	class WEBEXTRACTOR_EXPORT SimpleDataPPRequest : public QObject
	{
	    Q_OBJECT;
	    public:
		SimpleDataPPRequest(const QString & requestUrlPart, QObject * parent = 0);
		~SimpleDataPPRequest();
		QNetworkReply * reply() const;
		QString requestUrlPart() const;
		/*! \brief This function delete accosiated QNetworkReply( if any )
		 * This function will be called automaticaly when request is sent to
		 * SimpleDataPP. But you can call it yourself.
		 */
		void clear();
		friend class SimpleDataPP;
	    Q_SIGNALS:	
		void finished();
		void error();
	    private:
		void setReply(QNetworkReply * );
		QNetworkReply * m_reply;
		QString m_requestUrlPart;
	};
    }
}
#endif

