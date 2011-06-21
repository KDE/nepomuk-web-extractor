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

#include "ddms_core.h"

#include <QList>
#include <QVariant>
#include <QUrl>
#include <QFile>
#include <QFileInfo>
#include <QDir>
#include <QtDebug>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlDatabase>

#define ID_TABLE_NAME "id"
#define RES_TABLE_NAME "resources"
#define ID_COLUMN_NAME "id"
#define RES_COLUMN_NAME "resource"

class Nepomuk::DecisionStorage::Private 
{
    public:
	bool valid;
	QSqlDatabase db;
	QString pathFolder;
	/*! \brief Register new file with patch in database and return it's name
	 * You shoud write actuall path serialization to this file. File will be
	 * created automatically
	 * On fail, return NULL string
	 * \return Filename or empty string, if failed
	 */
	QString requestFileName(const QList<QUrl> & resources);
	
	/*! \brief Prepare database for usage
	 * Create necessary tables.
	 * \return true, if success. If some table failed to be created or table already
	 * exist,but has incorrect declaration, return false
	 */
	bool prepareDatabase() ;
	bool removeFromIndex(int);
	bool removeFromFolder(int);
	bool checkIdTable();
	bool checkResTable();
	static QString insertIdQueryString;
	static QString insertResUrlQueryString;
	//static QString;
};

QString Nepomuk::DecisionStorage::Private::insertIdQueryString = 
QString("INSERT INTO "ID_TABLE_NAME" VALUES (NULL)");

QString Nepomuk::DecisionStorage::Private::insertResUrlQueryString = 
QString("INSERT INTO "RES_TABLE_NAME" VALUES (:id, :res)");

Nepomuk::DecisionStorage::DecisionStorage(
	const QSqlDatabase & database, 
	const QString & pathFolder):
    d(new Private())
{
    d->db = database;
    d->pathFolder = pathFolder;
    d->valid = true;

    QFileInfo pathInfo(pathFolder);
    if ( !pathInfo.isDir() or !pathInfo.isWritable() ) {
	d->valid = false;
    }
    
}

QString 
Nepomuk::DecisionStorage::Private::requestFileName( const QList<QUrl> & resources)
{
    Q_ASSERT(valid);

    // Start transaction
    if (!db.transaction()) {
	qDebug() << "Can't start transaction: " << db.lastError().text();
	return QString();
    }

    // Because labels are used, all variables must be declared
    // in the begining
    QSqlQuery insertResUrlQuery(db);
    QSqlQuery getIdQuery(db);
    QString filename;
    QFile file;
    bool success = true;
    int id = -1;
    QList<QUrl>::const_iterator it;

    // Stage 1 - create unique ID
    QSqlQuery insertIdQuery(this->insertIdQueryString,this->db);
    insertIdQuery.exec();

    if (!insertIdQuery.isActive()) {
	qDebug() << "Can't insert new ID" << insertIdQuery.lastError().text();
	goto FAIL;
    }

    // Get this id
    getIdQuery.prepare("last_inserted_rowid()");
    if(!getIdQuery.isActive()) {
	qDebug() << "Can't acquire inserted ID" << insertIdQuery.lastError().text();
	goto FAIL;
    }

    if ( getIdQuery.next() ) {
	id = getIdQuery.value(0).toInt();
    }
    else {
	qDebug() << "last_inserted_rowid() reports" << getIdQuery.lastError().text();
	goto FAIL;
    }
    getIdQuery.finish();

    if ( id <= 0 )
	goto FAIL;

    // Stage 2 - add record to the table
    success = true;
    for( it = resources.begin(); it != resources.end(); ++it )
    {
	insertResUrlQuery.prepare(insertResUrlQueryString);
	insertResUrlQuery.bindValue(":res",it->toString());
	insertResUrlQuery.bindValue(":id",id);
	insertResUrlQuery.exec();
	if (!insertResUrlQuery.isActive()) {
	    qDebug() << "Can't insert resource into table. Error: " << insertResUrlQuery.lastError().text();

	    success = false;
	    break;
	}
    }
    insertResUrlQuery.finish();

    if (!success)
	goto FAIL;

    // Stage 3 - create file in the specified folder
    filename = QString::number(id);
    file.setFileName(pathFolder + '/' + filename );
    file.open(QIODevice::WriteOnly);
    file.close();
    if (!db.commit() ) {
	qDebug() << "Failed to commit changes. Error:" << db.lastError().text();
	db.rollback();
	return QString();
    }
    return filename;

FAIL:
    db.rollback();
    return QString();
}

