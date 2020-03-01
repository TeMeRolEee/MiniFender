#include "dbmanager.h"

#include <QDebug>
#include <QtSql>
#include <QJsonDocument>

DBManager::DBManager(const QString &path) : dataBaseFilePath(path) {
	connect(this, &DBManager::init_signal, this, &DBManager::init_slot, Qt::QueuedConnection);
}

bool DBManager::init() {
	emit init_signal();
}

QJsonArray DBManager::getLastXScan(int lastX) {
	if (database->open()) {
		QSqlQuery query;
		query.prepare("SELECT scanResult, engineResults, scanDate FROM scanHistory ORDER BY id DESC LIMIT (:limit)");
		query.bindValue(":limit", lastX);

		QJsonArray resultArray;
		if (query.exec()) {
			while (query.next()) {
				QJsonObject data;
				int scanResult = query.value(0).toInt();
				QJsonObject engineResults = query.value(1).toJsonObject();
				int scanDate = query.value(2).toInt();
				data.insert("scanResult", scanResult);
				data.insert("engineResults", engineResults);
				data.insert("scanDate", scanDate);
				resultArray.push_back(data);
			}
			return resultArray;
		}
		database->close();
	}
	return QJsonArray();
}

void DBManager::addScanData_slot(const QJsonObject &data) {
	if (!data.isEmpty() && database->open()) {
		QSqlQuery dbQuery;
		QJsonObject engines;
		engines.insert("engineResults", data.value("engineResults"));
		dbQuery.prepare(
				"INSERT INTO scanHistory (scanResult, engineResults, scanDate) VALUES ((:scanResult), (:engineResults), (:scanDate))");
		dbQuery.bindValue(":scanResult", data.value("scanResult").toInt());
		dbQuery.bindValue(":engineResults", QJsonDocument(engines).toJson(QJsonDocument::JsonFormat::Compact));
		dbQuery.bindValue(":scanDate", data.value("scanDate").toInt());

		bool queryResult;
		queryResult = dbQuery.exec();
		if (!queryResult) {
			qDebug() << queryResult << dbQuery.lastError();
		}
		database->close();
		emit addScanDataResult_signal(queryResult);
	}
	emit addScanDataResult_signal(false);
}

DBManager::~DBManager() {
	delete database;
}

void DBManager::init_slot() {
	database = new QSqlDatabase();
	QSqlDatabase::addDatabase("QSQLITE");
	database->setDatabaseName(dataBaseFilePath);

	if (database->open()) {
		QSqlQuery createScanHistoryTable(
				"CREATE TABLE IF NOT EXISTS \"scanHistory\" ( `id` INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT UNIQUE, `scanResult` INTEGER NOT NULL DEFAULT 0, `engineResults` TEXT NOT NULL, `scanDate` INTEGER NOT NULL )");

		bool initOkay = createScanHistoryTable.exec();
		database->close();
		if (!initOkay) {
			delete database;
		}
		emit initDone_signal(initOkay);
	}
	emit initDone_signal(false);
}


