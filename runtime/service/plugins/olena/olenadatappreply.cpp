/*
   Copyright (C) 2010 by Sebastian Trueg <trueg@kde.org>

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


#include "olenadatappreply.h"
#include "olenadatapp.h"

#include "text_extraction.hh"

#include <QtGui/QImage>

#include <webextractor/decisionfactory.h>
#include <Nepomuk/File>
#include <Nepomuk/Variant>
#include <Nepomuk/Vocabulary/NIE>

#include <KDebug>

namespace {
// TODO: perform some useful checks
bool checkText( const QString& text ) {
    return !text.isEmpty();
}
}

Nepomuk::OlenaReply::OlenaReply(OlenaDataPP * parent, const WebExtractor::DecisionFactory * factory, const Nepomuk::Resource & res )
    : SimpleDataPPReply(parent, factory, res)
{
    kDebug() << res;
    if( res.isFile() && res.toFile().url().isLocalFile() ) {
        const KUrl url = res.toFile().url();
        kDebug() << url;
        QImage img;
        if( img.load(url.toLocalFile()) ) {
            QString extractedText = scribo::toolchain::nepomuk::text_extraction( img );
            kDebug() << extractedText;
            if( checkText( extractedText ) ) {
                kDebug() << "Creating decision";

                // create the new decision
                WebExtractor::Decision d = newDecision();
                WebExtractor::PropertiesGroup g = d.newGroup();

                // set the actual data
                g.proxyResource(res).setProperty(Nepomuk::Vocabulary::NIE::plainTextContent(), extractedText);

                // we are very sure about this since we do not want to burden any user with looking at text
                // extracted from images
                d.setRank(1.0);

                addDecision(d);
            }
        }
        else {
            kDebug() << "Failed to load image data from" << url;
        }
    }

    finish();
}

bool Nepomuk::OlenaReply::isValid() const
{
    return true;
}

void Nepomuk::OlenaReply::abort() 
{
    // we cannot abort. It is all or nothing here.
}
