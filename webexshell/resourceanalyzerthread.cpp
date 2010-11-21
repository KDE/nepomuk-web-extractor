/*
   This file is part of the Nepomuk KDE project.
   Copyright (C) 2010 Sebastian Trueg <trueg@kde.org>

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) version 3, or any
   later version accepted by the membership of KDE e.V. (or its
   successor approved by the membership of KDE e.V.), which shall
   act as a proxy defined in Section 6 of version 3 of the license.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with this library.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "resourceanalyzerthread.h"
#include "parameters.h"
#include "global.h"
#include "resourceanalyzerfactory.h"
#include "category.h"
#include "webextractor_plugin.h"
#include "datapp.h"
#include "resourceanalyzer.h"

#include <Soprano/QueryResultIterator>
#include <Soprano/StorageModel>

#include <Nepomuk/Resource>
#include <Nepomuk/ResourceManager>

#include <KTempDir>
#include <KDebug>
#include <KLocale>

using namespace Nepomuk::WebExtractor;

Q_DECLARE_METATYPE(Nepomuk::Resource)

ResourceAnalyzerThread::ResourceAnalyzerThread(QObject *parent)
    : QThread(parent)
{
    qRegisterMetaType<Nepomuk::Resource>();
}

void ResourceAnalyzerThread::run()
{
    //
    // Prepare settings
    //
    emit infoMessage(i18n("Preparing analyzer"));

    m_resourceQueue.clear();

    ExtractParameters params;
    foreach(const DataPPDescr & dppdescr, m_category->plugins()) {
        if(Nepomuk::WebExtractorPlugin* plugin = dppdescr.createPlugin()) {
            KConfigGroup * group = new KConfigGroup();
            params.addDataPP(new DataPPWrapper(plugin->getDataPP(QSharedPointer<KConfigGroup>(group)), dppdescr.identifier(), dppdescr.rank(), dppdescr.coff()));
        }
    }
    params.setUCrit(m_category->uCrit());
    params.setACrit(m_category->aCrit());
    params.setPluginSelectStep(m_category->pluginSelectionStep());

    KTempDir dbTempDir;
    Soprano::BackendSettings settings;
    settings << Soprano::BackendSetting(Soprano::BackendOptionStorageDir, dbTempDir.name());
    params.setBackendName("virtuoso");
    params.setBackendSettings(settings);


    //
    // Create Analyzer
    //
    ResourceAnalyzerFactory factory(params);
    ResourceAnalyzer* analyzer = factory.newAnalyzer();
    if(!analyzer) {
        kError() << "ResourceAnalyzerFactory failed to create Analyzer";
        return;
    }
    connect(analyzer, SIGNAL(analyzingFinished()), this, SLOT(slotAnalyzingFinished()));

    //
    // Do the actual work
    //
    if(m_url.isEmpty()) {
        kDebug() << "Analyzing:" << m_category->query();
        Soprano::QueryResultIterator it
                = Nepomuk::ResourceManager::instance()->mainModel()->executeQuery(m_category->query().toSparqlQuery(), Soprano::Query::QueryLanguageSparql);
        while(it.next()) {
            // FIXME: this is ugly - we should not cache all results!
            m_resourceQueue.enqueue(it[0].uri());
        }
    }
    else {
        kDebug() << "Analyzing:" << m_url;
        m_resourceQueue.enqueue(m_url);
    }

    if(!m_resourceQueue.isEmpty()) {
        Nepomuk::Resource res = m_resourceQueue.dequeue();
        emit analyzingResource(res);
        analyzer->analyze(res);

        //
        // start the event loop
        //
        exec();
    }

    // cleanup
    delete analyzer;
    qDeleteAll(params.plugins().values());
    qDeleteAll(params.plugins().keys());
}

void ResourceAnalyzerThread::start(Category* cat, const KUrl& optionalFile)
{
    m_category = cat;
    m_url = optionalFile;
    QThread::start();
}

void ResourceAnalyzerThread::slotAnalyzingFinished()
{
    kDebug();
    ResourceAnalyzer* analyzer = qobject_cast<ResourceAnalyzer*>(sender());
    Nepomuk::WebExtractor::DecisionList decisions = analyzer->decisions();
    if(!decisions.isEmpty())
        emit newDecisions(decisions);
    if(!m_resourceQueue.isEmpty()) {
        emit newDecisions(analyzer->decisions());

        Nepomuk::Resource res = m_resourceQueue.dequeue();
        emit analyzingResource(res);
        analyzer->analyze(res);
    }
    else {
        kDebug() << "All resources analyzed";
        exit();
    }
}
