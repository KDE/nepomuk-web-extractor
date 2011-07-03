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
#include "protocol_types.h"
#include <decision/decision.h>
#include <decision/decisionmetadata.h>

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
#define META_TABLE_NAME "metadata" 
#define AUTHOR_TABLE_NAME "authors" 
#define ID_COLUMN_NAME "id"
#define RES_COLUMN_NAME "resource"

#define TABLE_INFO_COLUMN_NAME 1
#define TABLE_INFO_COLUMN_TYPE 2

using namespace Nepomuk;
using namespace Nepomuk::Decision;

/* ======= Table definitions =========
 * Table: id
 * ( id : int )
 *
 * Table : resources
 * (id : int, resource: string )
 *
 * Table : metadata
 * (id : int, rank : double, description : string )
 *
 * Table : authors
 * ( id : int, name : string, version : int )
 * name - author name
 * version - author version
 */
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
        QString requestFileName(const QList<QString> & resources, const DecisionMetadata & metadata, int * id = 0);
        
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
        bool checkMetaTable();
        bool checkAuthorsTable();
        static QString insertIdQueryString;
        static QString insertResUrlQueryString;
        static QString insertMetaQueryString;
        static QString insertAuthorQueryString;
        //static QString;
};

QString Nepomuk::DecisionStorage::Private::insertIdQueryString = 
QString("INSERT INTO "ID_TABLE_NAME" VALUES (NULL)");

QString Nepomuk::DecisionStorage::Private::insertResUrlQueryString = 
QString("INSERT INTO "RES_TABLE_NAME" VALUES (:id, :res)");

QString Nepomuk::DecisionStorage::Private::insertMetaQueryString = 
QString("INSERT INTO "META_TABLE_NAME" VALUES (:id, :rank, :description)");

QString Nepomuk::DecisionStorage::Private::insertAuthorQueryString = 
QString("INSERT INTO "AUTHOR_TABLE_NAME" VALUES (:id, :name, :version)");

Nepomuk::DecisionStorage::DecisionStorage(
        const QSqlDatabase & database, 
        const QString & pathFolder):
    d(new Private())
{
    d->valid = true;

    d->db = database;
    d->pathFolder = pathFolder;


    if (!d->prepareDatabase()) {
        qDebug() << "Failed to prepare database";
        d->valid = false;
        return;
    }

    QFileInfo pathInfo(pathFolder);
    if ( !pathInfo.isDir() or !pathInfo.isWritable() ) {
        qDebug() << "Given path for storage is not a writable directory. Path: " << pathInfo.absolutePath();
        d->valid = false;
    }
    
}

