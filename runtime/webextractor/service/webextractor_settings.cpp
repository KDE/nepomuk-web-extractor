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
#include "debugexecutive.h"
#include "datapp.h"
#include "executivewrapper.h"
#include "global.h"
#include "parameters.h"
#include "datapppool.h"
#include "categoriespool.h"
#include "category.h"
#include <decision/global.h>
// use absolute path to prevent linking with libwebextractor/global.h
#include "webexsettings_global.h"


namespace NW = Nepomuk::WebExtractor;
namespace NQ = Nepomuk::Query;
namespace ND = Nepomuk::Decision;


Nepomuk::WebExtractorSettings::WebExtractorSettings():
    WebExtractorConfig()
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

    // Free all ExecutiveWrappers
    foreach(const NW::ExtractParameters& ptr, m_parameters) {
        const NW::ExecutiveKeeper &  plugins = ptr.plugins();
        foreach(NW::ExecutiveWrapper * dppw, plugins) {
            delete dppw;
        }
    }
    m_parameters.clear();

}

void Nepomuk::WebExtractorSettings::update()
{

    clear();

    // Some preparation
    foreach(Category*  cat, Nepomuk::CategoriesPool::self()->categories()) {
        NW::ExtractParameters p;

        foreach(const DataPPDescr & dppdescr, cat->plugins()) {
            /*Load plugin with this name and parse it config*/
            NW::Executive * dpp = 0 ;
            NW::ExecutiveWrapper * dppw = 0 ;
            double rank = dppdescr.rank();
            double coff = dppdescr.coff();
            const QString & dataPPId = dppdescr.identifier();

            if(dataPPId == "debug") {
                //This is predefined plugin
                dpp = new NW::DebugExecutive();
                //m_datapp.insert("debug",dpp);
            } else {
                // Create new DataPP
                dpp = DataPP::executive(dataPPId);
                if(dpp) {
                    kDebug() << "Loaded DataPP " << dataPPId << "version: " << dpp->version();
                } else {
                    kError() << "Error while generating DataPP " << dataPPId;
                }
            }

            // Add DataPP to category parameters
            if(dpp) {
                dppw = new NW::ExecutiveWrapper(dpp, dataPPId, rank, coff);
                p.addExecutive(dppw);
                //m_datappwrappers.insert(pluginName,dppw);
            }

        }

        // TODO: use one setCategory method or put the Category in the ExtractParameters constructor
        p.setUCrit(cat->uCrit());
        p.setACrit(cat->aCrit());
        p.setPluginSelectStep(cat->pluginSelectionStep());

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
        p.setMergePolitics(ND::Highest);
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
    dbg << "Total DataPP loaded: " << DataPPPool::availableDataPPCount() << '\n';
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

