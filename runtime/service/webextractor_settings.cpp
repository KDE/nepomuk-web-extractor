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
#include "webextractor_settings.h"
#include <QtCore/QString>
#include <KDebug>
#include <KService>
#include <KStandardDirs>
#include <KServiceTypeTrader>
#include <KPluginFactory>
#include <KPluginLoader>
#include <QtGlobal>
#include "debug_datapp.h"
#include "datapp.h"
#include "datappwrapper.h"
#include "global.h"
#include "parameters.h"
#include "datapppool.h"
// use absolute path to prevent linking with libwebextractor/global.h
#include "libwebexsettings/global.h"
#include <nepomuk/queryserializer.h>

namespace NW = Nepomuk::WebExtractor;
Nepomuk::WebExtractorSettings::WebExtractorSettings():
    WebExtractorConfig(),
    m_globalTempDir(0)
{
    update();
}

Nepomuk::WebExtractorSettings::~WebExtractorSettings()
{
    clear();
}

void Nepomuk::WebExtractorSettings::clear()
{
    // Free all dataPP
    /*
    foreach( NW::DataPP * pp, m_datapp)
    {
    delete pp;
    }
    m_datapp.clear();
    */

    // Free all DataPPWrappers
    foreach(NW::ExtractParametersPtr ptr, m_parameters) {
        const NW::DataPPKeeper &  plugins = ptr->plugins();
        foreach(NW::DataPPWrapper * dppw, plugins) {
            delete dppw;
        }
    }
    m_parameters.clear();

    // Delete global storage model ( if any )
    if(m_globalManager) {
        m_globalManager->deleteInstance();
        Soprano::BackendSettings settings;
        settings << Soprano::BackendSetting(
                     Soprano::BackendOptionStorageDir,
                     m_globalTempDir->name()
                 );
        delete m_globalStorageModel;
        m_globalBackend->deleteModelData(settings);
        m_globalBackend = 0;
        m_globalStorageModel = 0;
        m_globalManager = 0;
    }


    //m_plugins.clear();
    delete m_globalTempDir;
    m_globalTempDir = 0;


}

void Nepomuk::WebExtractorSettings::update()
{

    clear();

    WebExtractorConfig::update();

    // Some preparation
    int scheme = decisionsModelScheme();
    bool forceDefault = false;
    switch(scheme) {
    case EnumDecisionsModelScheme::User : /* User */
    case EnumDecisionsModelScheme::Auto : /* User */
    case EnumDecisionsModelScheme::Redland : { /* Redland */
        break;
    }
    case EnumDecisionsModelScheme::Virtuoso: { /* Virtuoso */
        // We use one model for all analyzers in all categories
        const Soprano::Backend * b = Soprano::discoverBackendByName("virtuoso");
        if(!b) {  // check that this backend is available
            forceDefault = true;
            break;
        }
        m_globalBackend = b;
        m_globalTempDir = new KTempDir(KStandardDirs::locateLocal("tmp", "dmodel"));
        Soprano::BackendSettings globalVirtuosoSettings;
        globalVirtuosoSettings << Soprano::BackendSetting(
                                   Soprano::BackendOptionStorageDir,
                                   m_globalTempDir->name()
                               );
        m_globalStorageModel = b->createModel(globalVirtuosoSettings);
        if(!m_globalStorageModel) {  // check that model is successfuly created
            forceDefault = true;
            break;
        }
        m_globalManager = Nepomuk::ResourceManager::createManagerForModel(m_globalStorageModel);
        break;
    }
    }

    if(forceDefault) {
    }
    QStringList cats = WebExConfigBase::categories();
    foreach(const QString &  cat, cats) {
        WebExCategoryConfig * cfg = m_categories[cat];

        NW::ExtractParameters * p  = new NW::ExtractParameters;


        foreach(const DataPPDescr & dppdescr, cfg->plugins()) {
            /*Load plugin with this name and parse it config*/
            NW::DataPP * dpp = 0 ;
            NW::DataPPWrapper * dppw = 0 ;
            double rank = dppdescr.rank;
            double coff = dppdescr.coff;
            const QString & dataPPName = dppdescr.name;

            if(dataPPName == "debug") {
                //This is predefined plugin
                dpp = new NW::DebugDataPP();
                //m_datapp.insert("debug",dpp);
            } else {
                // Create new DataPP
                dpp = DataPPConfig::dataPP(dataPPName);
                if(dpp) {
                    kDebug() << "Loaded DataPP " << dataPPName << "version: " << dpp->pluginVersion();
                } else {
                    kError() << "Error while generating DataPP " << dataPPName;
                }
            }

            // Add DataPP to category parameters
            if(dpp) {
                dppw = new NW::DataPPWrapper(dpp, dataPPName, rank, coff);
                p->addDataPP(dppw);
                //m_datappwrappers.insert(pluginName,dppw);
            }

        }

        Q_CHECK_PTR(cfg);
        p->setUCrit(cfg->uCrit());
        p->setACrit(cfg->aCrit());
        p->setPluginSelectStep(cfg->pluginSelectStep());
        int scheme = decisionsModelScheme();
        switch(scheme) {
        case EnumDecisionsModelScheme::Auto : /* User */
        case EnumDecisionsModelScheme::User : /* User */
        case EnumDecisionsModelScheme::Redland : { /* Redland */
            Soprano::BackendSettings settings;
            settings << Soprano::BackendOptionStorageMemory;
            p->setBackendName("redland");
            p->setBackendSettings(settings);
            break;
        }
        case EnumDecisionsModelScheme::Virtuoso: { /* Virtuoso */
            p->setDecisionsModel(m_globalStorageModel);
            break;
        }
        }


        NW::WE::LaunchPolitics pol;
        switch(cfg->pluginSelectType()) {
        case(WebExCategory::EnumPluginSelectType::stepwise) : {
            pol = NW::WE::StepWise;
            break;
        }
        case(WebExCategory::EnumPluginSelectType::all) : {
            pol = NW::WE::All;
            break;
        }
        }
        p->setLaunchPolitics(pol);
        p->setMergePolitics(NW::WE::Highest);
        this->m_parameters.insert(cat, NW::ExtractParametersPtr(p));
    }
}