QString 
Nepomuk::DecisionStorage::Private::requestFileName( const QList<QString> & resources, const DecisionMetadata & metadata, int * idAnswer)
{
    Q_ASSERT(valid);
    qDebug() << "CALL: " << __func__;

    // Start transaction
    if (!db.transaction()) {
        qDebug() << "Can't start transaction: " << db.lastError().text();
        return QString();
    }

    // Because labels are used, all variables must be declared
    // in the begining
    QSqlQuery insertAuthorQuery(db);
    QSqlQuery insertMetadataQuery(db);
    QSqlQuery insertResUrlQuery(db);
    QSqlQuery getIdQuery(db);
    QString filename;
    QFile file;
    bool success = true;
    int id = -1;
    QList<QString>::const_iterator it;
    QHash<QString,int>::const_iterator author_it;
    QVariant lastId;

    // Stage 1 - create unique ID
    QSqlQuery insertIdQuery(this->db);
    insertIdQuery.prepare(this->insertIdQueryString);
    insertIdQuery.exec();

    if (!insertIdQuery.isActive()) {
        qDebug() << "Can't insert new ID" << insertIdQuery.lastError().text();
        goto FAIL;
    }

    // Get this id
    lastId = insertIdQuery.lastInsertId();
    if (!lastId.isValid()) {
        qDebug() << "Can't acquire inserted ID" << insertIdQuery.lastError().text();
        goto FAIL;
    }

    id = lastId.toInt();

    if ( id <= 0 ) {
        qDebug() << "Accquired ID is negative";
        goto FAIL;
    }

    //qDebug() << "Accuired ID: " << id;


    // Stage 2.0 - add record to the resources table
    success = true;
    for( it = resources.begin(); it != resources.end(); ++it )
    {
        insertResUrlQuery.prepare(insertResUrlQueryString);
        insertResUrlQuery.bindValue(":res",*it);
        insertResUrlQuery.bindValue(":id",id);
        insertResUrlQuery.exec();
        if (!insertResUrlQuery.isActive()) {
            qDebug() << "Can't insert resource into table. Error: " << insertResUrlQuery.lastError().text();

            success = false;
            break;
        }
        insertResUrlQuery.finish();
    }
    insertResUrlQuery.finish();

    if (!success)
        goto FAIL;

    // Stage 2.1 - add record to metadata table
    insertMetadataQuery.prepare(insertMetaQueryString);
    insertMetadataQuery.bindValue(":description",metadata.description);
    insertMetadataQuery.bindValue(":rank",metadata.rank);
    insertMetadataQuery.exec();
    if (!insertMetadataQuery.isActive()) {
        qDebug() << "Can't insert metadata into table. Error: " << insertMetadataQuery.lastError().text();
        goto FAIL;
    }
    insertMetadataQuery.finish();

    // Stage 2.2 - add record to authors table
    success = false;
    for( author_it = metadata.authorsData.begin();
            author_it != metadata.authorsData.end();
            ++author_it
       )
    {
        insertAuthorQuery.prepare(insertAuthorQueryString);
        insertAuthorQuery.bindValue(":id",id);
        insertAuthorQuery.bindValue(":name",author_it.key());
        insertAuthorQuery.bindValue(":version",author_it.value());
        insertAuthorQuery.exec();
        if (!insertAuthorQuery.isActive()) {
            qDebug() << "Can't insert author into table. Error: " << insertAuthorQuery.lastError().text();

            success = false;
            break;
        }
        insertAuthorQuery.finish();
    }
    insertAuthorQuery.finish();

    

    // Stage 3 - create file in the specified folder
    filename = QString::number(id);
    file.setFileName(pathFolder + '/' + filename );
    file.open(QIODevice::WriteOnly);
    file.close();

    // Stage 4 - commit transaction
    if (!db.commit() ) {
        qDebug() << "Failed to commit changes. Error:" << db.lastError().text();
        db.rollback();
        return QString();
    }

    if ( idAnswer) 
        *idAnswer = id;
    
    return filename;

FAIL:
    db.rollback();
    if ( idAnswer)
        *idAnswer = -1;
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

    QString name = idCheckQuery.value(TABLE_INFO_COLUMN_NAME).toString();
    if ( name != ID_COLUMN_NAME ) {
        qDebug() << "id table has column named " << name << " instead of " << ID_COLUMN_NAME;
        return false;
    }

    QString type = idCheckQuery.value(TABLE_INFO_COLUMN_TYPE).toString();
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
        qDebug() << "Table has no column. Must have at least one column. SQL error:" <<
            resCheckQuery.lastError().text();
        return false;
    }

    // Check first column. It must has name ID_COLUMN_NAME and type INTEGER

    QString name = resCheckQuery.value(TABLE_INFO_COLUMN_NAME).toString();
    if ( name != ID_COLUMN_NAME ) {
        qDebug() << "resources table has column named " << name << " instead of " << ID_COLUMN_NAME;
        return false;
    }

    QString type = resCheckQuery.value(TABLE_INFO_COLUMN_TYPE).toString();
    if ( type != "INTEGER" ) {
        qDebug() << "resources table has column with type " << type << "instead of INTEGER";
        return false;
    }

    // As usual, nothing works in the world of sql databases
#if 0
    if ( !resCheckQuery.seek(1) ) {
        // Table has no second column
        qDebug() << "Table has one column. Must have at least two columns. SQL error:" <<
            resCheckQuery.lastError().text();
        return false;
    }

    name = resCheckQuery.value(TABLE_INFO_COLUMN_NAME).toString();
    if ( name != RES_COLUMN_NAME ) {
        qDebug() << "resources table has column named " << name << " instead of " << RES_COLUMN_NAME;
        return false;
    }

    type = resCheckQuery.value(TABLE_INFO_COLUMN_TYPE).toString();
    if ( type != "TEXT" ) {
        qDebug() << "resources table has column with type " << type << "instead of TEXT";
        return false;
    }

    // Check that there is no more columns
    if ( resCheckQuery.next() ) {
        qDebug() << "resources table has more than two columns";
        return false;
    }
