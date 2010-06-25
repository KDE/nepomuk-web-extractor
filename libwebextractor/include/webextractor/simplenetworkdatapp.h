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

#ifndef __simplenetworkdatapp_h_
#define __simplenetworkdatapp_h_

#include <webextractor/datapp.h>
//#include <webextractor/simplenetworkreplyfactory.h>
#include <QNetworkAccessManager>
#include <QTimer>
#include <QQueue>
#include <QTime>
#include <QMutex>
#include <webextractor/webextractor_export.h>
#include <webextractor/simplenetworkreplyfactory.h>
#include <webextractor/simplenetworkrequest.h>
namespace Nepomuk {
    namespace WebExtractor {
	class SimpleNetworkDataPPReply;
	class WEBEXTRACTOR_EXPORT SimpleNetworkDataPP : public Nepomuk::WebExtractor::DataPP
	{
	    Q_OBJECT;
	    public:
		// SimpleNetworkDataPP will take parentship on factory
		SimpleNetworkDataPP(const QString & dataPPVersion, const QString & endpointUrl, SimpleReplyFactory * factory );
		// Safe therad. Called from another thread - now from thread where
		// Datapp leaves
		virtual DataPPReply * requestDecisions(const DecisionFactory * factory, const Nepomuk::Resource & res);
		//virtual DataPPReply * requestDecisions(const DecisionFactory * factory, const Nepomuk::Resource & res,QObject * target, const char * finishedSlot, const char * errorSlot);
		// This method can be redefined in sucessors. Default implementation will use
		// given factory (if any).
		virtual Nepomuk::WebExtractor::SimpleNetworkDataPPReply * newReply(const Nepomuk::WebExtractor::DecisionFactory * factory, const Nepomuk::Resource & res);
		virtual ~SimpleNetworkDataPP();


		void get( SimpleNetworkDataPPRequest * request );
	    protected Q_SLOTS:
		//void launchNext();
		void timeout();
		void startTimer();
	    protected:
		void enqueue( SimpleNetworkDataPPRequest * request );
		// Return 0 if queue was empty. Actualy it should never happens.
		int runNext();
		void getRequest( SimpleNetworkDataPPRequest * request);
		// void enqueue(SimpleNetworkDataPPReply*);
		// This is constuctor for class that inherit this one and doesn't require factory
		SimpleNetworkDataPP(const QString & dataPPVersion, const QString & serverName, const QString & endpointUrl);
		QNetworkAccessManager * m_net;
		QTimer * m_reqTimer;
		bool m_timeout;
		QMutex m_timeoutMutex;
		QMutex m_queueMutex;
		QQueue<SimpleNetworkDataPPRequest*> m_queue;
		SimpleReplyFactory * m_factory;
		QTime m_last_request_time;
		int m_interval;
		QString m_endpointUrl;
		unsigned int m_skipped;
		unsigned int m_maxSkipped;
	    private:
		void __init();
	};
    }
}

#endif
