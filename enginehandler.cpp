#include <iostream>

#include <QDebug>
#include <QtCore/QJsonDocument>
#include <QtCore/QJsonObject>

#include "enginehandler.h"

EngineHandler::EngineHandler() {
    engineList = new QMap<int, Engine*>();
    resultMap = new QMap<QString, QJsonArray*>();
    scanIdList = new QVector<int>();
}

EngineHandler::~EngineHandler() {
	delete engineList;
	delete resultMap;
	delete scanIdList;
}

void EngineHandler::run() {
	qDebug() << "[ENGINE_HANDLER]\t" << "Running";
	QThread::run();
}

void EngineHandler::handleEngineResult_slot(QJsonObject result) {
	qDebug() << "[ENGINE_HANDLER]\t" << QJsonDocument(result).toJson(QJsonDocument::JsonFormat::Compact);
	emit scanComplete_signal(result);
}

void EngineHandler::deleteEngineHandler_slot() {
	for (auto engineID : engineList->keys()) {
		emit engineList->value(engineID)->deleteEngine_signal();
	}
}

void EngineHandler::addNewEngine_slot(const QString &enginePath, const QString &scanParameter, const QString &engineName) {
	if (!enginePath.isEmpty() && !findExistingEngine(engineName)) {
		qDebug() << "[ENGINE_HANDLER]\t" << "ADDING ENGINE:\t" << engineCount;

		auto engine = new Engine(engineCount, enginePath, scanParameter);
		connect(engine, &Engine::processDone_signal, this, &EngineHandler::handleEngineResult_slot, Qt::QueuedConnection);
		connect(this, &EngineHandler::newTask_signal, engine, &Engine::addNewWorker_slot, Qt::QueuedConnection);
		connect(engine, &Engine::finished, engine, &Engine::deleteLater);
		//connect(engine, &Engine::deletingDone_signal, this, &EngineHandler::handleEngineDeletion_slot);
		engineList->insert(engineCount, engine);
		engineNameList.insert(engineName, engineCount++);
		engine->start();
	}
}

void EngineHandler::handleNewTask_slot(QUuid uniqueId, const QString &file) {
	qDebug() << "[ENGINE_HANDLER]\t" << "ENGINE_COUNT:\t" << engineList->count();
	if (!file.isEmpty()) {
		scanIdList->push_back(scanId);
		emit newTask_signal(scanId++, file);
	}
}

bool EngineHandler::findExistingEngine(const QString &engineName) {
	return engineNameList.find(engineName) != engineNameList.end();
}

int EngineHandler::getEngineCount() {
	return engineList->count();
}

void EngineHandler::handleEngineDeletion_slot(int id) {
	//engineList->value(id)->quit();
	//engineList->value(id)->wait();

	//delete engineList->take(id);
}
