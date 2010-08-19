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

#ifndef __nepomuk_servicedata_backend_h_
#define __nepomuk_servicedata_backend_h_

#include <QMap>
#include <QDateTime>
#include <QUrl>
#include <QString>
#include <Nepomuk/Resource>
namespace Nepomuk
{
    namespace WebExtractor
    {
        /*! \brief This is interface class for storing information about examined DataPP
         * Different storages will be available - SQL, Nepomuk, etc. Backend is bound to
         * resource. For each new Nepomuk::Resource object new instance of backend will
         * be created
         */
        class ServiceDataBackend : public QObject
        {
                Q_OBJECT;
            public:
                /*! \brief Add/Update information about examined DataPP ( name, version)
                         * This function is used to mark that DataPP with given name (dataPPName) has finished extraction
                         * for the resource. Current time is used as time of extraction finishing. Version of the DataPP
                         * is also stored. If the previous version of the DataPP was examined, then the version is updated.
                         * Implementation must not store the more, than one DataPP, that differ only by version!
                         * \param dataPPName Name of the DataPP
                         * \param dataPPVersion Version of the DataPP
                 */
                virtual void setExaminedDataPPInfo(const QString & dataPPName, int dataPPVersion, const QDateTime & date = QDateTime()) = 0;

                /*! \brief This function checks that all given DataPP has been examined for the resource.
                 * Default implementation will compare examinedDataPPInfo() with given map
                 */
                virtual bool checkExaminedDataPPInfo(const QMap< QString, int> & dataPPInfo) ;
                /*! \brief Return map (name, version ) about all examined DataPP
                         * This function must return the set of non-obsolete ( this is important ) records about DataPP that
                         * has been examined. Usually this mean then implementations of this function should clear
                         * all obsolete data upon the call.
                         * \note The return value must be Map, not MultiMap. Do not store several versions of the same DataPP marked
                         * as examined.
                         *
                         * \return Return the map <name, version>
                 */
                virtual QMap< QString, int > examinedDataPPInfo() = 0;

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


                /*! \brief Clear all information about examined DataPP
                         * This function should remove all information about examined DataPP. After call to this function,
                         * the examinedDataPPInfo() must return empty map. But if any other information is stored too, it
                         * could remain in storage.
                 */
                virtual void clearExaminedInfo() = 0;
                /*! \brief Unmark DataPP with given name as examined
                 * This function mark DataPP as not-examined. Version parameter is not necessary, because there must be
                 * only one version of the DataPP marked as examined.
                 */
                virtual void clearExaminedInfo(const QString & name) = 0;

                /*! \brief Clear all webextrator service information about this resource
                 * This function will clear examined info ( can be done directly via clearExaminedInfo() ),
                 * and all other stored service information
                 * This function is more for feature releases
                         * \note Default implementation will call clearExaminedInfo().
                 */
                virtual void clearServiceInfo();


                /*! \brief Return resource of the ServiceDataBackend.
                 * Each ServiceDataBackend is bound to some resource ( look at the constructor ).
                 */
                virtual Nepomuk::Resource  resource() const = 0;
                /*! \brief Return the url of the resources of the ServiceDataBackend.
                         */
                virtual const QUrl & uri() const = 0 ;

                /*! \brief return names of properties where some usefull debugging information is stored
                 * This function return names of Qt properties. This names are
                 * implementation depended. For example SQL backend may return
                 * table name, and NRL backend - graph name
                         * \note This function is more for debugging/developing proposes. It allow introspection of the
                         * the backend.
                 */
                virtual QStringList serviceInfoPropertiesNames() const;

                virtual ~ServiceDataBackend();
        };

    }
}

#endif
