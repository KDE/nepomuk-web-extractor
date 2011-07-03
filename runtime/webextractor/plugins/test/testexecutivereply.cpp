/*
   Copyright (C) 2010 by Artem Serebriyskiy <v.for.vandal@gmail.com>

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




#include "testplugin_config.h"

#include "testexecutivereply.h"

#include "testexecutive.h"

#include <decision/decisionfactory.h>
#include <Nepomuk/Resource>
#include <Nepomuk/Tag>
#include <Nepomuk/Variant>
#include <Soprano/Vocabulary/NAO>
#include <KDebug>
#include <QTimer>

namespace NW = Nepomuk::WebExtractor;
namespace ND = Nepomuk::Decision;

Nepomuk::TestReply::TestReply(TestExecutive * parent, const Decision::DecisionFactory * factory, const Nepomuk::Resource & res):
    SimpleExecutiveReply(parent, factory, res)
{
    bool result = true;

    result = result && testProxy();

    // Create final decision
    ND::DecisionCreator d = newDecision();
    Nepomuk::Resource r =  d.proxyResource(resource());


    QString tagName = (result) ? QString("TestSuccess") : QString("TestFailure");
    Nepomuk::Tag t(tagName, d.manager());
    t.setLabel("Tag indicating result of the tests");
    t.setProperty(Soprano::Vocabulary::NAO::prefLabel(), Nepomuk::Variant(tagName));

    r.addTag(t);

    d.setDescription(m_testsResults.join("\n"));

    addDecision(d);

    // Finish
    finish();

}

bool Nepomuk::TestReply::isValid() const
{
    return true;
}

void Nepomuk::TestReply::abort()
{
    return;
}
/* This method will test that calling proxyResource() twice
 * on the same resource will return the same QUrl
 */
bool Nepomuk::TestReply::testProxy()
{
    ND::DecisionCreator d = newDecision();
    QUrl first = d.proxyUrl(resource());
    if(first.isEmpty()) {
        m_testsResults << "Proxy Test: Failed to create proxy for resource";
        return false;
    }

    QUrl second = d.proxyUrl(resource());
    if(first.isEmpty()) {
        m_testsResults << "Proxy Test: Failed to return proxy for resource at second call";
        return false;
    }

    if(first != second) {
        m_testsResults << "Proxy Test: Urls from first call and from second call are differet: " << first.toString() << " vs " << second.toString();
        return false;
    }
    m_testsResults << "Proxy Test: SUCCESS";
    return  true ;
}
