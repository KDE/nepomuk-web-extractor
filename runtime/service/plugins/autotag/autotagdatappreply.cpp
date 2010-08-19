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




#include "autotagplugin_config.h"

#include "autotagdatappreply.h"

#include "autotagdatapp.h"

#include "decisionfactory.h"
#include <Nepomuk/Resource>
#include <Nepomuk/Variant>
#include <Nepomuk/Tag>
#include <Soprano/Vocabulary/NAO>
#include <Soprano/Statement>
#include "nfo.h"
#include <KDebug>
#include <QTimer>

namespace NW = Nepomuk::WebExtractor;

Nepomuk::AutotagReply::AutotagReply(AutotagDataPP * parent, const WebExtractor::DecisionFactory * factory, const Nepomuk::Resource & res):
    SimpleDataPPReply(parent, factory, res)
{
    // Check that resource is file object
    if(!res.hasType(Nepomuk::Vocabulary::NFO::FileDataObject())) {
        kDebug() << "Resource is not a file";
        // Set error
        setError(ResourceTypeIncorrect);
        // Finish
        QTimer::singleShot(0, this, SLOT(finish()));
        return;
    }



    // Take regexp from the parent
    QRegExp regexp = parent->m_regexp;

    // Take tag name from the parent
    QString tagName = parent->m_tag;

    // Check that file name math regexp
    // Take the filename
    QString filename = res.property(Nepomuk::Vocabulary::NFO::fileName()).toString();
    if(regexp.exactMatch(filename)) {
        // Generate Decision and assign tag
        NW::Decision d = newDecision();
        Q_ASSERT(d.isValid());
        Q_ASSERT(d.manager());
        // Generate proxy resource for main resource
        // After this call Resource with uri proxyResUrl will exist.
        Nepomuk::Resource proxyRes = d.proxyResource(res);
        QUrl proxyResUrl = proxyRes.resourceUri();
#if 0
#endif
        // Create PropertyGroup
        NW::PropertiesGroup grp = d.newGroup();
        //Q_ASSERT(grp);
        // Create tag in this model
        Nepomuk::Tag t(tagName, d.manager());
        // The code above is not enough to actually create resource.
        // We will set the description of the tag and this will do the work
        t.setDescription("What on the hell this tag for ?");
        // Bound tag to the resource
        grp.makeCurrent();
        proxyRes.addTag(t);


        // Set group rank
        grp.setRank(0.99);

        // Set rank of the Decision
        d.setRank(0.99);
        // Set description of the Decision
        static QString descriptionTemplate = QString("Assign tag %1 to the resource %2");
        d.setDescription(descriptionTemplate.arg(tagName, proxyResUrl.toString()));
        // Add Decision to the main list
        addDecision(d);

    } else {
        kDebug() << "File name doesn't match parameter";
    }
    // Finish
    finish();

    // Thats all
}

bool Nepomuk::AutotagReply::isValid() const
{
    return true;
}

void Nepomuk::AutotagReply::abort()
{
    return;
}
