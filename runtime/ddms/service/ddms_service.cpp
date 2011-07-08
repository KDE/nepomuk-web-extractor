/*
   Copyright (C) 2011 by Serebriyskiy Artem <v.for.vandal at gmail.com>

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

#include "ddms_service.h"
#include "decisionmanagementserviceadaptor.h"


#include <KStandardDirs>
#include <QSqlDatabase>
#include <decision/decision.h>

Nepomuk::DecisionManagementService::DecisionManagementService(
        QObject * parent,
        const QList<QVariant> & args ):
    Nepomuk::Service(parent,true),
    m_storage(0)
{
    (void)new  DecisionManagementAdaptor(this);

    /* Init necessary QT metatypes */
    IdAndError::registerMetaType();
    IdList::registerMetaType();
    Decision::DecisionMetadata::registerMetaType();
    MetadataAndError::registerMetaType();

    // Get application data directory
    QString dataDir = KStandardDirs::locateLocal("data","nepomuk/ddms/",true);
    qDebug() << "Data directory: " << dataDir;

    if (dataDir.isEmpty()) {
        qCritical() << "Can not locate data directory";
        setServiceInitialized(false);
        return;
    }

    // Prepare sql service
    m_db = QSqlDatabase::addDatabase("QSQLITE","ddms_database");
    m_db.setDatabaseName(dataDir + "/index.sqlite");
    m_db.open();

    if (!m_db.isValid() or m_db.isOpenError() ) {
        // Fail
        qCritical() << "Failed to open/create database";
        setServiceInitialized(false);
        m_db.close();
        return;
    }

    m_storage = new DecisionStorage(m_db,dataDir);
    if (!m_storage->isValid()) {
        qCritical() << "Failed to initialize Decision storage";
        delete m_storage;
        m_storage = 0;
        setServiceInitialized(false);
        return;
    }

    setServiceInitialized(true);
}

Nepomuk::DecisionManagementService::~DecisionManagementService()
{
    
    delete m_storage;
    m_storage = 0;
    m_db.close();
}


IdList
Nepomuk::DecisionManagementService::getDecisions(const QString & uri)
{
    qDebug() << "CALL: " << __func__; 
    return m_storage->queryDecisions(uri);
}

int Nepomuk::DecisionManagementService::removeDecision(int id)
{
    qDebug() << "CALL: " << __func__; 
    return m_storage->removeDecision(id);
}

IdAndError Nepomuk::DecisionManagementService::addDecision( 
        const QByteArray & decision)
{
    // Load decision from byte array
    QDataStream dstream(decision);
    Decision::Decision dec = Decision::Decision::load(dstream);

    qDebug() << "CALL: " << __func__; 
    int id = -1;
    int error =  m_storage->addDecision(dec,&id);
    IdAndError result;
    result.first = id;
    result.second = error;
    if ( error != Error::NoError ) 
       qDebug() << "Error while adding Decision. code: " << error;
    else 
       qDebug() << "Add decision. ID: " << id;

    return result;
}

bool Nepomuk::DecisionManagementService::existsDecision(int id)
{
    return m_storage->existsDecision(id);
}

MetadataAndError Nepomuk::DecisionManagementService::decisionMetadata(int  id)
{
    int error;
    Decision::DecisionMetadata md = m_storage->decisionMetadata(id,error);
    MetadataAndError answer;
    answer.error = error;
    answer.metadata = md;
    return answer;
}



#include <kpluginfactory.h>
#include <kpluginloader.h>
NEPOMUK_EXPORT_SERVICE(Nepomuk::DecisionManagementService, "decisionmanagementservice");
// vim:sw=4 ts=8 expandtab
