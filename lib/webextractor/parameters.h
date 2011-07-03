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

#ifndef _WEBEXTRCT_PARAMETERS
#define _WEBEXTRCT_PARAMETERS

#include <QtCore/QObject>
#include <QtCore/QtDebug>
#include <QtCore/QSharedDataPointer>
#include "global.h"
#include "webextractor_export.h"
#include "executivewrapper.h"
#include <Soprano/Backend>
#include <Soprano/Global>
#include <Nepomuk/ResourceManager>
#include "resourceservicedatamanager.h"
#include <Soprano/BackendSettings>

#include <decision/global.h>


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


                bool isValid() const;

                Decision::MergePolitics mergePolitics() const;
                void setMergePolitics(Decision::MergePolitics val) ;

                unsigned int pluginSelectStep() const;
                void setPluginSelectStep(unsigned int step);

                LaunchPolitics launchPolitics() const;
                void setLaunchPolitics(LaunchPolitics politics);

                double aCrit() const;
                void setACrit(double);

                double uCrit() const;
                void setUCrit(double);

                /*
                double scaleCoff(Executive*)  const;
                void setScaleCoff(Executive*, double) ;
                */

                const ExecutiveKeeper & plugins() const;
                void addExecutive(ExecutiveWrapper *) ;

                bool hasAnyExecutive() const;
                int executiveCount() const;

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

                /*! \brief Enable automatic ontology management
                 * If enabled, then ontologies will be loaded and  updated automatically.
                 * This option is usefull only if storage model for Decisions is set with
                 * setDecisionsModel() call. Ontologies <b>must</b> be loaded to the model
                 * where Decisions are stored. If you don't want to do this by hand, then
                 * set this option to true.
                 * If decisions storage model is generated automatically, then
                 * this option is always enabled
                 */
                bool autoManageOntologies() const;
                void setAutoManageOntologies(bool val);

                /*
                bool forceModelStorageDir() const;
                void setForceModelStorageDir();
                */
                Soprano::Model * decisionsModel() const;
                /*! \brief Instead of creating new model for each ResourceAnalyzer, the shared model for all of them can be used.
                 * To do this, you should provide a Model for Decisions. In this case,
                 * the backendSettings(), backendName(), autoDeleteModelData() parameters
                 *  will be ignored.  The web extractor system will not take ownership
                 *  on the given manager and will not delete or change it in any way.
                 *  If you want to disable this setting, then pass Null as manager
                         *  \param decisionsModel Model to store Decisions in
                         *  \param autoManageOntologies Set this flag to true, if you want that
                         *  ontologies were loaded to the model automatically. You can change you
                         *  choice later with setAutoManageOntologies() method.
                         *  \sa setAutoManageOntologies
                 */
                void setDecisionsModel(Soprano::Model * decisionsModel, bool autoManageOntologies = true);

                /*! \brief Set resource service data manager
                 * ResourceServiceData is responsible for storing information about examined Executive
                 * for resource
                 */
                ResourceServiceDataManager * resourceServiceDataManager() const;
                /*! \brief Set resource service data manager.
                 * \param manager New manager or 0 to reset to default one
                 */
                void setResourceServiceDataManager(ResourceServiceDataManager * manager);

                QMap<QString, int> dataPPInfo() const;

                friend QDebug operator<<(QDebug dbg,  const ExtractParameters & cat);

            private:
                class Private;
                QSharedDataPointer<Private> d;
        };
        WEBEXTRACTOR_EXPORT QDebug operator<<(QDebug dbg,  const ExtractParameters & cat);
    }
}
#endif
