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
#include "categoriespool.h"
#include "category.h"
#include <Soprano/StorageModel>
// use absolute path to prevent linking with libwebextractor/global.h
#include "libwebexsettings/global.h"

namespace NW = Nepomuk::WebExtractor;
namespace NQ = Nepomuk::Query;


Nepomuk::WebExtractorSettings::WebExtractorSettings():
    WebExtractorConfig(),
    m_globalTempDir(0),
    m_globalManager(0)
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
    foreach(const NW::ExtractParameters& ptr, m_parameters) {
        const NW::DataPPKeeper &  plugins = ptr.plugins();
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

    foreach(Category*  cat, Nepomuk::CategoriesPool::self()->categories()) {
        NW::ExtractParameters p;

        foreach(const DataPPDescr & dppdescr, cat->plugins()) {
            /*Load plugin with this name and parse it config*/
            NW::DataPP * dpp = 0 ;
            NW::DataPPWrapper * dppw = 0 ;
            double rank = dppdescr.rank();
            double coff = dppdescr.coff();
            const QString & dataPPName = dppdescr.identifier();

            if(dataPPName == "debug") {
                //This is predefined plugin
                dpp = new NW::DebugDataPP();
                //m_datapp.insert("debug",dpp);
            } else {
                // Create new DataPP
                dpp = DataPPConfig::dataPP(dataPPName);
                if(dpp) {
                    kDebug() << "Loaded DataPP " << dataPPName << "version: " << dpp->version();
                } else {
                    kError() << "Error while generating DataPP " << dataPPName;
                }
            }

            // Add DataPP to category parameters
            if(dpp) {
                dppw = new NW::DataPPWrapper(dpp, dataPPName, rank, coff);
                p.addDataPP(dppw);
                //m_datappwrappers.insert(pluginName,dppw);
            }

        }

        // TODO: use one setCategory method or put the Category in the ExtractParameters constructor
        p.setUCrit(cat->uCrit());
        p.setACrit(cat->aCrit());
        p.setPluginSelectStep(cat->pluginSelectionStep());
        int scheme = decisionsModelScheme();
        switch(scheme) {
        case EnumDecisionsModelScheme::Auto : /* User */
        case EnumDecisionsModelScheme::User : /* User */
        case EnumDecisionsModelScheme::Redland : { /* Redland */
            Soprano::BackendSettings settings;
            settings << Soprano::BackendOptionStorageMemory;
            p.setBackendName("redland");
            p.setBackendSettings(settings);
            break;
        }
        case EnumDecisionsModelScheme::Virtuoso: { /* Virtuoso */
            p.setDecisionsModel(m_globalStorageModel);
            break;
        }
        }


        NW::LaunchPolitics pol;
        switch(cat->pluginSelectionType()) {
        case(Category::Stepwise) : {
            pol = NW::StepWise;
            break;
        }
        case(Category::All) : {
            pol = NW::All;
            break;
        }
        }
        p.setLaunchPolitics(pol);
        p.setMergePolitics(NW::Highest);
        this->m_parameters.insert(cat->name(), p);
    }
}

int Nepomuk::WebExtractorSettings::maxPluginsLaunched(const QString & categoryName)
{

    int s =  max_plugins_launched_per_category();
    const Category* cat = Nepomuk::CategoriesPool::self()->categoryById(categoryName);
    if(cat->pluginSelectionType() == Category::All)
        if(s)
            return s;
        else
            return 0;
    else {
        int s2 = cat->pluginSelectionStep();
        return qMin(s, s2);
    }

}

int Nepomuk::WebExtractorSettings::maxResSimult(const QString & categoryName)
{
    return qMin(maxResSimultPerCategory(), Nepomuk::CategoriesPool::self()->categoryById(categoryName)->maxResSimult());
}

int Nepomuk::WebExtractorSettings::interval(const QString & categoryName)
{
    return Nepomuk::CategoriesPool::self()->categoryById(categoryName)->interval();
}

NW::ExtractParameters  Nepomuk::WebExtractorSettings::extractParameters(const QString categoryName) const
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
#warning Why not return the full query instead of just the term?
    return Nepomuk::CategoriesPool::self()->categoryById(categoryName)->query().term();
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
            QHash< QString, WebExtractor::ExtractParameters >::const_iterator it = conf.m_parameters.begin();
            it != conf.m_parameters.end();
            it++
        ) {
            dbg << "Category " << it.key() << '\n';
            dbg << it.value() << '\n';
        }
    }
    return dbg;
}

