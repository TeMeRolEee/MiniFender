#include "core.h"

#include <chrono>
#include <mutex>
#include <iostream>

#include <QDebug>
#include <QtCore/QJsonDocument>
#include <QtCore/QSettings>
#include <QtCore/QFile>
#include <QtCore/QMap>
#include <QtCore/QDateTime>
#include <QtCore/QDir>
#include <QtCore/QTimer>

Core::Core(const QString &rootDirectory) : rootDirectory(rootDirectory) {
	if (!QDir(rootDirectory + "/db/").exists()) {
		QDir().mkdir(rootDirectory + "/db/");
	}

	if (!QDir(rootDirectory + "/settings/").exists()) {
		QDir().mkdir(rootDirectory + "/settings/");
	}
	dbManager = new DBManager(rootDirectory + "/db/scanHistoryDB.sqlite");
	engineHandler = new EngineHandler();
	cliHandler = new CliHandler();
	authClient = new AuthClient();
}

Core::~Core() {
	emit removeEngines_signal();
	emit cliHandler->stopCli_signal();

	engineHandler->quit();
	engineHandler->wait();
	cliHandler->quit();
	cliHandler->wait();
	authClient->quit();
	authClient->wait();

	delete authClient;
	delete dbManager;
	delete scanMap;
	delete isChecked;
}

bool Core::init(const QString &settingsFilePath) {
	connect(this, &Core::startCalculateResult_signal, this, &Core::result_slot, Qt::QueuedConnection);
	connect(this, &Core::finished, this, &Core::deleteLater);

	if (!dbManager->init()) {
		return false;
	}

	scanMap = new QMap<QUuid, QJsonObject>();

	engineHandler->start();

	connect(this, &Core::addNewEngine_signal, engineHandler, &EngineHandler::addNewEngine_slot, Qt::QueuedConnection);
	connect(this, &Core::startNewScanTask_signal, engineHandler, &EngineHandler::handleNewTask_slot,Qt::QueuedConnection);
	connect(engineHandler, &EngineHandler::scanComplete_signal, this, &Core::handleEngineResults_slot,Qt::QueuedConnection);
	connect(engineHandler, &EngineHandler::finished, engineHandler, &EngineHandler::deleteLater);
	connect(engineHandler, &EngineHandler::engineInited_signal, this, &Core::handleEngineInit_slot);

	cliHandler->start();

	connect(cliHandler, &CliHandler::newTask_signal, this, &Core::handleNewTask_slot, Qt::QueuedConnection);
	connect(cliHandler, &CliHandler::finished, cliHandler, &CliHandler::deleteLater);
	connect(cliHandler, &CliHandler::stopCli_signal, cliHandler, &CliHandler::stopCli_slot);

	if (!readSettings(settingsFilePath)) {
		qCritical() << "[CORE]\t" << "Could not load any engine. Shutting down!";
		return false;
	}

	return true;
}

void Core::handleEngineResults_slot(QUuid uniqueId, const QJsonObject &result) {
	auto it = scanMap->find(uniqueId);
	if (it != scanMap->end()) {
		QJsonArray temp = it->find("engineResults").value().toArray();
		temp.push_back(result);
		it->insert("engineResults", temp);
	}

	if (scanMap->value(uniqueId).value("engineResults").toArray().count() == engineHandler->getEngineCount()) {
		emit startCalculateResult_signal(uniqueId);
	}
}

void Core::run() {
	QThread::run();
}

bool Core::readSettings(const QString &filePath) {
	QVector<QString> badEngines;
	QSettings settings(filePath, QSettings::IniFormat);
	QStringList keys = settings.childGroups();

	for (const auto &engineName : keys) {
		settings.beginGroup(engineName);
		QMap<QString, QString> engineData;

		if (settings.childKeys().contains("path")) {
			for (const auto &key : settings.childKeys()) {
				engineData.insert(key, settings.value(key).toString());
			}
			//qInfo() << "adding engine:" << engineName << engineData.value("path");
			emit addNewEngine_signal(engineData.value("path"), engineName);
		} else {
			badEngines.append(engineName);
			qWarning() << "[CORE]\t" << "Engine:" << engineName << "cannot be started.\t Skipping";
		}
		settings.endGroup();
	}

	if (badEngines.count() > 0) {
		QString message = "Engines that could not be loaded:";
		for (const auto &engine : badEngines) {
			message.append(" " + engine);
		}
		qWarning() << "[CORE]\t" << message;
	}

	return (badEngines.count() != settings.childGroups().count() && settings.childGroups().count() != 0);
}