bool 
Nepomuk::DecisionStorage::Private::checkIdTable()
{
    QSqlQuery idCheckQuery("PRAGMA table_info("ID_TABLE_NAME")",db);
    idCheckQuery.exec();
    if (!idCheckQuery.isActive()) {
	qDebug() << "Can't retrieve information about id table";
	return false;
    }

    if ( !idCheckQuery.next() ) {
	// Table has no column
	qDebug() << "Table has no column. Must have at least one column";
	return false;
    }

    // Check first column. It must has name ID_COLUMN_NAME and type INTEGER

    QString name = idCheckQuery.value(0).toString();
    if ( name != ID_COLUMN_NAME ) {
	qDebug() << "id table has column named " << name << " instead of " << ID_COLUMN_NAME;
	return false;
    }

    QString type = idCheckQuery.value(1).toString();
    if ( type != "INTEGER" ) {
	qDebug() << "id table has column with type " << type << "instead of INTEGER";
	return false;
    }

    // Check that there is no more columns
    if ( idCheckQuery.next() ) {
	qDebug() << "id table has more than one column";
	return false;
    }

    return true;

}

bool 
Nepomuk::DecisionStorage::Private::checkResTable()
{
    QSqlQuery resCheckQuery("PRAGMA table_info("ID_TABLE_NAME")",db);
    resCheckQuery.exec();
    if (!resCheckQuery.isActive()) {
	qDebug() << "Can't retrieve information about resources table";
	return false;
    }

    if ( !resCheckQuery.next() ) {
	// Table has no column
	qDebug() << "Table has no column. Must have at least one column";
	return false;
    }

    // Check first column. It must has name ID_COLUMN_NAME and type INTEGER

    QString name = resCheckQuery.value(0).toString();
    if ( name != ID_COLUMN_NAME ) {
	qDebug() << "resources table has column named " << name << " instead of " << ID_COLUMN_NAME;
	return false;
    }

    QString type = resCheckQuery.value(1).toString();
    if ( type != "INTEGER" ) {
	qDebug() << "resources table has column with type " << type << "instead of INTEGER";
	return false;
    }

    if ( !resCheckQuery.next() ) {
	// Table has no column
	qDebug() << "Table has one column. Must have at least two columns";
	return false;
    }

    name = resCheckQuery.value(0).toString();
    if ( name != RES_COLUMN_NAME ) {
	qDebug() << "resources table has column named " << name << " instead of " << RES_COLUMN_NAME;
	return false;
    }

    type = resCheckQuery.value(1).toString();
    if ( type != "TEXT" ) {
	qDebug() << "resources table has column with type " << type << "instead of TEXT";
	return false;
    }

    // Check that there is no more columns
    if ( resCheckQuery.next() ) {
	qDebug() << "resources table has more than two columns";
	return false;
    }

    return true;

}
bool 
Nepomuk::DecisionStorage::Private::prepareDatabase()
{
    if (!db.transaction()) {
	qDebug() << "Can not start a transaction";
	return false;
    }

    QSqlQuery idTableExists(db);
    QSqlQuery resTableExists(db);
    QSqlQuery createIdTable(db);
    QSqlQuery createResTable(db);
    // Check whether id table already exists
    idTableExists.prepare(
	    "SELECT name FROM sqlite_master WHERE type='table' AND name='id'");
    idTableExists.exec();
    if (!idTableExists.isActive()) {
	qDebug() << "Can not execute query to check existance of id table. Error: " <<
	    idTableExists.lastError().text();
	goto FAIL;
    }
    if (idTableExists.next()) {
	// Table exists
	if (!checkIdTable())
	    qDebug() << "ID table exists, but has incorrect specification";
	    goto FAIL;
    }
    else {
	// Create table
	createIdTable.prepare("CREATE TABLE id( id INTEGER PRIMARY KEY )");
	createIdTable.exec();
	if (!createIdTable.isActive()) {
	    qDebug() << "Can't create ID table. Error: " << 
		createIdTable.lastError().text();
	    goto FAIL;
	}
	    
    }

    resTableExists.prepare(
	    "SELECT name FROM sqlite_master WHERE type='table' AND name='resources'");

    if (resTableExists.next()) {
	// Table exists
	if (!checkResTable()) {
	    qDebug() << "resources table exist, but has incorrect specification";
	    goto FAIL;
	}
    }
    else {
	// Create table
	createResTable.prepare("CREATE TABLE resources( id INTEGER, resource TEXT)");
	createResTable.exec();
	if (!createResTable.isActive()) {
	    qDebug() << "Can't create ID table. Error: " << 
		createResTable.lastError().text();
	    goto FAIL;
	}
    }

    return true;
	

FAIL:
    db.rollback();
    return false;
}

