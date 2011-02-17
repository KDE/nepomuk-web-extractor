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
        /*! \brief This is interface class for storing information about examined Executive
         * Different storages will be available - SQL, Nepomuk, etc. Backend is bound to
         * resource. For each new Nepomuk::Resource object new instance of backend will
         * be created
         */
        class ServiceDataBackend : public QObject
        {
                Q_OBJECT;
            public:
                /*! Constructor.
                      * \param res Resource for the backend
                      */
                ServiceDataBackend(const Nepomuk::Resource & res);
                /*! \brief Add/Update information about examined Executive ( name, version)
                         * This function is used to mark that Executive with given name (dataPPName) has finished extraction
                         * for the resource. Current time is used as time of extraction finishing. Version of the Executive
                         * is also stored. If the previous version of the Executive was examined, then the version is updated.
                         * Implementation must not store the more, than one Executive, that differ only by version!
                         * \param dataPPName Name of the Executive
                         * \param dataPPVersion Version of the Executive
                 */
                virtual void setExaminedExecutiveInfo(const QString & dataPPName, int dataPPVersion, const QDateTime & date = QDateTime()) = 0;

                /*! \brief This function checks that all given Executive has been examined for the resource.
                 * Default implementation will compare examinedExecutiveInfo() with given map
                 */
                virtual bool checkExaminedExecutiveInfo(const QMap< QString, int> & dataPPInfo) ;

                /*! \brief This function checks whether the Executive with given name was examined
                 * Version is unimportant
                 */
                virtual bool checkExaminedExecutiveInfo(const QString & name);

                /*! \brief This function checks whether the Executive with given name and version was examined
                 */
                virtual bool checkExaminedExecutiveInfo(const QString & name, int version);
                /*! \brief Return map (name, version ) about all examined Executive
                         * This function must return the set of non-obsolete ( this is important ) records about Executive that
                         * has been examined. Usually this mean then implementations of this function should clear
                         * all obsolete data upon the call.
                         * \note The return value must be Map, not MultiMap. Do not store several versions of the same Executive marked
                         * as examined.
                         *
                         * \return Return the map <name, version>
                 */
                virtual QMap< QString, int > examinedExecutiveInfo() = 0;

                /*! \brief This is optional function, that returns map of (Executive name, last extraction date)
                 * This function is for introspection proporses only.
                 *
                 * Backends that doesn't store this information may not implement this function. Default one will return
                 * empty map
                 */
                virtual QMap< QString, QDateTime > examinedExecutiveDates();

                /*! \brief This is optional function that return last extraction date for given Executive
                 * This function is for introspection proporses only.
                 *
                 * This function should return date of last extraction of Executive with given name. If Executive with given name
                 * has not been used, then invalid Date should be return.
                 * Backends that doesn't store this information may not implement this function. Default one will return
                 * invalid Executive.
                 */
                virtual QDateTime examinedDate(const QString & name);


                /*! \brief Clear all information about examined Executive
                         * This function should remove all information about examined Executive. After call to this function,
                         * the examinedExecutiveInfo() must return empty map. But if any other information is stored too, it
                         * could remain in storage.
                 */
                virtual void clearExaminedInfo() = 0;
                /*! \brief Unmark Executive with given name as examined
                 * This function mark Executive as not-examined. Version parameter is not necessary, because there must be
                 * only one version of the Executive marked as examined.
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
                virtual Nepomuk::Resource  resource() const ;
                /*! \brief Return the url of the resources of the ServiceDataBackend.
                         */
                virtual const QUrl & uri() const ;

                /*! \brief return names of properties where some usefull debugging information is stored
                 * This function return names of Qt properties. This names are
                 * implementation depended. For example SQL backend may return
                 * table name, and NRL backend - graph name
                         * \note This function is more for debugging/developing proposes. It allow introspection of the
                         * the backend.
                 */
                virtual QStringList serviceInfoPropertiesNames() const;

                virtual ~ServiceDataBackend();
            private:
                // Copying is forbidden
                ServiceDataBackend(const ServiceDataBackend &);
                ServiceDataBackend & operator=(const ServiceDataBackend &);
                class Private;
                Private * const d;
        };

    }
}

#endif
