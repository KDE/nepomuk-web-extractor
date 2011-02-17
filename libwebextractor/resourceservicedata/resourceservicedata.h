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
                ResourceServiceData(const Nepomuk::Resource & res, ResourceServiceDataManager * manager);
                ResourceServiceData(const Nepomuk::Resource & res);
                ResourceServiceData(const ResourceServiceData & arg);
                const ResourceServiceData & operator= (const ResourceServiceData & rhs);
                ~ResourceServiceData();
                /*! \brief Add/Update information about examined Executive ( name, version)
                 */
                void setExaminedExecutiveInfo(const QString & dataPPName, int  dataPPVersion) ;

                /*! \brief Return map (name, version ) about all examined Executive
                 */
                QMap< QString, int > examinedExecutiveInfo();

                /*! \brief Return true if all given Executive has been examined for the resource
                 */
                bool checkExaminedExecutiveInfo(const QMap< QString, int> & dataPP) const;
                bool checkExaminedExecutiveInfo(const QString  & name) const;
                bool checkExaminedExecutiveInfo(const QString  & name, int version) const;

                /*! \brief Clear all information about examined Executive
                 */
                void clearExaminedInfo() ;
                void clearExaminedInfo(const QString & dataPPName) ;
                /*! \brief Clear all webextrator service information about this resource
                 * This function will clear examined info ( can be done directly via clearExaminedInfo() ),
                 * and all other stored service information
                 * This function is more for feature releases
                 */
                void clearServiceInfo();


                QMap< QString, QDateTime > examinedExecutiveDates();
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