bool Nepomuk::DecisionStorage::Private::removeFromIndex(ID id)
{
    bool failed = false;
    QString remove_prepare = 
	QString("DELETE FROM :table WHERE "ID_COLUMN_NAME" = ") +
	QString::number(id);

    QSqlQuery removeIdQuery(db);
    removeIdQuery.prepare(remove_prepare);
    removeIdQuery.bindValue(":table",ID_TABLE_NAME);
    removeIdQuery.exec();
    if (!removeIdQuery.isActive()) {
	failed = true;
    }

    QSqlQuery removeResQuery(db);
    removeResQuery.prepare(remove_prepare);
    removeResQuery.bindValue(":table",RES_TABLE_NAME);
    removeResQuery.exec();
    if (!removeResQuery.isActive()) {
	failed = true;
    }

    if ( failed ) {
	qWarning() << "Failed to remove decision(ID:" << id << ") from index(table id)";
    }
    return failed;
}

bool Nepomuk::DecisionStorage::Private::removeFromFolder(ID id)
{
    QFile file(pathFolder + '/' + QString::number(id));
    return file.remove();
}

Nepomuk::DecisionStorage::Error
Nepomuk::DecisionStorage::addDecision( const Decision & decision)
{
    if (!d->valid)
	return SystemError ;

    // Stage 1 - accquire filename
    QString filename = d->requestFileName(QList<QUrl>());
    
   if ( filename.isEmpty() ) {
      // Error
      return DatabaseError;
   } 

   // Open file
   QFile file(filename);
   file.open(QIODevice::WriteOnly);

   file.close();
   return NoError;

}


bool Nepomuk::DecisionStorage::isValid() const
{ return d->valid; }


Nepomuk::DecisionStorage::Error 
Nepomuk::DecisionStorage::removeDecision(ID id)
{
    bool result = true;
    result = result && d->removeFromIndex(id);
    result = result && d->removeFromFolder(id);
    if ( !result )
	return NoSuchDecision; 

    return NoError;
}

QList<int> Nepomuk::DecisionStorage::queryDecisions(const QUrl & resource)
{
    QSqlQuery selectQuery(
	    QString("SELECT id FROM "RES_TABLE_NAME" WHERE resource = ") + resource.toString(),
	    d->db
	    );
    selectQuery.exec();
    if (!selectQuery.isActive() ) {
	qWarning() << "Can not execute SELECT query to database";
	return QList<int>();
    }

    QList<int> answer;
    while( selectQuery.next() ) 
    {
	answer << selectQuery.value(0).toInt();
    }
    return answer;
}

Nepomuk::DecisionStorage::~DecisionStorage()
{;}
