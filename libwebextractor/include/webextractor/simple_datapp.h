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

#ifndef __simple_datapp_h_
#define __simple_datapp_h_

#include <webextractor/datapp.h>
//#include <webextractor/simple_reply_factory.h>
#include <QNetworkAccessManager>
#include <QTimer>
#include <QQueue>
#include <QMutex>
#include <webextractor/webextractor_export.h>
#include <webextractor/simple_reply_factory.h>
namespace Nepomuk {
    namespace WebExtractor {
	class SimpleWEPlugin;
	class SimpleDataPPReply;
	class WEBEXTRACTOR_EXPORT SimpleDataPP : public QObject, public Nepomuk::WebExtractor::DataPP
	{
	    Q_OBJECT;
	    public:
		// SimpleDataPP will take parentship on factory
		SimpleDataPP(const QString & dataPPVersion, SimpleReplyFactory * factory );
		// Safe therad. Called from another thread - now from thread where
		// Datapp leaves
		virtual DataPPReply * requestDecisions(const DecisionFactory * factory, const Nepomuk::Resource & res);
		// This method can be redefined in sucessors. Default implementation will use
		// given factory (if any).
		virtual Nepomuk::WebExtractor::SimpleDataPPReply * newReply(const Nepomuk::WebExtractor::DecisionFactory * factory, const Nepomuk::Resource & res);
		virtual ~SimpleDataPP();

	    protected Q_SLOTS:
		//void launchNext();
		void timeout();
		void startTimer();
	    protected:
		void enqueue(SimpleDataPPReply*);
		// This is constuctor for class that inherit this one and doesn't require factory
		SimpleDataPP(const QString & dataPPVersion, const QString & serverName);
		QNetworkAccessManager * m_net;
		QTimer * m_reqTimer;
		bool m_timeout;
		QMutex m_timeoutMutex;
		QMutex m_queueMutex;
		QQueue<SimpleDataPPReply*> m_queue;
		SimpleReplyFactory * m_factory;
		QTime m_last_request_time;
		unsigned int m_skipped;
		unsigned int m_maxSkipped;
		int m_interval;
	    private:
		void __init();
	};
    }
}

#endif
