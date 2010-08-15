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

#ifndef _NEPOMUK_WEBEXTRCT_RESOURCE_ANALYZER_FACTORY_H_
#define _NEPOMUK_WEBEXTRCT_RESOURCE_ANALYZER_FACTORY_H_

#include <QtCore/QObject>
#include "resourceanalyzer.h"
#include "webextractor_export.h"
#include "datapp.h"
#include "parameters.h"
#include <Nepomuk/Resource>

namespace Nepomuk
{
    class OntologyLoader;

    namespace WebExtractor
    {
        class WEBEXTRACTOR_EXPORT ResourceAnalyzerFactory: public QObject
        {
                Q_OBJECT;
            public:
                ResourceAnalyzerFactory(
                    ExtractParametersPtr extractParams,
                    QObject * parent = 0
                );
                /*! \brief Create new ResourceAnalyzer
                 * If settings that were passed are incorrect, or can not be
                 * satisfied, then NULL will be returned. So you should check
                 * for it.
                 */
                ResourceAnalyzer * newAnalyzer();
                /*! \brief Create new ResourceAnalyzer and bound it to resource
                 * This is an overload, provided for convinience.
                 */
                ResourceAnalyzer * newAnalyzer(const Resource & res);
                /*! \brief Delete ResourceAnalyzer.
                 * TODO Delete this method as unnecessary.
                 */
                void deleteAnalyzer(ResourceAnalyzer * res);
                /*! \brief Return backend, that will be used for creating models for ResourceAnalyzer.
                 * Each ResourceAnalyzer is given a model. You can select backend, used for
                 * it with ExtractParameters( passed to the constructor ). But if your
                 * settings can not be satisfied, then some fallbacks will be used.
                 * Use this method to get the real backend that was used to create
                 * ResourceAnalyzers.
                 *
                 * ResourceAnalyzer will not delete model data (
                 * Backend::deleteModelData). You are responsible for this.
                 */
                const Soprano::Backend * backend() const;

                /*! \brief Return backend settings that are used to create new model
                 * As backend() method, this method return the real settings that are
                 * used to creaet a new model. They may differ from settings, that you
                 * have passed in extractParams
                 */
                Soprano::BackendSettings backendSettings() const;
            private:
                DataPPKeeper  m_dataPPKeeper;
                LaunchPolitics m_launchPolitics;
                MergePolitics m_mergePolitics;
                unsigned int m_step;
                double m_acrit;
                double m_ucrit;
                bool m_autoDeleteModelData;
                bool m_autoManageOntologies;

                /*! \brief The backend that support in-memory models and will be used for storing decisions
                 * Currently it is redland, but in future releases I will add some
                 * functions to allow user select different backends
                 */
                //const Soprano::Backend * usedBackend();
                // This is function pointer that is used for introspection of analyzer between
                // iterations ( when politics is Iterative)
                //void (*debug_interrupter)();

                // The folowing 2 variables are used to store Decisions while processing
                // them. This is necessary because we need some sparql support and can't
                // use Soprano::Graph.
                // When starting analyzing new resource the model must be totall reset.
                // ResourceManager is manager for model. It is not very necessary now, but
                // will be defenetely used in the future.

                // Model for storing Decisions in memory
                //Soprano::StorageModel * decisionsStorageModel;
                // ResourceManager that manages this model. This pointer is non-null
                // only if extractParams->manager() was non-null.
                // ResourceManager * decisionsResourceManager;
                Soprano::Model * decisionsMainModel;
                OntologyLoader * decisionsMainModelOntologyLoader;
                const Soprano::Backend * m_backend;
                Soprano::BackendSettings m_backendSettings;
        };
    }
}

#endif
