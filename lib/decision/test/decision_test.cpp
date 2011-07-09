/*
   This file is part of the Nepomuk KDE project.
   Copyright (C) 2011 Serebriyskiy Artem <v.for.vandal@gmail.com>

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) version 3, or any
   later version accepted by the membership of KDE e.V. (or its
   successor approved by the membership of KDE e.V.), which shall
   act as a proxy defined in Section 6 of version 3 of the license.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with this library.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "decision_test.h"

#include "../decisionfactory.h"
#include "../decisionauthor.h"
#include <nepomuk/simpleresourcegraph.h>
#include <nepomuk/simpleresource.h>

#include <QByteArray>
#include <QDataStream>
#include <QtTest/QTest>


using namespace Nepomuk;
using namespace Nepomuk::Decision;

class TestDecisionAuthor : public DecisionAuthor
{
    public:
        QString name() const { return "TestName"; }
        int version() const { return 1; }
};

void DecisionTest::initTestCase()
{
    this->decision = createDecision();
    this->propgrp = createPropertisGroup();
}

Decision::PropertiesGroup DecisionTest::createPropertisGroup()
{
    Decision::PropertiesGroup pg;
    pg.setRank(0.4);
    pg.setDescription("Reference properties group");

    SimpleResourceGraph changes;
    SimpleResource res(QUrl("nepoumk://res1"));
    res.addProperty(QUrl("prop1"),QVariant(25));
    changes << res;
    pg.setChanges(changes);

    return pg;
}

Decision::Decision DecisionTest::createDecision()
{
    DecisionCreator creator = DecisionFactory::debugFactory()->newDecision( new TestDecisionAuthor());
    SimpleResourceGraph changes;
    SimpleResource res(QUrl("nepoumk://res1"));
    res.addProperty(QUrl("prop1"),QVariant(25));
    changes << res;
    creator.setChanges(changes);
    creator.setRank(0.5);
    creator.setDescription("Reference description");
    return creator.data();
}


void DecisionTest::testDecisionStream()
{
    QByteArray data;
    QDataStream stream(&data,QIODevice::ReadWrite);
    stream << decision;
    //qDebug() << this->decision;

    Decision::Decision d;
    QDataStream rstream(&data,QIODevice::ReadOnly);
    rstream >> d;
    //qDebug() << d;
    QCOMPARE(d,decision);
}

void DecisionTest::testDecisionEquality()
{
    QCOMPARE(this->decision,this->decision);

    Decision::Decision d2 = this->decision;
    QCOMPARE(d2,this->decision);

    Decision::Decision d3 = createDecision();
    //qDebug() << this->decision;
    //qDebug() << d3;
    QCOMPARE(d3,decision);
}

void DecisionTest::testPropertiesGroupEquality()
{
    QCOMPARE(this->propgrp, this->propgrp);

    PropertiesGroup grp2 = propgrp;
    QCOMPARE(this->propgrp, grp2);

    PropertiesGroup grp3 = createPropertisGroup();
    QCOMPARE(this->propgrp, grp2);

}


QTEST_MAIN(DecisionTest);
