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

#ifndef __nepomuk_resourcedatappwrapper_backend_h_
#define __nepomuk_resourcedatappwrapper_backend_h_

#include "servicedatabackend.h"
#include <QMap>
#include <QString>

namespace Nepomuk
{
    namespace WebExtractor
    {
        /*! \brief This is backend for storing information service information inside Nepomuk
         */
        class NepomukServiceDataBackend : public ServiceDataBackend
        {
                Q_OBJECT;
            public:
                NepomukServiceDataBackend(const QUrl &);
                /*! \brief Add/Update information about examined DataPP ( name, version)
                 */
                virtual void setExaminedDataPPInfo(const QString & dataPPName, const QString & dataPPVersion) ;

                /*! \brief Return map (name, version ) about all examined DataPP
                 */
                virtual QMap< QString, QString > examinedDataPPInfo();

                /*! \brief Clear all information about examined DataPP
                 */
                virtual void clearExaminedIfno() ;
                virtual void clearExaminedIfno(const QString & name) ;

                // TODO Reimplement function clearServiceInfo. This function must clear
                // all unused DataPP Resources, all examined info ( including statements
                // that are outside appropriate graph ( clearExaminedIfno() doesn't do
                // this). Such statements may appear because of some program errors.
                virtual void clearServiceInfo();

                virtual Nepomuk::Resource resource() const;
                virtual const QUrl & uri() const;

                QUrl readGraphName() const;

                Q_PROPERTY(QUrl graphName READ readGraphName);

                virtual QStringList serviceInfoPropertiesNames() const;

            private:
                void loadGraph();
                void loadCreateGraph();
                /*! \brief return url of resource, corresponding this DataPP
                 * If there is no such resource, then this resource will be created.
                 * name is the name of the DataPP, version is its version.
                 * If name or version are invalid(empty) invalid url will be returned
                 */
                static QUrl dataPPResourceUrl(const QString & name, const QString & version);
                static void clearUnusedDataPP();
                // FIXME Only one of the folowing 2 variables is necessary, because
                // we can retrive resource by url  and url by resource
                Nepomuk::Resource  m_res;
                QUrl m_url;
                // End
                Soprano::Node m_graphNode;
        };
    }
}

#endif

