/*
   Copyright (C) 2010 by Serebriyskiy Artem <v.for.vandal at gmail.com>

   This library is free software; you can redistribute it and/or modify
   it under the terms of the GNU Library General Public License as published by
   the Free Software Foundation; either version 3, or (at your option)
   any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#ifndef _NEPOMUK_WEBEXTRCT_RESOURCE_ANALYZER_FACTORY_H_
#define _NEPOMUK_WEBEXTRCT_RESOURCE_ANALYZER_FACTORY_H_

#include <QtCore/QObject>
#include "webextractor_export.h"
#include "parameters.h"
#include <Nepomuk/Resource>
#include <Soprano/BackendSettings>


namespace Soprano
{
    class Backend;
}
namespace Nepomuk
{
    class OntologyLoader;

    namespace WebExtractor
    {
        class ResourceAnalyzer;

        class WEBEXTRACTOR_EXPORT ResourceAnalyzerFactory: public QObject
        {
                Q_OBJECT;
            public:
                ResourceAnalyzerFactory(
                    const ExtractParameters& extractParams,
                    QObject * parent = 0
                );
                ~ResourceAnalyzerFactory();
                /*! \brief Create new ResourceAnalyzer
                 * If settings that were passed are incorrect, or can not be
                 * satisfied, then NULL will be returned. So you should check
                 * for it.
                 */
                ResourceAnalyzer * newAnalyzer();
                /*! \brief Create new ResourceAnalyzer and bound it to resource
                 * This is an overload, provided for convinience.
                 */
                ResourceAnalyzer * newAnalyzer(const Resource & res);
                /*! \brief Delete ResourceAnalyzer.
                 * TODO Delete this method as unnecessary.
                 */
                void deleteAnalyzer(ResourceAnalyzer * res);
            private:
                class Private;
                Private * const d;
        };
    }
}

#endif
