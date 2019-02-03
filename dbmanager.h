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

    QJsonArray getHistory(int lastX = 1);

private:
    QString dataBaseFilePath;
    QSqlDatabase database;
};


