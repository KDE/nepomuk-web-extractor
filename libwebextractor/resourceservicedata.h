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

#ifndef __webextractor__resoucreservicedata_h_
#define __webextractor__resoucreservicedata_h_

#include <QSharedDataPointer>
#include <Nepomuk/Resource>
#include "resourceservicedatamanager.h"
#include "webextractor_export.h"

namespace Nepomuk
{
    namespace WebExtractor
    {
        /*! \brief This class is proxy for any resource that handles metadata for webextractor
         * This class manage graphs and properties where
         * webextractor store it's data - list of examined plugins, etc
         */
        class WEBEXTRACTOR_EXPORT ResourceServiceData
        {
            public:
                ResourceServiceData();
                ResourceServiceData(Nepomuk::Resource & res, ResourceServiceDataManager * manager);
                ResourceServiceData(Nepomuk::Resource & res);
                ResourceServiceData(ResourceServiceData & arg);
                const ResourceServiceData & operator= (const ResourceServiceData & rhs);
                ~ResourceServiceData();
                /*! \brief Add/Update information about examined DataPP ( name, version)
                 */
                void setExaminedDataPPInfo(const QString & dataPPName, float  dataPPVersion) ;

                /*! \brief Return map (name, version ) about all examined DataPP
                 */
                QMap< QString, float > examinedDataPPInfo();

                /*! \brief Return true if all given DataPP has been examined for the resource
                 */
                bool checkExaminedDataPPInfo(const QMap< QString, float> & dataPP) const;

                /*! \brief Clear all information about examined DataPP
                 */
                void clearExaminedInfo() ;
                void clearExaminedInfo(const QString & dataPPName) ;
                /*! \brief Clear all webextrator service information about this resource
                 * This function will clear examined info ( can be done directly via clearExaminedInfo() ),
                 * and all other stored service information
                 * This function is more for feature releases
                 */
                void clearServiceInfo();


                QMap< QString, QDateTime > examinedDataPPDates();
                QDateTime examinedDate(const QString & name);
                bool isValid() const;

                QStringList serviceInfoPropertiesNames() const;
                QVariant property(const char * name) const;
            private:
                class Private;
                // TODO May be it is wiser to user simple pointer instead of
                // shared pointer
                QSharedDataPointer<Private>  d;
        };
    }
};
#endif
