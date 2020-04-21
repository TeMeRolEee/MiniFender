#include <QDebug>
#include <QtCore/QJsonDocument>
#include <QtCore/QJsonObject>
#include <QUuid>

#include "enginehandler.h"

EngineHandler::EngineHandler() {
	engineList = new QMap<int, Engine *>();
	resultMap = new QMap<QString, QJsonArray *>();
	scanIdList = new QVector<int>();
}

EngineHandler::~EngineHandler() {
	delete engineList;
	delete resultMap;
	delete scanIdList;
}

void EngineHandler::run() {
	QThread::run();
}

void EngineHandler::handleEngineResult_slot(QUuid uniqueId, QJsonObject result) {
	//qInfo() << "[" << __FUNCTION__  << "|" << __FILE__ << "]" << QJsonDocument(result).toJson(QJsonDocument::JsonFormat::Compact);
	emit scanComplete_signal(uniqueId, std::move(result));
}

void EngineHandler::addNewEngine_slot(const QString &enginePath, const QString &engineName) {
	//qInfo() << "[" << __FUNCTION__  << "|" << __FILE__ << "]";
	if (!enginePath.isEmpty() && !findExistingEngine(engineName)) {
		auto engine = new Engine(++engineCount, enginePath, engineName);
		engineList->insert(engineCount, engine);
		engineNameList.insert(engineName, engineCount);
		//qInfo() << "[" << __FUNCTION__  << "|" << __FILE__ << "]" << (engineList->keys()) << (engineList->values());
		engine->start();

		connect(engine, &Engine::processDone_signal, this, &EngineHandler::handleEngineResult_slot,Qt::QueuedConnection);
		connect(this, &EngineHandler::startEngine_signal, engine, &Engine::addNewWorker_slot, Qt::QueuedConnection);
		connect(this, &EngineHandler::newTask_signal, engine, &Engine::startScan_slot, Qt::QueuedConnection);
		connect(engine, &Engine::finished, engine, &Engine::deleteLater);
		connect(engine, &Engine::engineInitFinished_signal, this, &EngineHandler::handleEngineInit_slot);

		emit startEngine_signal();
	} else {
		qCritical() << "[" << __FUNCTION__  << "|" << __FILE__ << "]" << "Engine could not be added for some reason";
	}
}

void EngineHandler::handleNewTask_slot(QUuid uniqueId, const QString &file) {
	//qInfo() << "[" << __FUNCTION__  << "|" << __FILE__ << "]";

	if (!file.isEmpty()) {
		emit newTask_signal(uniqueId, file);
	}
}

bool EngineHandler::findExistingEngine(const QString &engineName) {
	//qInfo() << "[" << __FUNCTION__  << "|" << __FILE__ << "]";
	return engineNameList.find(engineName) != engineNameList.end();
}

int EngineHandler::getEngineCount() {
	//qInfo() << "[" << __FUNCTION__  << "|" << __FILE__ << "]";
	return engineCount;
}

void EngineHandler::handleEngineInit_slot(bool success, const QString &engineName) {
	//qInfo() << "[" << __FUNCTION__  << "|" << __FILE__ << "]" << success;
	if (!success) {
		engineList->value(engineNameList.value(engineName))->quit();
		engineList->value(engineNameList.value(engineName))->wait();

		delete engineList->take(engineNameList.value(engineName));
		engineNameList.take(engineName);
		--engineCount;
	}

	emit engineInited_signal();
}

