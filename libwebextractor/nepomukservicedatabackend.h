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
#include <Nepomuk/Query/Query>
#include <Nepomuk/Query/ComparisonTerm>
#include "webextractor_export.h"

namespace Soprano
{
    class Model;
}

namespace Nepomuk
{
    namespace WebExtractor
    {
        /*! \brief This is backend for storing information service information inside Nepomuk
             * All information is stored in separate graph and this graph is bound to the resource
             * with ndco:decisionMetaGraphFor property.
         */
        class WEBEXTRACTOR_EXPORT NepomukServiceDataBackend : public ServiceDataBackend
        {
                Q_OBJECT;
            public:
                NepomukServiceDataBackend(const Nepomuk::Resource &);
                /*! \brief Add/Update information about examined DataPP ( name, version)
                 */
                virtual void setExaminedDataPPInfo(const QString & dataPPName, int dataPPVersion, const QDateTime & = QDateTime());

                /*! \brief Return map (name, version ) about all examined DataPP
                 */
                virtual QMap< QString, int > examinedDataPPInfo();

                /*! \brief Clear all information about examined DataPP
                 */
                virtual void clearExaminedInfo() ;
                virtual void clearExaminedInfo(const QString & name) ;

                // TODO Reimplement function clearServiceInfo. This function must clear
                // all unused DataPP Resources, all examined info ( including statements
                // that are outside appropriate graph ( clearExaminedInfo() doesn't do
                // this). Such statements may appear because of some program errors.
                virtual void clearServiceInfo();

                QUrl readGraphName() const;

                Q_PROPERTY(QUrl graphName READ readGraphName);

                virtual QStringList serviceInfoPropertiesNames() const;

                /*! \brief This is optional function, that returns map of (DataPP name, last extraction date)
                 * This function is for introspection proporses only.
                 *
                 * Backends that doesn't store this information may not implement this function. Default one will return
                 * empty map
                 */
                virtual QMap< QString, QDateTime > examinedDataPPDates();

                /*! \brief This is optional function that return last extraction date for given DataPP
                 * This function is for introspection proporses only.
                 *
                 * This function should return date of last extraction of DataPP with given name. If DataPP with given name
                 * has not been used, then invalid Date should be return.
                 * Backends that doesn't store this information may not implement this function. Default one will return
                 * invalid DataPP.
                 */
                virtual QDateTime examinedDate(const QString & name);

                static Nepomuk::Query::Query queryUnparsedResources(const Nepomuk::Query::Term & mainTerm, const QMap<QString, int> & assignedDataPP, Soprano::Model * model);

            private:
                /*! \brief This function tries to found graph where all information should be stored
                 * If there is no such graph, then this function will do nothing. It will not create the one!
                 */
                void loadGraph();
                /*! \brief This function behaves like loadGraph(), but it create the graph, if there is no one
                 */
                void loadCreateGraph();
                /*! \brief return url of resource, corresponding this DataPP
                 * If there is no such resource, then this resource will be created.
                 * name is the name of the DataPP, version is its version.
                 * If name or version are invalid(empty) invalid url will be returned
                 */
                static QUrl dataPPResourceUrl(const QString & name, int version, ResourceManager * manager);
                /*! \brief This function remove unnecessary DataPP resources
                 * TODO Enable it usage
                 */
                static void clearUnusedDataPP(ResourceManager * manager);

                static QString date_query_templ() ;

                // FIXME Only one of the folowing 2 variables is necessary, because
                // we can retrive resource by url  and url by resource
                Nepomuk::Resource  m_res;
                QUrl m_url;
                // End
                // This is the node that store graph where all decisions are stored
                Soprano::Node m_graphNode;

                Nepomuk::Query::Query m_dataPPQuery;
                Nepomuk::Query::ComparisonTerm m_dataPPTerm;
        };
    }
}

#endif

