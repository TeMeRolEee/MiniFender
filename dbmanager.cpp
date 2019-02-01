#include "dbmanager.h"

#include <QDebug>
#include <QtSql/QSqlQuery>

DBManager::DBManager(const QString &path) : dataBaseFilePath(path) {

}

bool DBManager::init() {
    database = QSqlDatabase::addDatabase("QSQLITE");
    database.setDatabaseName(dataBaseFilePath);

    if (database.open()) {
        QSqlQuery createScanHistoryTable("");
        return true;
    }
    qDebug() << "[DBMANAGER]\t" << "Couldn't connect to database";
    return false;
}