#endif
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
    QSqlQuery tableExists(db);
    QSqlQuery createIdTable(db);
    QSqlQuery createResTable(db);
    QSqlQuery createMetaTable(db);
    QSqlQuery createAuthorTable(db);

    QString tableExistsString = "SELECT name FROM sqlite_master WHERE type='table' AND \
            name= :name";

    QSqlQuery enableForeignKeys(db);
    // Enable keys anyway
    enableForeignKeys.prepare("PRAGMA foreign_keys = ON");
    enableForeignKeys.exec();
    if (!enableForeignKeys.isActive()) {
        qDebug() << "Can't enable foreign keys in database";
        return false;
    }

    // Check whether id table already exists
    idTableExists.prepare(tableExistsString);
    idTableExists.bindValue(":name",ID_TABLE_NAME);

    idTableExists.exec();
    if (!idTableExists.isActive()) {
        qDebug() << "Can not execute query to check existance of id table. Error: " <<
            idTableExists.lastError().text();
        goto FAIL;
    }


    if (idTableExists.next()) {
        // Table exists
        if (!checkIdTable()) {
            qDebug() << "ID table exists, but has incorrect specification";
            goto FAIL;
        }
    }
    else {
        // Create table
        createIdTable.prepare("CREATE TABLE "ID_TABLE_NAME"( id INTEGER PRIMARY KEY )");
        createIdTable.exec();
        if (!createIdTable.isActive()) {
            qDebug() << "Can't create ID table. Error: " << 
                createIdTable.lastError().text();
            goto FAIL;
        }
            
    }
    idTableExists.finish();
    createIdTable.finish();

    /* ======= Table: resources  ====== */
    resTableExists.prepare(tableExistsString);
    resTableExists.bindValue(":name",RES_TABLE_NAME);

    resTableExists.exec();
    if (!resTableExists.isActive()) {
        qDebug() << "Can not execute query to check existance of resources table. Error: " <<
            resTableExists.lastError().text();
        goto FAIL;
    }

    if (resTableExists.next()) {
        // Table exists
        if (!checkResTable()) {
            qDebug() << "resources table exist, but has incorrect specification";
            goto FAIL;
        }
    }
    else {
        // Create table
        createResTable.prepare("CREATE TABLE "RES_TABLE_NAME"( id INTEGER, resource TEXT,  FOREIGN KEY(id) REFERENCES id(id) ON UPDATE CASCADE ON DELETE CASCADE DEFERRABLE INITIALLY  DEFFERED)");
        createResTable.exec();
        if (!createResTable.isActive()) {
            qDebug() << "Can't create resources table. Error: " << 
                createResTable.lastError().text();
            goto FAIL;
        }
    }

    resTableExists.finish();
    createResTable.finish();

    /* ======= Table: metadata ========= */
    tableExists.prepare(tableExistsString);
    tableExists.bindValue(":name",META_TABLE_NAME);

    tableExists.exec();
    if (!tableExists.isActive()) {
        qDebug() << "Can not execute query to check existance of metadata table. Error: " <<
            tableExists.lastError().text();
        goto FAIL;
    }

    if (tableExists.next()) {
        // Table exists
    }
    else {
        // Create table
        createMetaTable.prepare("CREATE TABLE "META_TABLE_NAME"( id INTEGER PRIMARY KEY, rank DOUBLE, description  TEXT,  FOREIGN KEY(id) REFERENCES id(id) ON UPDATE CASCADE ON DELETE CASCADE DEFERRABLE INITIALLY  DEFFERED)");
        createMetaTable.exec();
        if (!createMetaTable.isActive()) {
            qDebug() << "Can't create metadata table. Error: " << 
                createMetaTable.lastError().text();
            goto FAIL;
        }
    }

    tableExists.finish();
    createMetaTable.finish();


    /* ======== Table : author */
    tableExists.prepare(tableExistsString);
    tableExists.bindValue(":name",AUTHOR_TABLE_NAME);

    tableExists.exec();
    if (!tableExists.isActive()) {
        qDebug() << "Can not execute query to check existance of authors table. Error: " <<
            tableExists.lastError().text();
        goto FAIL;
    }

    if (tableExists.next()) {
        // Table exists
    }
    else {
        // Create table
        createAuthorTable.prepare("CREATE TABLE "AUTHOR_TABLE_NAME"( id INTEGER, name TEXT, version INTEGER,  FOREIGN KEY(id) REFERENCES id(id) ON UPDATE CASCADE ON DELETE CASCADE DEFERRABLE INITIALLY  DEFFERED)");
        createAuthorTable.exec();
        if (!createAuthorTable.isActive()) {
            qDebug() << "Can't create authors table. Error: " << 
                createAuthorTable.lastError().text();
            goto FAIL;
        }
    }

    tableExists.finish();
    createAuthorTable.finish();


    if (!db.commit() ) {
        qCritical() << "Can not prepare database: transaction commit fails.\n" << db.lastError().text();
        goto FAIL;
    }

    return true;
        

