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

#ifndef __NEPOMUK_ALGORITHM_H_
#define __NEPOMUK_ALGORITHM_H_

#include <QUrl>
#include <Soprano/Node>
#include <QUrl>
#include "modelgraphvisitor.h"
#include "webextractor_export.h"

class QTextStream;

namespace Soprano
{
    class Model;
}

namespace Nepomuk
{

    class Resource;
    class ResourceManager;

    /*! Make a deep copy of the resource.
     * \param from Resource to copy
     * \param to ResourceManager that manages a model to copy to
     * \param sameModels Set this parameter to true if model of \p from and model of \p to
     * are the same. This will prevent deadlocks
     * \return Url of the new resource
     */
    WEBEXTRACTOR_EXPORT QUrl  deep_resource_copy(const Nepomuk::Resource & from,  Nepomuk::ResourceManager * to = 0, bool sameModels = true);


    /*! Make a deep copy or the resource, respecting already copied parts.
     * This function is usefull when you want to copy resources and it childs to the
     * model in several steps. This function guarantees that each resource will be copied
     * only once.
     * \param convtable This is table of previously copied resources. In form
     * < original resource url, copy resource url>
     */
    WEBEXTRACTOR_EXPORT QHash<QUrl, QUrl> *  deep_resource_copy_adjust(const Nepomuk::Resource & from,  Nepomuk::ResourceManager * to = 0,   QHash<QUrl, QUrl> * convtable = 0 , bool sameModels = true);

    /*! \brief Write graph as text
     * Use this function only for debugging.
     */
    WEBEXTRACTOR_EXPORT void dump_resource_as_text(const Nepomuk::Resource & from, QTextStream & stream, int depth_limit = -1);
    /*! \brief Write graph as text
     * Use this function only for debugging.
     */
    WEBEXTRACTOR_EXPORT void dump_resource_as_text(const QUrl & from, Soprano::Model * model, QTextStream & stream, int depth_limit = -1);
    WEBEXTRACTOR_EXPORT void dump_resources_as_text(const QSet<QUrl> & resources, Soprano::Model * model, QTextStream & stream, int depth_limit = -1);



}

#endif
