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


#include "resourceanalyzerfactory.h"
#include <KDebug>
#include <Soprano/Global>
#include <Nepomuk/Tag>

namespace NW = Nepomuk::WebExtractor;
Nepomuk::WebExtractor::ResourceAnalyzerFactory::ResourceAnalyzerFactory(
    ExtractParametersPtr extractParams,
    /*
    ResourceAnalyzer::LaunchPolitics launchPolitics,
    DecisionList::MergePolitics mergePolitics,
    unsigned int step,
    double acrit,
    double ucrit,
    */
    QObject * parent
):
    QObject(parent),
    m_launchPolitics(WE::StepWise),
    m_mergePolitics(WE::Highest),
    m_step(10),
    decisionsResourceManager(0),
    m_backend(0)
{
    if(extractParams.isNull()) {
        kDebug() << "Parameters are null. Reseting to defaults";
        m_backend = Soprano::discoverBackendByFeatures(Soprano::BackendFeatureStorageMemory);
    } else {
        m_dataPPKeeper = extractParams->plugins();
        m_step = extractParams->pluginSelectStep();
        m_launchPolitics = extractParams->launchPolitics();
        m_mergePolitics = extractParams->mergePolitics() ;
        m_ucrit = extractParams->uCrit() ;
        m_acrit = extractParams->aCrit() ;
        // We need backend only if ResourceManager is not provided.
        this->decisionsResourceManager = extractParams->manager();
        if(!this->decisionsResourceManager) {
            QString backendName = extractParams->backendName();
            if(backendName.isEmpty()) {
                m_backend = Soprano::discoverBackendByFeatures(Soprano::BackendFeatureStorageMemory);
            } else { // Trying to found specified backend
                m_backend = Soprano::discoverBackendByName(backendName);
                // If not found, then trying to found default backend
                if(!m_backend)
                    m_backend = Soprano::discoverBackendByFeatures(Soprano::BackendFeatureStorageMemory);
            }

            //kDebug() << "ACrit: " << m_acrit;
            //kDebug() << "UCrit: " << m_ucrit;
            m_backendSettings = extractParams->backendSettings();
        }

    }

    //decisionsStorageModel = 0;
    //decisionsResourceManager = 0;
    this->m_autoDeleteModelData = extractParams->autoDeleteModelData();
}

Nepomuk::WebExtractor::ResourceAnalyzer * Nepomuk::WebExtractor::ResourceAnalyzerFactory::newAnalyzer()
{
    DecisionFactory * fct = 0;
    if(this->decisionsResourceManager) {
        fct = new DecisionFactory(m_ucrit, m_acrit, decisionsResourceManager, false, 0);
    } else {
        const Soprano::Backend * b = m_backend;
        if(b) {
            Nepomuk::ResourceManager * adrm = 0;
            // Initialize model
            Soprano::StorageModel * decisionsStorageModel = b->createModel(m_backendSettings);
            if(!decisionsStorageModel) {
                return 0;
            }

            // Initialize ResourceManager
            adrm = ResourceManager::createManagerForModel((Soprano::Model*)(decisionsStorageModel));

            // Check the model
            /*
            #warning DISABLE THIS CODE! IT POLLUTES MODEL
            Nepomuk::Tag t(QString("test_tag"),adrm);
            t.setDescription("tag to test redland model");
            kDebug() << "Tag successfuly generated: " <<t.resourceUri();
            QUrl uu = adrm->generateUniqueUri("res");
            */
            // Set this manager to the factory
            fct = new DecisionFactory(m_ucrit, m_acrit, adrm, m_autoDeleteModelData, decisionsStorageModel, m_backendSettings);
            //fct->setResourceManager(decisionsResourceManager);
        } else {
            return 0;
        }
    }

    fct->setThreshold(m_ucrit);
    return new Nepomuk::WebExtractor::ResourceAnalyzer(
               m_dataPPKeeper,
               fct,
               this->m_mergePolitics,
               this->m_launchPolitics,
               m_acrit,
               m_ucrit,
               this->m_step
           );
}

Nepomuk::WebExtractor::ResourceAnalyzer * Nepomuk::WebExtractor::ResourceAnalyzerFactory::newAnalyzer(const Nepomuk::Resource & res)
{
    ResourceAnalyzer * resanal = newAnalyzer();
    if(resanal) {
        resanal->setResource(res);
    }
    return resanal;
}

void Nepomuk::WebExtractor::ResourceAnalyzerFactory::deleteAnalyzer(Nepomuk::WebExtractor::ResourceAnalyzer * res)
{
    if(res) res->deleteLater();
}

const Soprano::Backend * NW::ResourceAnalyzerFactory::backend() const
{
    return m_backend;
}

Soprano::BackendSettings NW::ResourceAnalyzerFactory::backendSettings() const
{
    return m_backendSettings;
}

/*
const Soprano::Backend * NW::ResourceAnalyzerFactory::usedBackend()
{
    //Soprano::BackendSettings settings;
    //settings << Soprano::BackendOptionStorageMemory;
    static const Soprano::Backend * backend = Soprano::discoverBackendByFeatures(Soprano::BackendFeatureStorageMemory);
    return backend;
}
*/