int Nepomuk::WebExtractorSettings::maxPluginsLaunched(const QString & categoryName)
{

    int s =  max_plugins_launched_per_category();
    WebExCategoryConfig * c = m_categories[categoryName];
    Q_CHECK_PTR(c);
    if(c->pluginSelectType() == WebExCategoryConfig::EnumPluginSelectType::all)
        if(s)
            return s;
        else
            return 0;
    else {
        int s2 = c->pluginSelectStep();
        return qMin(s, s2);
    }

}

int Nepomuk::WebExtractorSettings::maxResSimult(const QString & categoryName)
{

    int s =  maxResSimultPerCategory();
    WebExCategoryConfig * c = m_categories[categoryName];
    Q_CHECK_PTR(c);
    int s2 = c->maxResSimult();
    return qMin(s, s2);

}

int Nepomuk::WebExtractorSettings::interval(const QString & categoryName)
{

    /*
    int s =  maxResSimultPerCategory();
    if ( s)
    return s;
    else {
    WebExCategory * c = m_categories[categoryName];
    Q_CHECK_PTR(c);
    return c->maxResSimult();
    }*/
    WebExCategoryConfig * c = m_categories[categoryName];
    Q_CHECK_PTR(c);
    return c->interval();

}

NW::ExtractParametersPtr  Nepomuk::WebExtractorSettings::extractParameters(const QString categoryName) const
{
    //return NW::ExtractParametersPtr(m_parameters[categoryName].data());
    if(!m_parameters.contains(categoryName)) {
        kDebug() << "No such category";
    }
    return m_parameters[categoryName];

}

#if 0
DataPPKeeper & Nepomuk::WebExtractorSettings::datapp(const QString categoryName)
{
    /*
    WebExCategory * c = m_categories[categoryName];
    Q_CHECK_PTR(c);
    return c->datapp();
    */
}
#endif

bool Nepomuk::WebExtractorSettings::isOptimizedForNepomuk(const QString & categoryName)
{
    return (examinedDataPPStorage() == EnumExaminedDataPPStorage::Nepomuk);
}

NQ::Term Nepomuk::WebExtractorSettings::query(const QString categoryName)
{
    WebExCategoryConfig * c = m_categories[categoryName];
    Q_CHECK_PTR(c);
    return NQ::parseTerm(c->query());
}

QString Nepomuk::WebExtractorSettings::queryPrefix(const QString categoryName)
{
    WebExCategoryConfig * c = m_categories[categoryName];
    Q_CHECK_PTR(c);
    return c->queryPrefix();
}


QDebug Nepomuk::operator<<(QDebug dbg,  const WebExtractorSettings & conf)
{
    dbg << static_cast<const WebExtractorConfig &>(conf);

#if 0
    QStringList cats = conf.categories();
    if(cats.size() > 0) {
        dbg << cats.size() << " Categories:";
        foreach(const QString & cat, cats)
        //foreach( WebExCategory* cat, conf.m_categories)
        {
            dbg << *(conf.m_parameters[cat]);
            //dbg<<"wtf";
            //dbg<<cat->query();
        }
    } else {
        dbg << "Config has no category enabled";
    }
#endif
    dbg << "Total plugins loaded: " << GlobalSettings::pluginCount() << '\n';
    dbg << "Total DataPP loaded: " << DataPPConfig::dataPPCount() << '\n';
    if(conf.categories().size() > 0) {
        dbg << "Parameters per category" << '\n';
        for(
            QHash< QString, WebExtractor::ExtractParametersPtr >::const_iterator it = conf.m_parameters.begin();
            it != conf.m_parameters.end();
            it++
        ) {
            dbg << "Category " << it.key() << '\n';
            dbg << *(it.value()) << '\n';
        }
    }
    return dbg;
}

