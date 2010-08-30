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

#include "parameters.h"
#include <KDebug>
#include <stdint.h>
#include <QtCore/QSharedData>

namespace NW = Nepomuk::WebExtractor;

class Nepomuk::WebExtractor::ExtractParameters::Private : public QSharedData
{
    public:
        // Merging politics
        MergePolitics m_mergePolitics;
        // DataPP launch politics
        LaunchPolitics m_launchPolitics;
        // DataPP launch step. Used in some launch politics
        unsigned int m_pss;
        // Thresholds
        double acrit, ucrit;
        // Storage of all plugins to launch
        DataPPKeeper dataPPlugins;
        // Backend name. If set to empty string,
        // then the most apropriate backend will be found
        QString backendName;
        Soprano::BackendSettings backendSettings;

        bool autoDeleteModelData;

        bool autoManageOntologies;

        bool forceModelStorageDir;

        Soprano::Model * decisionsModel;

        NW::ResourceServiceDataManager * rsdManager;

};


Nepomuk::WebExtractor::ExtractParameters::ExtractParameters()
{
    this->d = QSharedDataPointer<Private>(
                  new Nepomuk::WebExtractor::ExtractParameters::Private()
              );
    // Set defaults
    d->m_mergePolitics = NW::Highest;
    d->m_launchPolitics = NW::StepWise;
    d->m_pss = 5;
    d->autoDeleteModelData = false;
    d->rsdManager = ResourceServiceDataManager::instance();
}

Nepomuk::WebExtractor::ExtractParameters::~ExtractParameters()
{
    //kDebug() << "Deleted: " << uintptr_t(this);
}

Nepomuk::WebExtractor::ExtractParameters::ExtractParameters(const ExtractParameters & rhs)
{
    d = rhs.d;
}

const Nepomuk::WebExtractor::ExtractParameters & Nepomuk::WebExtractor::ExtractParameters::operator=(const ExtractParameters & rhs)
{
    d = rhs.d;
    return *this;
}

bool Nepomuk::WebExtractor::ExtractParameters::hasAnyDataPP() const
{
    return (d->dataPPlugins.size() != 0);
}

int NW::ExtractParameters::dataPPCount() const
{
    return d->dataPPlugins.size();
}

Nepomuk::WebExtractor::MergePolitics Nepomuk::WebExtractor::ExtractParameters::mergePolitics() const
{
    return d->m_mergePolitics;
}

void Nepomuk::WebExtractor::ExtractParameters::setMergePolitics(MergePolitics val)
{
    if((val < MergePolitics_MIN) or(val > MergePolitics_MAX)) {
        kDebug() << "Unknow merge politics :" << val << "Defaulted to MergePolitics::Highest";
        val = Highest;
    }
    d->m_mergePolitics = val;
}

Nepomuk::WebExtractor::LaunchPolitics Nepomuk::WebExtractor::ExtractParameters::launchPolitics() const
{
    return d->m_launchPolitics;
}

void Nepomuk::WebExtractor::ExtractParameters::setLaunchPolitics(LaunchPolitics val)
{
    if((val < LaunchPolitics_MIN) or(val > LaunchPolitics_MAX)) {
        kDebug() << "Unknow merge politics :" << val << "Defaulted to LaunchPolitics::Highest";
        val = StepWise;
    }
    d->m_launchPolitics = val;
}

unsigned int Nepomuk::WebExtractor::ExtractParameters::pluginSelectStep() const
{
    return d->m_pss;
}

void Nepomuk::WebExtractor::ExtractParameters::setPluginSelectStep(unsigned int step)
{
    if(step == 0) {
        kDebug() << "Zero step as plugin select step";
        step = 1;
    }
    d->m_pss = step;
}

double Nepomuk::WebExtractor::ExtractParameters::aCrit() const
{
    return d->acrit;
}

void Nepomuk::WebExtractor::ExtractParameters::setACrit(double val)
{
    if(val > 1)
        val = 1;

    if(val < 0)
        val = 0;

    d->acrit = val;

}

