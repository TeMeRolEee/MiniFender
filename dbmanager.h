#pragma once

#include <QtCore/QString>
#include <QtSql/QSqlDatabase>

class DBManager {
public:
    DBManager(const QString &path);

    bool init();
private:
    QString dataBaseFilePath;
    QSqlDatabase database;
};


