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

#ifndef webextractor_simplenetworkexecutive_h
#define webextractor_simplenetworkexecutive_h

#include "executive.h"
//#include "simplenetworkreplyfactory.h"
#include <QNetworkAccessManager>
#include <QTimer>
#include <QQueue>
#include <QTime>
#include <QMutex>
#include "webextractor_export.h"
#include "simplenetworkreplyfactory.h"
#include "simplenetworkrequest.h"
namespace Nepomuk
{
    namespace WebExtractor
    {
        class SimpleNetworkExecutiveReply;
        class WEBEXTRACTOR_EXPORT SimpleNetworkExecutive : public Nepomuk::WebExtractor::Executive
        {
                Q_OBJECT;
            public:
                // SimpleNetworkExecutive will take parentship on factory
                SimpleNetworkExecutive(int dataPPVersion, const QString & endpointUrl, SimpleReplyFactory * factory);
                // Safe therad. Called from another thread - now from thread where
                // Datapp leaves
                virtual ExecutiveReply * requestDecisions(const DecisionFactory * factory, const Nepomuk::Resource & res);
                //virtual ExecutiveReply * requestDecisions(const DecisionFactory * factory, const Nepomuk::Resource & res,QObject * target, const char * finishedSlot, const char * errorSlot);
                // This method can be redefined in sucessors. Default implementation will use
                // given factory (if any).
                virtual Nepomuk::WebExtractor::SimpleNetworkExecutiveReply * newReply(const Nepomuk::WebExtractor::DecisionFactory * factory, const Nepomuk::Resource & res);
                virtual ~SimpleNetworkExecutive();


                void get(SimpleNetworkExecutiveRequest * request);
            protected Q_SLOTS:
                //void launchNext();
                void timeout();
                void startTimer();
            protected:
                void enqueue(SimpleNetworkExecutiveRequest * request);
                // Return 0 if queue was empty. Actualy it should never happens.
                int runNext();
                void getRequest(SimpleNetworkExecutiveRequest * request);
                // void enqueue(SimpleNetworkExecutiveReply*);
                // This is constuctor for class that inherit this one and doesn't require factory
                SimpleNetworkExecutive(int dataPPVersion, const QString & serverName, const QString & endpointUrl);
                QNetworkAccessManager * m_net;
                QTimer * m_reqTimer;
                bool m_timeout;
                QMutex m_timeoutMutex;
                QMutex m_queueMutex;
                QQueue<SimpleNetworkExecutiveRequest*> m_queue;
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
