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

#ifndef __WEBEXTRACTOR_TEST_REPLY_TIMEOUT_H_
#define __WEBEXTRACTOR_TEST_REPLY_TIMEOUT_H_


#include <QtCore/QObject>
#include <QtTest/QtTest>

#include "simpledatappreply.h"
#include "decisionfactory.h"
#include <Nepomuk/Resource>
#include <QtCore/QTimer>
using namespace Nepomuk::WebExtractor;

class TestTimeoutReply : public SimpleExecutiveReply
{
    Q_OBJECT;
    public:
	TestTimeoutReply(Executive * parent, const  DecisionFactory * factory, const Nepomuk::Resource & res):
	    SimpleExecutiveReply(parent,factory,res)
	{
	    kDebug() << "Set timeout";
	    this->setTimeout(200);
	}
	public Q_SLOTS:
	    void abort() {;}
	    bool isValid() const { return true; }
	    void aboutToTimeOut()
	    {
		kDebug() << "About to finish";
	    }

	

};



class TestTimeout : public QObject
{
    Q_OBJECT;
    private Q_SLOTS:
	void timeoutAbort()
	{
	    Nepomuk::Resource res;
	    DecisionFactory * factory = DecisionFactory::debugFactory();
	    Executive * dpp = new ExecutiveTempalate<TestTimeoutReply>();
	    ExecutiveReply * reply = dpp->requestDecisions(factory, res );
	    //QSignalSpy spy(reply, SIGNAL(error(ExecutiveReply::ExecutiveReplyError)));
	    
	    /*
	    QTimer t;
	    QSignalSpy spy(&t, SIGNAL(timeout()));
	    t.setSingleShot(true);
	    t.setInterval(200);
	    t.start();
	    */
	    QTest::qWait(reply->timeout() + 100 );
	    //QCOMPARE(spy.count(), 1);
	    //QVERIFY(!t.isActive());
	    QCOMPARE(reply->error(),ExecutiveReply::TimeExceeded);
	}

};


#endif
