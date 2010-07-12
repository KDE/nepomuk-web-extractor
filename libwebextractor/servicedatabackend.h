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
                 */
                virtual void setExaminedDataPPInfo(const QString & dataPPName, const QString & dataPPVersion) = 0;

                /*! \brief Return map (name, version ) about all examined DataPP
                 */
                virtual QMap< QString, QString > examinedDataPPInfo() = 0;

                /*! \brief Clear all information about examined DataPP
                 */
                virtual void clearExaminedIfno() = 0;
                virtual void clearExaminedIfno(const QString & name) = 0;

                /*! \brief Clear all webextrator service information about this resource
                 * This function will clear examined info ( can be done directly via clearExaminedIfno() ),
                 * and all other stored service information
                 * This function is more for feature releases
                 */
                virtual void clearServiceInfo();


                virtual Nepomuk::Resource  resource() const = 0;
                virtual const QUrl & uri() const = 0 ;

                /*! \brief return names of properties where some usefull debugging information is stored
                 * This function return names of Qt properties. This names are
                 * implementation depended. For example SQL backend may return
                 * table name, and NRL backend - graph name
                 */
                virtual QStringList serviceInfoPropertiesNames() const;

                virtual ~ServiceDataBackend();
        };

    }
}

#endif