FAIL:
    idTableExists.finish();
    resTableExists.finish();
    tableExists.finish();
    createResTable.finish();
    createIdTable.finish();
    createMetaTable.finish();
    createAuthorTable.finish();
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

int Nepomuk::DecisionStorage::addDecision( const Decision::Decision & decision,
        const QList<QString> & uri,
        int * id)
{
    if (!d->valid)
        return Error::SystemError ;

    // Stage 1 - accquire filename
    QString filename = d->requestFileName(uri,decision.metadata(),id);
    
   if ( filename.isEmpty() ) {
      // Error
      return Error::DatabaseError;
   } 

   /*
   if ( id) 
       qDebug() << __func__ << "ID: " << *id;
       */

   // Open file
   QFile file(filename);
   file.open(QIODevice::WriteOnly);

   file.close();
   return Error::NoError;

}


bool Nepomuk::DecisionStorage::isValid() const
{ return d->valid; }


int 
Nepomuk::DecisionStorage::removeDecision(ID id)
{
    bool result = true;
    result = result && d->removeFromIndex(id);
    result = result && d->removeFromFolder(id);
    if ( !result )
        return Error::NoSuchDecision; 

    return Error::NoError;
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


bool Nepomuk::DecisionStorage::existsDecision( int id)
{
    bool answer = false;
    if ( id < 0 )
        return false;
    QSqlQuery checkQuery(d->db);
    checkQuery.prepare("SELECT * FROM "ID_TABLE_NAME" WHERE id = :id LIMIT 1");
    checkQuery.bindValue(":id",id);
    checkQuery.exec();
    if (!checkQuery.isActive()) {
        qDebug() << "Can't query database for existance of id. SQL: " << checkQuery.lastError().text();
        return false;
    }
    if ( checkQuery.next() ) {
        answer =  true;
    }
    checkQuery.finish();

    return answer;
}

DecisionMetadata Nepomuk::DecisionStorage::decisionMetadata(int  id, int & error)
{
    DecisionMetadata answer;
    // Check that id is in index
    bool exists = this->existsDecision(id);
    if (!exists) {
        error = Error::NoSuchDecision;
        return answer;
    }

    // Look for the file
    QFile file(d->pathFolder + QString::number(id));
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text) ) {
        qDebug() << "Can't open file for " << id << "for reading.";
        // Remove it from index

        // report an error
        error = Error::NoSuchDecision;
        return answer;
    }

    // Now read metadata from the index
    QSqlQuery readMetaQuery(d->db);
    readMetaQuery.prepare(
            QString("SELECT (rank,description) FROM "META_TABLE_NAME" WHERE id = %1").arg(id));
    readMetaQuery.exec();
    if (!readMetaQuery.isActive() ) {
        qDebug() << "Can't execute select metadata query. Error is " <<
            readMetaQuery.lastError().text();
    }
    if ( readMetaQuery.next() ) {
        answer.description = readMetaQuery.value(1).toString();
        answer.rank = readMetaQuery.value(0).toDouble();
    }

    QSqlQuery readAuthorsQuery(d->db);
    readAuthorsQuery.prepare(
        QString("SELECT (author,version) FROM "AUTHOR_TABLE_NAME" WHERE id = %1").arg(id)
        );
    readAuthorsQuery.exec();
    if (!readAuthorsQuery.isActive() ) {
        qDebug() << "Can't execute selet  authors query. Error is " <<
            readAuthorsQuery.lastError().text();
    }
    while(readAuthorsQuery.next()) 
    {
        QString name = readAuthorsQuery.value(0).toString();
        int version = readAuthorsQuery.value(1).toInt();
        answer.authorsData[name] = version;
    }

    return answer;
}

Nepomuk::DecisionStorage::~DecisionStorage()
{;}

// vim:sw=4 ts=8 expandtab
