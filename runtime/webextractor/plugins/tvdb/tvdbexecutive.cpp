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

#include "tvdbplugin_config.h"
#include "tvdbexecutive.h"
#include "tvdbexecutivereply.h"
#include "seriescache.h"

#include <webextractor/executivereply.h>

#include <Nepomuk/File>


Nepomuk::TvdbExecutive::TvdbExecutive(int pluginVersion)
    : Executive(pluginVersion)
{
    m_seriesCache = new SeriesCache(this);
}


Nepomuk::WebExtractor::ExecutiveReply* Nepomuk::TvdbExecutive::requestDecisions(const Nepomuk::Decision::DecisionFactory* factory, const Nepomuk::Resource & res)
{
    if(res.isFile()) {
        const KUrl url = res.toFile().url();
        QList<TVShowFilenameAnalyzer::AnalysisResult> result = m_filenameAnalyzer.analyzeFilename(url.toLocalFile());
        bool hasValid = false;
        if(result.size() > 0) {
            foreach( const TVShowFilenameAnalyzer::AnalysisResult c, result)
            {
                if ( c.isValid() ) {
                    hasValid = true;
                    break;
                }
            }
        }

        if ( hasValid ) {
            return new TvdbReply(this, factory, res, result);
        }
        else {
            kDebug() << res.resourceUri() << "failed to analyze the filename - no tv show file?";
        }
    } else {
        kDebug() << res.resourceUri() << "is not a file";
    }
    return 0;
}

Nepomuk::TvdbExecutive::~TvdbExecutive()
{
}
