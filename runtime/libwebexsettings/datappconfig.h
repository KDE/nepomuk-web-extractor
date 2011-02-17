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

#ifndef NEPOMUK_DATAPPCONFIG_H
#define NEPOMUK_DATAPPCONFIG_H


#include "datappconfigbase.h"
#include <QReadWriteLock>
#include <QSharedPointer>

class KConfigBase;

namespace Nepomuk
{

    namespace WebExtractor
    {
        class DppExecutive;
    }

    class WebExtractorPlugin;
    class WebExtractorPluginKCM;

    /*!\brief  This class represent the config object for DppExecutive
     * It inherits from KConfigBase
     */
    class DppExecutiveConfig : public DppExecutiveConfigBase
    {
        public:
            //typedef KSharedConfig<DppExecutiveConfigBase> Ptr;

            DppExecutiveConfig(const QString &);
            /*! \brief Return config file for this DppExecutive
            */
            KSharedConfigPtr config();

            /*! \brief Returns the config object with use settings
             * All settings of the DppExecutive can be devided into 2 groups - the system DppExecutive
             * settings ( source, and some other ) and user settings - aka source plugin dependent
             * settings. The config() returns whole config object  and userConfig() - only
             * part with user settings. You should prefer using userConfig() as this helps to
             * avoid hard-to-detect erros connected to accidentally overwriting system settings
             */
            QSharedPointer<KConfigBase> userConfig();

            virtual ~DppExecutiveConfig();
            /*! \brief Return true if there is a datapp with given name and config is valid
             */
            bool isValid() const;

            /*! \brief Return source plugin.
             * Attention! It will load libwebextractor and any other dependencies of this
             * plugin.
             * Return 0 if DppExecutiveConfig is invalid or some other error occur
             */
            WebExtractorPlugin * plugin();

            /*! \brief Return DppExecutive
             * Generate and return DppExecutive  using config object returned by userConfig() 
             * Return 0 if DppExecutiveConfig is invalid or some other error occur
             */
            WebExtractor::DppExecutive * dataPP();

            /*! \brief Return KCM for the DppExecutive
             * Return 0 if there is no KCM or DppExecutiveConfig is invalid. 
             * The KCM is unique for plugin and thus shared across several DppExecutive.
             * The returned KCM will be automatically switched to this DppExecutiveConfig.
             * If it was used somewhere else you are responsible for applying/discarding
             * changes of the previous DppExecutive before calling this function. If you don't 
             * save changes, you loose them.
             */
            WebExtractorPluginKCM * kcm();

            /*! \brief Return source plugin name
             */
            //QString source() const;

            /*! \brief Load plugin, create datapp and return it
             */
            static WebExtractor::DppExecutive * dataPP(const QString & name);

            static int dataPPCount();
        private:
            QString m_name;
            //QString m_displayName;
            // FIXME Wrap into method
            static QString path;

            static QReadWriteLock & m_lock();

            static QHash< QString, Nepomuk::WebExtractor::DppExecutive*> & m_datapp();
    };

}

#endif

