#include "dbmanager.h"

#include <QDebug>
#include <QtSql/QSqlQuery>

DBManager::DBManager(const QString &path) : dataBaseFilePath(path) {

}

bool DBManager::init() {
    database = QSqlDatabase::addDatabase("QSQLITE");
    database.setDatabaseName(dataBaseFilePath);

    if (database.open()) {
        QSqlQuery createScanHistoryTable(
                "CREATE TABLE \"scanHistory\" ( `id` INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT UNIQUE, `scanResult` INTEGER NOT NULL DEFAULT 0, `engineResults` TEXT NOT NULL )");
        /*
         *  NOT USING AT THE MOMENT
         *  QSqlQuery createEngineTable(
         *      "CREATE TABLE IF NOT EXISTS `engines` ( `id` INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT UNIQUE, `name` TEXT NOT NULL UNIQUE )");
        */
        return (createScanHistoryTable.exec());
    }
    qDebug() << "[DBMANAGER]\t" << "Couldn't connect to database";
    return false;
}

QJsonObject DBManager::getLatestScanData() {
    QSqlQuery getLatestScan("SELECT scanResult, engineResults, engineResults, scanDate FROM scanHistory LIMIT 1");

    return QJsonObject();
}

QJsonArray DBManager::getHistory(int lastX) {
    return QJsonArray();
}


