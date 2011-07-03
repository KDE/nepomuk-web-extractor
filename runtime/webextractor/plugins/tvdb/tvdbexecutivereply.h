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

#ifndef __webextractor_datappreply_tvdb_h_
#define __webextractor_datappreply_tvdb_h_

#include "simpleexecutivereply.h"
#include "tvshowfilenameanalyzer.h"

#include <QtCore/QList>

#include <tvdb/series.h>

namespace Nepomuk  {
    class TvdbExecutive;

    class TvdbReply : public WebExtractor::SimpleExecutiveReply
    {
        Q_OBJECT

    public:
        TvdbReply(TvdbExecutive* parent, const WebExtractor::DecisionFactory* factory, const Nepomuk::Resource& res, const QString& name, int season, int episode );

        virtual void abort();
        virtual bool isValid() const;

    private Q_SLOTS:
        void slotRequestDone( int id, const QList<Tvdb::Series>& );

    private:
        int m_seriesCacheId;
        const QString m_name;
        const int m_season;
        const int m_episode;
    };
}

#endif

