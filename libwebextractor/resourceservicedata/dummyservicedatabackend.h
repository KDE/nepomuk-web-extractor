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

#ifndef __dummy_servicedata_backend_h_
#define __dummy_servicedata_backend_h_

#include "servicedatabackend.h"
#include "servicedatabackendfactory.h"

namespace Nepomuk
{
    namespace WebExtractor
    {
        /*! \brief This is interface class for storing information about examined Executive
         * Different storages will be available - SQL, Nepomuk, etc. Backend is bound to
         * resource. For each new Nepomuk::Resource object new instance of backend will
         * be created
         */
        class WEBEXTRACTOR_EXPORT DummyServiceDataBackend : public ServiceDataBackend
        {
                Q_OBJECT;
            public:
		typedef ServiceDataBackendFactoryTemplate<DummyServiceDataBackend> Factory;
		DummyServiceDataBackend(const Nepomuk::Resource & res);
                /*! \brief Add/Update information about examined Executive ( name, version)
		 * Do nothing
		 * \param dataPPName Name of the Executive
		 * \param dataPPVersion Version of the Executive
                 */
                virtual void setExaminedExecutiveInfo(const QString & dataPPName, int dataPPVersion, const QDateTime & date = QDateTime());

                /*! \brief This function checks that all given Executive has been examined for the resource.
		 * return false
                 */
                virtual bool checkExaminedExecutiveInfo(const QMap< QString, int> & dataPPInfo) ;
		virtual bool checkExaminedExecutiveInfo( const QString & name );
		virtual bool checkExaminedExecutiveInfo( const QString & name, int version );
                /*! \brief Return map (name, version ) about all examined Executive
		 * \return Return the empty map
                 */
                virtual QMap< QString, int > examinedExecutiveInfo() ;



                /*! \brief Clear all information about examined Executive
		 * Do nothing.
                 */
                virtual void clearExaminedInfo() ;
                /*! \brief Unmark Executive with given name as examined
                 * This function mark Executive as not-examined. Version parameter is not necessary, because there must be
                 * only one version of the Executive marked as examined.
                 */
                virtual void clearExaminedInfo(const QString & name) ;



        };


	

    }
}

#endif
