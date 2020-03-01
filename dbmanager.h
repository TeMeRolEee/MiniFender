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

	bool init();

private:
	QString dataBaseFilePath;

	QSqlDatabase *database = nullptr;

	QJsonArray getLastXScan(int lastX = 1);

public slots:

	void init_slot();

	void addScanData_slot(const QJsonObject &data);

signals:

	void init_signal();

	void initDone_signal(bool success);

	void addScanData_signal(const QJsonObject &data);

	void addScanDataResult_signal(bool success);

};


