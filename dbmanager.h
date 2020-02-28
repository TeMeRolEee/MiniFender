#pragma once

#include <QtCore/QString>
#include <QtSql/QSqlDatabase>
#include <QtCore/QJsonObject>
#include <QtCore/QJsonArray>
#include <QtCore/QThread>

class DBManager : public QThread {
	Q_OBJECT
public:
	DBManager(const QString &path);
	~DBManager();

private:
	QString dataBaseFilePath;
	QSqlDatabase *database;

	bool init();
	bool addScanData(const QJsonObject &data);
	QJsonArray getLastXScan(int lastX = 1);

public slots:
	void init_slot();

signals:
	void init_signal();
	void initDone_signal(bool success);

};


