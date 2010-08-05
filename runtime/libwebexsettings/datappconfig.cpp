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


#include "datappconfig.h"
#include "settings_config.h"
#include "global.h"
#include <QReadLocker>
#include <QWriteLocker>

using namespace Nepomuk;

/*
DataPPConfig::DataPPConfig(  KSharedConfig::Ptr config  )
  : DataPPConfigBase( config )
{
}
*/


DataPPConfig::DataPPConfig(const QString & name)
    : DataPPConfigBase(KSharedConfig::openConfig(path.arg(name))),
      m_name(name)
{
}

KSharedConfigPtr DataPPConfig::config()
{
    return KSharedConfig::openConfig(path.arg(m_name));
}

DataPPConfig::~DataPPConfig()
{
}

bool DataPPConfig::isValid() const
{
    return DataPPConfigBase::source().size();
}

WebExtractorPlugin * DataPPConfig::plugin()
{
    return GlobalSettings::plugin(DataPPConfigBase::source());
}

WebExtractor::DataPP * DataPPConfig::dataPP()
{
    return dataPP(m_name);
}

QHash< QString, Nepomuk::WebExtractor::DataPP*> & DataPPConfig::m_datapp()
{
    static QHash< QString, Nepomuk::WebExtractor::DataPP*> m_dp;
    return m_dp;
}

QReadWriteLock & DataPPConfig::m_lock()
{
    static QReadWriteLock m_l;
    return m_l;
}


WebExtractor::DataPP * DataPPConfig::dataPP(const QString & name)
{
    QReadLocker rl(&m_lock());
    QHash< QString, Nepomuk::WebExtractor::DataPP*>::iterator it = m_datapp().find(name);

    if(it == m_datapp().end()) {
        rl.unlock();
        // Load datapp
        DataPPConfig * dppcfg = new DataPPConfig(name);
        if(!dppcfg->isValid())
            return 0;

        WebExtractorPlugin * plg = dppcfg->plugin();
        if(plg) {
            WebExtractor::DataPP * dpp = plg->getDataPP(dppcfg->config());
            // Insert even if dpp == 0.
            QWriteLocker wl(&m_lock());
            m_datapp().insert(name, dpp);

            // Return now to avoid unnecessary searching
            delete dppcfg;
            return dpp;
        }
        delete dppcfg;
        return 0;
    }
    return *it;
}

int DataPPConfig::dataPPCount()
{
    QReadLocker rl(&m_lock());
    return m_datapp().size();
}

QString Nepomuk::DataPPConfig::path = QString(PLUGIN_CONFIG_DIR"%1rc");