double Nepomuk::WebExtractor::ExtractParameters::uCrit() const
{
    return d->ucrit;
}

void Nepomuk::WebExtractor::ExtractParameters::setUCrit(double val)
{
    if(val > 1)
        val = 0.999;

    if(val < 0)
        val = 0;

    d->ucrit = val;

}

const Nepomuk::WebExtractor::DataPPKeeper & Nepomuk::WebExtractor::ExtractParameters::plugins()  const
{
    return d->dataPPlugins;
}

void Nepomuk::WebExtractor::ExtractParameters::addDataPP(DataPPWrapper * pp)
{
    d->dataPPlugins.insert(pp->data(), pp);
}

QString NW::ExtractParameters::backendName() const
{
    return d->backendName;
}

void NW::ExtractParameters::setBackendName(const QString & name)
{
    d->backendName = name;
}

Soprano::BackendSettings NW::ExtractParameters::backendSettings() const
{
    return d->backendSettings;
}
void NW::ExtractParameters::setBackendSettings(const Soprano::BackendSettings & settings)
{
    d->backendSettings = settings;
}
bool NW::ExtractParameters::autoDeleteModelData() const
{
    return d->autoDeleteModelData;
}

void NW::ExtractParameters::setAutoDeleteModelData(bool val)
{
    // TODO Add more checks here. For example is model is given, instead
    // of backend, then this flag can not be set to true.
    d->autoDeleteModelData = val;
}

bool NW::ExtractParameters::autoManageOntologies() const
{
    return d->autoManageOntologies;
}

void NW::ExtractParameters::setAutoManageOntologies(bool val)
{
    d->autoManageOntologies = val;
}


Soprano::Model * NW::ExtractParameters::decisionsModel() const
{
    return d->decisionsModel;
}

void NW::ExtractParameters::setDecisionsModel(Soprano::Model * model, bool autoManageOntologies)
{
    d->decisionsModel = model;
    d->autoManageOntologies = autoManageOntologies;
}

NW::ResourceServiceDataManager * NW::ExtractParameters::resourceServiceDataManager() const
{
    return d->rsdManager;
}

void NW::ExtractParameters::setResourceServiceDataManager(ResourceServiceDataManager * manager)
{
    if(!manager)
        d->rsdManager = ResourceServiceDataManager::instance();
    else
        d->rsdManager = manager;
}

QMap<QString, int> NW::ExtractParameters::dataPPInfo() const
{
    // TODO Introduce caching of the result
    QMap<QString, int> answer;

    foreach(const DataPPWrapper * wrp, d->dataPPlugins) {
        answer.insert(wrp->name(), wrp->version());
    }
    return answer;
}
/*
double Nepomuk::WebExtractor::ExtractParameters::scaleCoff(DataPP* pp)  const
{
    if (!d->coffs.contains(pp) ) {
    kDebug() << "Unknown DataPP.";
    return 0;
    }
    else {
    return d->coffs[pp];
    }
}

void Nepomuk::WebExtractor::ExtractParameters::setScaleCoff(DataPP* pp, double coff)
{
    if (!d->coffs.contains(pp) ) {
    kDebug() << "Unknown DataPP.";
    }
    else {
     d->coffs[pp] = coff;;
    }
}
*/

QDebug Nepomuk::WebExtractor::operator<<(QDebug dbg,  const Nepomuk::WebExtractor::ExtractParameters & p)
{
    dbg << "Extract parameters:\n";
    dbg << "aCrit: " << p.aCrit() << "\n";
    dbg << "uCrit: " << p.uCrit() << "\n";
    if(p.launchPolitics() == StepWise) {
        dbg << "Step wise launch politics. Step: " << p.pluginSelectStep() << "\n";
    } else {
        dbg << "All launch politics\n";
    }
    dbg << "DataPP:( " << p.d->dataPPlugins.size() << ')' << '\n';
    foreach(DataPPWrapper * w, p.d->dataPPlugins) {
        dbg << w->name() << w->version() << '\n';
    }
    return dbg;
}
