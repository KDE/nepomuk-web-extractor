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

#include "autotagexecutivereply.h"

#include "autotagexecutive.h"

#include "decisionfactory.h"
#include <Nepomuk/Resource>
#include <Nepomuk/Variant>
#include <Nepomuk/Tag>
#include <Soprano/Vocabulary/NAO>
#include <Soprano/Statement>
#include <Soprano/Model>
#include "nfo.h"
#include <KDebug>
#include <nepomuk/simpleresource.h>

namespace NW = Nepomuk::WebExtractor;
namespace ND = Nepomuk::Decision;

Nepomuk::AutotagReply::AutotagReply(AutotagExecutive * parent, const Decision::DecisionFactory * factory, const Nepomuk::Resource & res):
    SimpleExecutiveReply(parent, factory, res)
{
    // Check that resource is file object
    if(!res.hasType(Nepomuk::Vocabulary::NFO::FileDataObject())) {
        kDebug() << "Resource is not a file";
        // Set error
        setError(ResourceTypeIncorrect);
        // Finish
	finish();
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
        ND::DecisionCreator d = newDecision();
        Q_ASSERT(d.isValid());

        Nepomuk::SimpleResourceGraph changes;
        Nepomuk::SimpleResource resChanges(res.resourceUri());
        Nepomuk::SimpleResource tag;
        tag.addType(Soprano::Vocabulary::NAO::Tag());
        tag.addProperty(Soprano::Vocabulary::NAO::description(),QString("What on the hell this tag for?"));
        resChanges.addProperty(Soprano::Vocabulary::NAO::hasTag(),tag);

        changes.insert(resChanges);
        changes.insert(tag);

        d.setChanges(changes);


        // Set rank of the Decision
        d.setRank(0.99);
        // Set description of the Decision
        static QString descriptionTemplate = QString("Assign tag %1 to the resource %2");
        d.setDescription(descriptionTemplate.arg(tagName, res.resourceUri().toString()));
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
