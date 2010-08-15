/*
   Copyright (C) 2010 by Serebriyskiy Artem <v.for.vandal at gmail.com>
    Copyright (C) 2010  Vishesh Handa <handa.vish@gmail.com>

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

#ifndef __NEPOMUK_MODEL_GRAPH_ALGORITHM_H_
#define  __NEPOMUK_MODEL_GRAPH_ALGORITHM_H_

#include <QList>
#include <QSet>
#include <QUrl>
#include "webextractor_export.h"

namespace Nepomuk
{
    namespace Graph
    {

        class ModelGraph;
        class ModelGraphVisitor;

        /*!\brief Visit graph
         * \param depth_limit Algorithm will ignore all nodes whoes depth is more or equal then \p depth_limit. Start vertices of the
         * algorithm ( \p targets ) has depth 0. If set to -1, then no limit will be used.
             * \param visitors List of visitors. If empty, then algorithm will not execute
         */
        WEBEXTRACTOR_EXPORT void visit_model_graph(
            ModelGraph * graph,
            const QSet<QUrl> & targets,
            QList<ModelGraphVisitor *> visitors,
            int depth_limit = -1,
            bool caching = true);

        WEBEXTRACTOR_EXPORT void visit_model_graph(
            ModelGraph * graph,
            const QSet<QUrl> & targets,
            ModelGraphVisitor * visitors,
            int depth_limit = -1,
            bool caching = true);
    }
}

#endif
