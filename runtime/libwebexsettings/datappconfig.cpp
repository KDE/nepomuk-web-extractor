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


#include "webextractor_plugin.h"
#include "webextractor_kcm.h"
#include "datappconfig.h"
#include "settings_config.h"
#include "global.h"
#include <QReadLocker>
#include <QWriteLocker>
#include <KConfigBase>

using namespace Nepomuk;

/*
DppExecutiveConfig::DppExecutiveConfig(  KSharedConfig::Ptr config  )
  : DppExecutiveConfigBase( config )
{
}
*/


DppExecutiveConfig::DppExecutiveConfig(const QString & name)
    : DppExecutiveConfigBase(KSharedConfig::openConfig(path.arg(name))),
      m_name(name)
{
}

KSharedConfigPtr DppExecutiveConfig::config()
{
    // TODO what about storing a weak pointer to avoid
    // reopenning of config on each call ?
    return KSharedConfig::openConfig(path.arg(m_name));
}

QSharedPointer<KConfigBase> DppExecutiveConfig::userConfig()
{
    KConfigGroup * answer = new KConfigGroup();
    *answer = DppExecutiveConfigBase::config()->group(WE_USER_CONFIG_GROUP);
    QSharedPointer<KConfigBase> a(answer);
    Q_ASSERT(a);
    if (!a) {
	kError() << "Config is empty!";
    }
    return a;
}

DppExecutiveConfig::~DppExecutiveConfig()
{
}

bool DppExecutiveConfig::isValid() const
{
    return DppExecutiveConfigBase::source().size();
}

WebExtractorPlugin * DppExecutiveConfig::plugin()
{
    return GlobalSettings::plugin(DppExecutiveConfigBase::source());
}

WebExtractorPluginKCM * DppExecutiveConfig::kcm()
{
    WebExtractorPluginKCM * answer = GlobalSettings::kcm(DppExecutiveConfigBase::source());
    if ( answer )
	answer->setCurrentDppExecutive(this->userConfig());
    return answer;
}

WebExtractor::DppExecutive * DppExecutiveConfig::dataPP()
{
    return dataPP(m_name);
}

QHash< QString, Nepomuk::WebExtractor::DppExecutive*> & DppExecutiveConfig::m_datapp()
{
    static QHash< QString, Nepomuk::WebExtractor::DppExecutive*> m_dp;
    return m_dp;
}

QReadWriteLock & DppExecutiveConfig::m_lock()
{
    static QReadWriteLock m_l;
    return m_l;
}


WebExtractor::DppExecutive * DppExecutiveConfig::dataPP(const QString & name)
{
    QReadLocker rl(&m_lock());
    QHash< QString, Nepomuk::WebExtractor::DppExecutive*>::iterator it = m_datapp().find(name);

    if(it == m_datapp().end()) {
        rl.unlock();
        // Load datapp
        DppExecutiveConfig * dppcfg = new DppExecutiveConfig(name);
        if(!dppcfg->isValid())
            return 0;

        WebExtractorPlugin * plg = dppcfg->plugin();
        if(plg) {
            WebExtractor::DppExecutive * dpp = plg->getExecutive(dppcfg->userConfig());
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

int DppExecutiveConfig::dataPPCount()
{
    QReadLocker rl(&m_lock());
    return m_datapp().size();
}

QString Nepomuk::DppExecutiveConfig::path = QString(PLUGIN_CONFIG_DIR"%1rc");
