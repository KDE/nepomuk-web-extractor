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

#ifndef __nepomuk_servicedata_backend_factory_h_
#define __nepomuk_servicedata_backend_factory_h_

#include "servicedatabackend.h"
#include "webextractor_export.h"
#include <Nepomuk/Query/Query>
#include <Nepomuk/Query/Term>
#include <Nepomuk/ResourceManager>

namespace Soprano
{
    class Model;
}

namespace Nepomuk
{
    namespace WebExtractor
    {

        class WEBEXTRACTOR_EXPORT ServiceDataBackendFactory
        {
            public:
                /*! \brief Return ServiceDataBackend for giver resource
                 * \param res Uri of the resource
                 */
                virtual ServiceDataBackend * backend(const Nepomuk::Resource & res) = 0;
                /*! \brief Return Nepomuk::Query that will match all resources that should be examined with given Executive
                 * This method must be eqivalent for quering all resources that matchs mainTerm and checking for every resource
                 * recived that any of the given Executive has not be examined for the resources. If such query can not be created,
                 * then invalid Query must be returned. Default implementation will return invalid query;
                 * \param assignedExecutive Map of Executive name, Executive version
                         * \return Query for selecting all unparsed resource or invalid query if this feature is not supported
                 */
                virtual Nepomuk::Query::Query queryUnparsedResources(const Nepomuk::Query::Term & mainTerm, const QMap<QString, int> & assignedExecutive, Soprano::Model * model) {
                    Q_UNUSED(mainTerm);
                    Q_UNUSED(assignedExecutive);
                    Q_UNUSED(model);
                    return Nepomuk::Query::Query();
                }

                virtual ~ServiceDataBackendFactory() {
                    ;
                }
        };

        template < typename T >
        class ServiceDataBackendFactoryTemplate : public ServiceDataBackendFactory
        {
            public:
                ServiceDataBackend * backend(const Nepomuk::Resource & res) {
                    return new T(res);
                }
                ~ServiceDataBackendFactoryTemplate() {
                    ;
                }
        };
    }
}
#endif