void Core::listEngineCount() {
	qInfo() << "[CORE]\t" << "Engine count is:\t" << engineHandler->getEngineCount();
}

void Core::handleNewTask_slot(const QString &input) {
	//qInfo() << "[" << __FUNCTION__  << "|" << __FILE__ << "]" << input;
	if (QFile::exists(input)) {
		QUuid uniqueId = QUuid::createUuid();

		QJsonObject initialData;
		auto *initialArray = new QJsonArray();
		initialData.insert("scanDate", QDateTime::currentMSecsSinceEpoch());
		initialData.insert("engineResults", *initialArray);
		scanMap->insert(uniqueId, initialData);

		emit startNewScanTask_signal(uniqueId, input);
	} else {
		qCritical() << "[CORE]\t" << "ERROR:\t" << input << "\t not found. Scan cannot be performed.";
	}
}

void Core::result_slot(QUuid id) {
	//qInfo() << "[" << __FUNCTION__  << "|" << __FILE__ << "]";
	QJsonObject finalResult = calculateResult(id);

	std::cout << QJsonDocument(finalResult).toJson(QJsonDocument::JsonFormat::Compact).toStdString() << std::endl;
	std::flush(std::cout);

	dbManager->addScanData(finalResult);

	scanMap->take(id);
}

QJsonObject Core::calculateResult(QUuid id) {
	//qInfo() << "[CORE]\t" << "Calculating results";
	int infectedCount = 0;
	auto scanResultObject = scanMap->value(id);
	QJsonObject finalResult;
	finalResult.insert("fileName", scanResultObject.value("engineResults").toArray().at(0).toObject().value("scan_result").toArray().at(0).toObject().value("filename")); ///TODO remove unnecessary json array from engine result

	for (auto results : scanResultObject.value("engineResults").toArray()) {
		auto scanResult = results.toObject().value("scan_result").toArray();
		if (scanResult.at(0).toObject().value("verdict").toInt() == 1) {
			infectedCount++;
		}
	}

	if (infectedCount > 0) {
		finalResult.insert("scanResult", 1);
	} else {
		finalResult.insert("scanResult", 0);
	}

	int64_t scanDateTime = scanMap->value(id).value("scanDate").toDouble();
	int64_t currentTime = QDateTime::currentMSecsSinceEpoch();
	int scanTime = currentTime - scanDateTime;

	finalResult.insert("scanTime", scanTime);

	return finalResult;
}

void Core::handleAuthenticationResponse_slot(bool isGood) {
	if (!*isChecked) {
		if (!isGood) {
			*isRegistered = false;
			qCritical() << "Critical error: not a valid serial key.";
		} else {
			*isRegistered = true;
			qInfo() << "Info: serial key verified and accepted";
		}
	}
	*isChecked = true;
}

bool Core::parseSerial(const QString &filePath, bool *isRegistered, bool *checked) {
	this->isChecked = checked;
	this->isRegistered = isRegistered;

	authClient->start();

	connect(authClient, &AuthClient::finished, authClient, &AuthClient::deleteLater, Qt::UniqueConnection);
	connect(authClient, &AuthClient::recievedResponse, this, &Core::handleAuthenticationResponse_slot,Qt::UniqueConnection);
	connect(this, &Core::sendSerialKey_signal, authClient, &AuthClient::sendSerialKey_slot, Qt::UniqueConnection);

	if (!QFile(filePath).exists()) {
		return false;
	}

	auto *settings = new QSettings(filePath, QSettings::IniFormat);
	QStringList keys = settings->childGroups();

	if (!keys.contains("Authentication")) {
		return false;
	}

	settings->beginGroup("Authentication");
	QString serial = settings->value("serial").toString();
	QString url = settings->value("url").toString();
	int port = settings->value("port").toInt();
	qInfo() << "[" << "INFO:" << "]" << serial << url << port;

	if (serial.isEmpty() || url.isEmpty() || (port < 1 || port > 65535)) {
		//qDebug() << "[CORE]\t" << "Wrong auth settings";
		return false;
	}

	if (!authClient->init(url, port)) {
		return false;
	}

	emit sendSerialKey_signal(serial);

	return true;
}

void Core::handleEngineInit_slot() {
	qInfo() << "[" << __FUNCTION__  << "|" << __FILE__ << "]" << "[CORE]\t" << "Engine inited" << "engine count is:" << engineHandler->getEngineCount();
}

