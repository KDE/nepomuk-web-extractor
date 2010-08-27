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

#ifndef __dummy_servicedata_backend_h_
#define __dummy_servicedata_backend_h_

#include "servicedatabackend.h"
#include "servicedatabackendfactory.h"

namespace Nepomuk
{
    namespace WebExtractor
    {
        /*! \brief This is interface class for storing information about examined DataPP
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
                /*! \brief Add/Update information about examined DataPP ( name, version)
		 * Do nothing
		 * \param dataPPName Name of the DataPP
		 * \param dataPPVersion Version of the DataPP
                 */
                virtual void setExaminedDataPPInfo(const QString & dataPPName, int dataPPVersion, const QDateTime & date = QDateTime());

                /*! \brief This function checks that all given DataPP has been examined for the resource.
		 * return false
                 */
                virtual bool checkExaminedDataPPInfo(const QMap< QString, int> & dataPPInfo) ;
		virtual bool checkExaminedDataPPInfo( const QString & name );
		virtual bool checkExaminedDataPPInfo( const QString & name, int version );
                /*! \brief Return map (name, version ) about all examined DataPP
		 * \return Return the empty map
                 */
                virtual QMap< QString, int > examinedDataPPInfo() ;



                /*! \brief Clear all information about examined DataPP
		 * Do nothing.
                 */
                virtual void clearExaminedInfo() ;
                /*! \brief Unmark DataPP with given name as examined
                 * This function mark DataPP as not-examined. Version parameter is not necessary, because there must be
                 * only one version of the DataPP marked as examined.
                 */
                virtual void clearExaminedInfo(const QString & name) ;



        };


	

    }
}

#endif
