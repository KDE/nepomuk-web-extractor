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

#ifndef NEPOMUK_DATAPPCONFIG_H
#define NEPOMUK_DATAPPCONFIG_H


#include "datappconfigbase.h"
#include "webextractor_plugin.h"
#include <QReadWriteLock>

namespace Nepomuk
{

    namespace WebExtractor
    {
        class DataPP;
    }

    class DataPPConfig : public DataPPConfigBase
    {
        public:

            DataPPConfig(const QString &);
            /*! Return config file for this DataPP
             */
            KSharedConfigPtr config();

            virtual ~DataPPConfig();
            /*! \brief Return true if there is a datapp with given name and config is valid
             */
            bool isValid() const;

            /*! \brief Return source plugin.
             * Attention! It will load libwebextractor and any other dependencies of this
             * plugin.
             * Return 0 if DataPPConfig is invalid or some other error occur
             */
            WebExtractorPlugin * plugin();

            /*! \brief Return DataPP
             * Generate and return DataPP  using config file returned by config() as parameter
             * Return 0 if DataPPConfig is invalid or some other error occur
             */
            WebExtractor::DataPP * dataPP();

            /*! \brief Return source plugin name
             */
            //QString source() const;

            /*! \brief Load plugin, create datapp and return it
             */
            static WebExtractor::DataPP * dataPP(const QString & name);

            static int dataPPCount();
        private:
            QString m_name;
            // FIXME Wrap into method
            static QString path;

            static QReadWriteLock & m_lock();

            static QHash< QString, Nepomuk::WebExtractor::DataPP*> & m_datapp();
    };

}

#endif

