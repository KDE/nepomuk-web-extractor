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

#ifndef NEPOMUK_DATAPP_H
#define NEPOMUK_DATAPP_H


#include "webextractor_kcm.h"
#include "webexsettings_export.h"
#include <QReadWriteLock>
#include <QSharedPointer>
#include <KConfigGroup>

namespace Nepomuk
{

    namespace WebExtractor
    {
        class Executive;
    }

    class WebExtractorPlugin;
    class WebExtractorPluginKCM;
    class DataPPPool;

    /*!\brief  This class represent the config object for Executive
     * It inherits from KConfigBase
     */
    class WEBEXSETTINGS_EXPORT DataPP 
    {
        public:
            //typedef KSharedConfig<DataPPBase> Ptr;

            DataPP(const QString & id);
            /*! \brief Return config file for this Executive
            */
            KConfigGroup config();

            /*! \brief Returns the config object with use settings
             * All settings of the Executive can be devided into 2 groups - the system Executive
             * settings ( source, and some other ) and user settings - aka source plugin dependent
             * settings. The config() returns whole config object  and userConfig() - only
             * part with user settings. You should prefer using userConfig() as this helps to
             * avoid hard-to-detect erros connected to accidentally overwriting system settings
             */
            KConfigGroup userConfig();

            virtual ~DataPP();
            /*! \brief Return true if there is a datapp with given name and config is valid
             */
            bool isValid() const;

            /*! \brief Return source plugin.
             * Attention! It will load libwebextractor and any other dependencies of this
             * plugin.
             * Return 0 if DataPP is invalid or some other error occur
             */
            WebExtractorPlugin * plugin();

            /*! \brief Return Executive
             * Generate and return Executive  using config object returned by userConfig() 
             * Return 0 if DataPP is invalid or some other error occur
             */
            WebExtractor::Executive * executive();

            /*! \brief Return KCM for the Executive
             * Return 0 if there is no KCM or DataPP is invalid. 
             * The default KCM is unique for plugin and thus shared across 
             * several Executive.
             * The returned KCM will be automatically switched to this DataPP.
             * If it was used somewhere else you are responsible for applying/discarding
             * changes of the previous Executive 
             * before calling this function. If you don't 
             * save changes, you loose them.
             * If you pass forceNew = true, then new instance of the KCM will
             * be created. It will not be cached for further use. 
             */
            WebExtractorPluginKCM::Ptr kcm(bool forceNew = false);

            /*! \brief Return source plugin name
             */
            QString source() const;
            void setSource( const QString & );

            /*! \brief  Return display name of the DataPP
             */
            QString displayName() const;
            void setDisplayName(const QString &);

            /*! \brief  Return display name of the DataPP
             */
            QString description() const;
            void setDescription(const QString &);

            /*! \brief  Return display name of the DataPP
             */
            QStringList categories() const;
            void setCategories(const QStringList &);


            /*! \brief Load plugin, create datapp and return it
             */
            static WebExtractor::Executive * executive(const QString & id);

            static DataPP* newDataPP();
            static void removeDataPP(const QString id);

            /*
            static QString path(const QString & id);
            static QString path();
            */

            void sync();
            void remove();


            friend class DataPPPool;
        private:
            class Private;
            Private * const d;
            //QString m_displayName;
            // FIXME Wrap into method
            static QString m_path;

            static QReadWriteLock & m_lock();

            static QHash< QString, Nepomuk::WebExtractor::Executive*> & m_executiveHash();

            //static QString filenameToId(const QString &);

            static KConfig* mainConfig();

            // This function add given string to the list of 'locked' id
            // These ids can not be returned as new id for DataPP
            /*
            static void lockId(const QString &);
            static void unlockId(const QString &);
            static bool isLocked(const QString &);
            static QSet<QString> & lockDb();
            */

    };

}

Q_DECLARE_METATYPE(Nepomuk::DataPP*);
#endif

