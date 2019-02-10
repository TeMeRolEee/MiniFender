#include "dbmanager.h"

#include <QDebug>
#include <QtSql>
#include <QJsonDocument>

DBManager::DBManager(const QString &path) : dataBaseFilePath(path) {

}

bool DBManager::init() {
    database = QSqlDatabase::addDatabase("QSQLITE");
    database.setDatabaseName(dataBaseFilePath);

    if (database.open()) {
        QSqlQuery createScanHistoryTable(
                "CREATE TABLE IF NOT EXISTS \"scanHistory\" ( `id` INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT UNIQUE, `scanResult` INTEGER NOT NULL DEFAULT 0, `engineResults` TEXT NOT NULL, `scanDate` INTEGER NOT NULL )");
        /*
         *  NOT USING AT THE MOMENT
         *  QSqlQuery createEngineTable(
         *      "CREATE TABLE IF NOT EXISTS `engines` ( `id` INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT UNIQUE, `name` TEXT NOT NULL UNIQUE )");
        */
        bool initOkay = createScanHistoryTable.exec();
        database.close();
        return (initOkay);
    }
    //qDebug() << "[DBMANAGER]\t" << "Couldn't connect to database";
    return false;
}

QJsonArray DBManager::getLastXScan(int lastX) {
    if (database.open()) {
        QSqlQuery query;
        query.prepare("SELECT scanResult, engineResults, scanDate FROM scanHistory ORDER BY id DESC LIMIT (:limit)");
        query.bindValue(":limit", lastX);

        QJsonArray resultArray;
        if (query.exec()) {
            while (query.next()) {
                QJsonObject data;
                QMapIterator<QString, QVariant> mapIterator(query.boundValues());
                while (mapIterator.hasNext()) {
                    mapIterator.next();
                    data.insert(mapIterator.key(), (const QJsonValue &) mapIterator.value());
                }
                resultArray.insert(resultArray.count(), data);
            }
            //qDebug() << "[DBMANAGER]" << resultArray;
            return resultArray;
        }
        database.close();
    }
    return QJsonArray();
}

bool DBManager::addScanData(const QJsonObject &data) {
    if (!data.isEmpty() && database.open()) {
        QSqlQuery dbQuery;
        QJsonObject engines;
        engines.insert("engineResults", data.value("engineResults"));
        dbQuery.prepare("INSERT INTO scanHistory (scanResult, engineResults, scanDate) VALUES ((:scanResult), (:engineResults), (:scanDate))");
        dbQuery.bindValue(":scanResult", data.value("scanResult").toInt());
        dbQuery.bindValue(":engineResults", QJsonDocument(engines).toJson(QJsonDocument::JsonFormat::Compact));
        dbQuery.bindValue(":scanDate", data.value("scanDate").toInt());

        bool queryResult;
        queryResult = dbQuery.exec();
        if (!queryResult) {
            qDebug() << queryResult << dbQuery.lastError();
        }
        database.close();
        return queryResult;
    }
    return false;
}


