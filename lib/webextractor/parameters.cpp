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
namespace ND = Nepomuk::Decision;

class NW::ExtractParameters::Private : public QSharedData
{
    public:
        // Merging politics
        ND::MergePolitics m_mergePolitics;
        // Executive launch politics
        LaunchPolitics m_launchPolitics;
        // Executive launch step. Used in some launch politics
        unsigned int m_pss;
        // Thresholds
        double acrit, ucrit;
        // Storage of all plugins to launch
        ExecutiveKeeper dataPPlugins;
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


NW::ExtractParameters::ExtractParameters()
    : d(new Private())
{
    // Set defaults
    d->m_mergePolitics = ND::Highest;
    d->m_launchPolitics = NW::StepWise;
    d->m_pss = 5;
    d->autoDeleteModelData = false;
    d->rsdManager = ResourceServiceDataManager::instance();
}

NW::ExtractParameters::~ExtractParameters()
{
    //kDebug() << "Deleted: " << uintptr_t(this);
}

NW::ExtractParameters::ExtractParameters(const ExtractParameters & rhs)
{
    d = rhs.d;
}

const NW::ExtractParameters & NW::ExtractParameters::operator=(const ExtractParameters & rhs)
{
    d = rhs.d;
    return *this;
}

bool NW::ExtractParameters::hasAnyExecutive() const
{
    return (d->dataPPlugins.size() != 0);
}

int NW::ExtractParameters::executiveCount() const
{
    return d->dataPPlugins.size();
}

ND::MergePolitics NW::ExtractParameters::mergePolitics() const
{
    return d->m_mergePolitics;
}

void NW::ExtractParameters::setMergePolitics(ND::MergePolitics val)
{
    if((val < ND::MergePolitics_MIN) or(val > ND::MergePolitics_MAX)) {
        kDebug() << "Unknow merge politics :" << val << "Defaulted to MergePolitics::Highest";
        val = ND::Highest;
    }
    d->m_mergePolitics = val;
}

NW::LaunchPolitics NW::ExtractParameters::launchPolitics() const
{
    return d->m_launchPolitics;
}

void NW::ExtractParameters::setLaunchPolitics(LaunchPolitics val)
{
    if((val < LaunchPolitics_MIN) or(val > LaunchPolitics_MAX)) {
        kDebug() << "Unknow merge politics :" << val << "Defaulted to LaunchPolitics::Highest";
        val = StepWise;
    }
    d->m_launchPolitics = val;
}

unsigned int NW::ExtractParameters::pluginSelectStep() const
{
    return d->m_pss;
}

void NW::ExtractParameters::setPluginSelectStep(unsigned int step)
{
    if(step == 0) {
        kDebug() << "Zero step as plugin select step";
        step = 1;
    }
    d->m_pss = step;
}

double NW::ExtractParameters::aCrit() const
{
    return d->acrit;
}

void NW::ExtractParameters::setACrit(double val)
{
    if(val > 1)
        val = 1;

    if(val < 0)
        val = 0;

    d->acrit = val;

}

double NW::ExtractParameters::uCrit() const
{
    return d->ucrit;
}

void NW::ExtractParameters::setUCrit(double val)
{
    if(val > 1)
        val = 0.999;

    if(val < 0)
        val = 0;

    d->ucrit = val;

}

const NW::ExecutiveKeeper & NW::ExtractParameters::plugins()  const
{
    return d->dataPPlugins;
}

void NW::ExtractParameters::addExecutive(ExecutiveWrapper * pp)
{
    d->dataPPlugins.insert(pp->data(), pp);
}

/*
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
*/


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

    foreach(const ExecutiveWrapper * wrp, d->dataPPlugins) {
        answer.insert(wrp->name(), wrp->version());
    }
    return answer;
}
/*
double NW::ExtractParameters::scaleCoff(Executive* pp)  const
{
    if (!d->coffs.contains(pp) ) {
    kDebug() << "Unknown Executive.";
    return 0;
    }
    else {
    return d->coffs[pp];
    }
}

void NW::ExtractParameters::setScaleCoff(Executive* pp, double coff)
{
    if (!d->coffs.contains(pp) ) {
    kDebug() << "Unknown Executive.";
    }
    else {
     d->coffs[pp] = coff;;
    }
}
*/

QDebug NW::operator<<(QDebug dbg,  const NW::ExtractParameters & p)
{
    dbg << "Extract parameters:\n";
    dbg << "aCrit: " << p.aCrit() << "\n";
    dbg << "uCrit: " << p.uCrit() << "\n";
    if(p.launchPolitics() == StepWise) {
        dbg << "Step wise launch politics. Step: " << p.pluginSelectStep() << "\n";
    } else {
        dbg << "All launch politics\n";
    }
    dbg << "Executive:( " << p.d->dataPPlugins.size() << ')' << '\n';
    foreach(ExecutiveWrapper * w, p.d->dataPPlugins) {
        dbg << w->name() << w->version() << '\n';
    }
    return dbg;
}

bool NW::ExtractParameters::isValid() const
{
#warning TODO: implement me
    return true;
}
