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
#include "datapp.h"
#include "settings_config.h"
#include "global.h"
#include <QReadLocker>
#include <QWriteLocker>
#include <KConfigBase>

using namespace Nepomuk;

/*
DataPP::DataPP(  KSharedConfig::Ptr config  )
  : DataPPConfigBase( config )
{
}
*/


DataPP::DataPP(const QString & id)
    : DataPPConfigBase(KSharedConfig::openConfig(path.arg(id))),
      m_id(id)
{
}

KSharedConfigPtr DataPP::config()
{
    // TODO what about storing a weak pointer to avoid
    // reopenning of config on each call ?
    return KSharedConfig::openConfig(path.arg(m_id));
}

KConfigGroup DataPP::userConfig()
{
    return DataPPConfigBase::config()->group(WE_USER_CONFIG_GROUP);
}

DataPP::~DataPP()
{
}

bool DataPP::isValid() const
{
    return DataPPConfigBase::source().size();
}

WebExtractorPlugin * DataPP::plugin()
{
    return GlobalSettings::plugin(DataPPConfigBase::source());
}

WebExtractorPluginKCM * DataPP::kcm()
{
    WebExtractorPluginKCM * answer = GlobalSettings::kcm(DataPPConfigBase::source());
    if ( answer )
	answer->setCurrentDataPP(this->userConfig());
    return answer;
}

WebExtractor::Executive * DataPP::executive()
{
    QReadLocker rl(&m_lock());
    QHash< QString, Nepomuk::WebExtractor::Executive*>::iterator it = m_executiveHash().find(m_id);

    if(it == m_executiveHash().end()) {
        rl.unlock();
        WebExtractorPlugin * plg = plugin();
        if(plg) {
            WebExtractor::Executive * ex = plg->getExecutive(userConfig());
            // Insert even if dpp == 0.
            QWriteLocker wl(&m_lock());
            m_executiveHash().insert(m_id, ex);

            // Return now to avoid unnecessary searching
            return ex;
        }
        return 0;
    }
    return *it;
}

QHash< QString, Nepomuk::WebExtractor::Executive*> & DataPP::m_executiveHash()
{
    static QHash< QString, Nepomuk::WebExtractor::Executive*> m_dp;
    return m_dp;
}

QReadWriteLock & DataPP::m_lock()
{
    static QReadWriteLock m_l;
    return m_l;
}


WebExtractor::Executive * DataPP::executive(const QString & name)
{
    QReadLocker rl(&m_lock());
    QHash< QString, Nepomuk::WebExtractor::Executive*>::iterator it = m_executiveHash().find(name);

    if(it == m_executiveHash().end()) {
        rl.unlock();
        // Load datapp
        DataPP * dppcfg = new DataPP(name);
        WebExtractor::Executive * ex = dppcfg->executive();
        delete dppcfg;
        return ex;
    }
    return *it;
}

#if 0
int DataPP::dataPPCount()
{
    QReadLocker rl(&m_lock());
    return m_executiveHash().size();
}
#endif

QString Nepomuk::DataPP::path = QString(PLUGIN_CONFIG_DIR"%1"".datapp");
