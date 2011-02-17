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


#ifndef __nepomuk_resourcedatappwrapper_backend_factory_h_
#define __nepomuk_resourcedatappwrapper_backend_factory_h_

#include "webextractor_export.h"
#include "servicedatabackend.h"
#include "servicedatabackendfactory.h"
#include "nepomukservicedatabackend.h"

namespace Nepomuk
{
    namespace WebExtractor
    {
        class WEBEXTRACTOR_EXPORT NepomukServiceDataBackendFactory : public ServiceDataBackendFactory
        {
                ServiceDataBackend * backend(const Nepomuk::Resource & res) {
                    return new NepomukServiceDataBackend(res);
                }
                ~NepomukServiceDataBackendFactory() {
                    ;
                }
                virtual Nepomuk::Query::Query queryUnparsedResources(const Nepomuk::Query::Term & mainTerm, const QMap<QString, int> & assignedExecutive, Soprano::Model * model) {
                    return NepomukServiceDataBackend::queryUnparsedResources(mainTerm, assignedExecutive, model);
                }
        };
    }
}
#endif

