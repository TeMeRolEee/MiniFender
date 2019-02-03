#pragma once

#include <QtCore/QString>
#include <QtSql/QSqlDatabase>
#include <QtCore/QJsonObject>
#include <QtCore/QJsonArray>

class DBManager {
public:
    DBManager(const QString &path);

    bool init();

    bool addScanData(const QJsonObject data);

    QJsonObject getLatestScanData();

    QJsonArray getHistory(int lastX);

private:
    QString dataBaseFilePath;
    QSqlDatabase database;
};


