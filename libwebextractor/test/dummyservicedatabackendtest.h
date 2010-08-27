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

#ifndef __WEBEXTRACTOR_TEST_DUMMY_SDB_H_
#define __WEBEXTRACTOR_TEST_DUMMY_SDB_H_

#include <QtCore/QObject>
#include <QtTest/QtTest>

#include "dummyservicedatabackend.h"
#include <Nepomuk/Resource>


using namespace Nepomuk::WebExtractor;

class DummySDBTest : public QObject
{
    Q_OBJECT;
    public:
	DummySDBTest():
	    factory(0),
	    backend(0)
    {;}

    private Q_SLOTS:
	void init()
	{
	    factory = new DummyServiceDataBackend::Factory();
	    backend = factory->backend(Nepomuk::Resource());
	}
	void cleanup()
	{
	    delete factory;
	    delete backend;
	}

	void write()
	{
	    QString name = "TEST";
	    int version = 42;

	    backend->setExaminedDataPPInfo(name, version);
	    QVERIFY(!backend->checkExaminedDataPPInfo(name,version));
	}

	void read()
	{
	    QString name = "TEST";
	    int version = 42;
	    QVERIFY(backend->examinedDataPPInfo().isEmpty());
	    QVERIFY(!backend->checkExaminedDataPPInfo(name));
	    QVERIFY(!backend->checkExaminedDataPPInfo(name,version));
	}

    private:
	DummyServiceDataBackend::Factory * factory;
	ServiceDataBackend * backend;
};

#endif
