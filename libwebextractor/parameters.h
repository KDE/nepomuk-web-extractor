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

#ifndef _WEBEXTRCT_PARAMETERS
#define _WEBEXTRCT_PARAMETERS

#include <QtCore/QObject>
#include <QtCore/QtDebug>
#include <QtCore/QSharedPointer>
#include <QtCore/QSharedDataPointer>
#include "global.h"
#include "webextractor_export.h"
#include "datappwrapper.h"
#include <Soprano/Backend>
#include <Soprano/Global>
#include <Nepomuk/ResourceManager>
#include "resourceservicedatamanager.h"
#include <Soprano/BackendSettings>


namespace Nepomuk
{
    namespace WebExtractor
    {
        class WEBEXTRACTOR_EXPORT ExtractParameters
        {
            public:
                ExtractParameters();
                ~ExtractParameters();
                ExtractParameters(const ExtractParameters &);
                const ExtractParameters & operator=(const ExtractParameters & rhs);


                WE::MergePolitics mergePolitics() const;
                void setMergePolitics(WE::MergePolitics val) ;

                unsigned int pluginSelectStep() const;
                void setPluginSelectStep(unsigned int step);

                WE::LaunchPolitics launchPolitics() const;
                void setLaunchPolitics(WE::LaunchPolitics politics);

                double aCrit() const;
                void setACrit(double);

                double uCrit() const;
                void setUCrit(double);

                /*
                double scaleCoff(DataPP*)  const;
                void setScaleCoff(DataPP*, double) ;
                */

                const DataPPKeeper & plugins() const;
                void addDataPP(DataPPWrapper *) ;

                bool hasAnyDataPP() const;
                int dataPPCount() const;

                QString backendName() const;
                void setBackendName(const QString &);

                Soprano::BackendSettings backendSettings() const;
                void setBackendSettings(const Soprano::BackendSettings &);

                /*! \brief Return true if backend->deleteModelData() will be called on the model of each ResourceAnalyzer
                 * Each new ResourceAnalyzer is given a model. This model can be generated for each ResourceAnalyzer or
                 * one model for all ResourceAnalyzers (spawned by common ResourceAnalyzerFactory ) can be used. If you
                 * use in-memory model ( and this is default variant ) it is prefered to generate new model for each ResourceAnalyzer.
                 * But if you use non-in-memory model, then some files will be generated. If you want this files to be automatically removed
                 * after ResourceAnalyzer is not necessary any more, then set this flag to true. Other way you will have to remove them by hand.
                 */
                bool autoDeleteModelData() const;
                void setAutoDeleteModelData(bool val);

                /*
                bool forceModelStorageDir() const;
                void setForceModelStorageDir();
                */
                /*! \brief Instead of creating new model for each ResourceAnalyzer, the shared model for all of them can be used.
                 * To do this, you should provide a Model for Decisions. In this case,
                 * the backendSettings(), backendName(), autoDeleteModelData() parameters
                 *  will be ignored.  The web extractor system will not take ownership
                 *  on the given manager and will not delete or change it in any way.
                 *  If you want to disable this setting, then pass Null as manager
                 */
                Soprano::Model * decisionsModel() const;
                void setDecisionsModel(Soprano::Model * decisionsModel);

                /*! \brief Set resource service data manager
                 * ResourceServiceData is responsible for storing information about examined DataPP
                 * for resource
                 */
                ResourceServiceDataManager * resourceServiceDataManager() const;
                /*! \brief Set resource service data manager.
                 * \param manager New manager or 0 to reset to default one
                 */
                void setResourceServiceDataManager(ResourceServiceDataManager * manager);

                QMap<QString, float> dataPPInfo() const;

                friend QDebug operator<<(QDebug dbg,  const ExtractParameters & cat);

            private:
                class Private;
                QSharedDataPointer<Private> d;
        };
        typedef QSharedPointer< const ExtractParameters> ExtractParametersPtr;
        WEBEXTRACTOR_EXPORT QDebug operator<<(QDebug dbg,  const ExtractParameters & cat);
    }
}
#endif
