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


Nepomuk::DecisionManagementService::DecisionManagementService(
	QObject * parent,
	const QList<QVariant> & args ):
    Nepomuk::Service(parent,true),
    m_storage(0)
{
    (void)new  DecisionManagementAdaptor(this);
    // Get application data directory
    QString dataDir = KStandardDirs::locateLocal("data","nepomuk/ddms/",true);

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
	return;
    }

    setServiceInitialized(true);
}

Nepomuk::DecisionManagementService::~DecisionManagementService()
{
    delete m_storage;
    m_storage = 0;
}


QList<Nepomuk::DecisionManagementService::ID>
Nepomuk::DecisionManagementService::getDecisions(const QString & uri)
{
    return m_storage->queryDecisions(uri);
}

int Nepomuk::DecisionManagementService::removeDecision(int id)
{
    return m_storage->removeDecision(id);
}
