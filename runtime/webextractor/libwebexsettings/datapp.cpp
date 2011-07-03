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
#include "webexsettings_global.h"
#include <QReadLocker>
#include <QTemporaryFile>
#include <QFileInfo>
#include <QWriteLocker>
#include <KConfigBase>
#include <KRandom>
#include <limits.h>

using namespace Nepomuk;


class DataPP::Private
{
    public:
        QString id;
        //bool idLocked;
        KConfigGroup config;
};

DataPP* DataPP::newDataPP()
{
    int i = 0;
    QString newId;
    while( ++i < INT_MAX)
    {
        newId = KRandom::randomString(PLUGIN_MAX_NAME_LENGTH);
        if (!mainConfig()->hasGroup(newId) /*&& !isLocked(newId)*/)
            break;
    }
    //lockId(newId);
    DataPP * answer =  new DataPP(newId);
    //answer->idLocked = true;
    return answer;
}

void DataPP::removeDataPP(const QString id)
{
    mainConfig()->deleteGroup(id);
}

DataPP::DataPP(const QString & id):
    d(new Private())
{
    d->id = id;
#if 0
    if (mainConfig()->hasGroup(id) ) { 
        // THis is config for existing DataPP
        d->idLocked = false;
    }
    else {
        // This is config for new DataPP. It is not written
        // to config untill synced
        d->idLocked = true;
        lockId(id);
    }
#endif
    d->config = mainConfig()->group(id);
}

KConfigGroup DataPP::config()
{
    // TODO what about storing a weak pointer to avoid
    // reopenning of config on each call ?
    return d->config; 
}

KConfigGroup DataPP::userConfig()
{
    return d->config.group(WE_USER_CONFIG_GROUP);
}

DataPP::~DataPP()
{
    delete d;
}

QString DataPP::source() const
{
    return d->config.readEntry("source","");
}

void DataPP::setSource( const QString & value )
{
    d->config.writeEntry("source",value);
}

QString DataPP::displayName() const
{
    return d->config.readEntry("displayName","");
}

void DataPP::setDisplayName(const QString & value)
{
    d->config.writeEntry("displayName",value);
}

QString DataPP::description() const
{
    return d->config.readEntry("description","");
}

void DataPP::setDescription(const QString & value)
{
    d->config.writeEntry("description",value);
}

QStringList DataPP::categories() const
{
    return d->config.readEntry("categories",QStringList());
}

void DataPP::setCategories(const QStringList & value)
{
    d->config.writeEntry("categories",value);
}


bool DataPP::isValid() const
{
    return source().size();
}

void DataPP::sync()
{
    d->config.sync();
    // Now we can unlock our id
#if 0
    if (d->idLocked) {
        unlockId(d->id);
    }
#endif
}

void DataPP::remove()
{
    mainConfig()->deleteGroup(d->id);
}

WebExtractorPlugin * DataPP::plugin()
{
    if (!isValid())
        return 0;

    return GlobalSettings::plugin(source());
}

WebExtractorPluginKCM::Ptr DataPP::kcm(bool forceNew )
{
    if (!isValid())
        return WebExtractorPluginKCM::Ptr();

    WebExtractorPluginKCM::Ptr answer = GlobalSettings::kcm(
            source(),
            forceNew
            );
    if ( answer )
        answer->setCurrentDataPP(this->userConfig());
    return answer;
}

WebExtractor::Executive * DataPP::executive()
{
    if (!isValid())
        return 0;

    QReadLocker rl(&m_lock());
    QHash< QString, Nepomuk::WebExtractor::Executive*>::iterator it = m_executiveHash().find(d->id);

    if(it == m_executiveHash().end()) {
        rl.unlock();
        WebExtractorPlugin * plg = plugin();
        if(plg) {
            WebExtractor::Executive * ex = plg->getExecutive(userConfig());
            // Insert even if dpp == 0.
            QWriteLocker wl(&m_lock());
            m_executiveHash().insert(d->id, ex);

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

/*
QString DataPP::path(const QString & id)
{
    return m_path.arg(id);
}

QString DataPP::path()
{
    static QString answer(PLUGIN_CONFIG_DIR);
    return answer;
}
*/

/*
QString DataPP::filenameToId(const QString & fileName)
{
    QString answer = fileName;
    return answer.remove(answer.size() - 7,7);
}
*/

KConfig* DataPP::mainConfig()
{
    static KConfig * _mc = new KConfig(PLUGIN_CONFIG_FILE, KConfig::SimpleConfig);
    return _mc;
}
/*

void DataPP::lockId(const QString & id)
{
    lockDb.insert(id);
}

void DataPP::unlockId(const QString & id)
{
    lockDb.remove(id);
}

bool DataPP::isLocked(const QString & id)
{
    QReadLocker rl(&m_lock());
    return lockDb.contains(id);
}

QSet<QString> & DataPP::lockDb()
{
    static QSet<QString> _db;
    return _db;
}
*/

QString Nepomuk::DataPP::m_path = QString(PLUGIN_CONFIG_DIR"%1"".datapp");
