#include <iostream>

#include <QDebug>
#include <QtCore/QJsonDocument>
#include <QtCore/QJsonObject>

#include "enginehandler.h"

EngineHandler::EngineHandler() {
    engineList = new QMap<int, Engine*>();
    resultArray = new QMap<QString, QJsonArray*>();
}

EngineHandler::~EngineHandler() {
	delete engineList;
	delete resultArray;
}

void EngineHandler::run() {
	qDebug() << "[ENGINE_HANDLER]\t" << "Running";
	QThread::run();
}

void EngineHandler::handleEngineResult_slot(QJsonObject result) {
	qDebug() << "[ENGINE_HANDLER]\t" << QJsonDocument(result).toJson(QJsonDocument::JsonFormat::Compact);
	emit scanComplete_signal(result);
}

void EngineHandler::removeEngine(int id) {
	emit engineList->value(id)->deleteEngine_signal();
	engineList->value(id)->quit();
	engineList->value(id)->wait();

	delete engineList->take(id);
}

void EngineHandler::deleteEngineHandler_slot() {
	for (auto engineID : engineList->keys()) {
		removeEngine(engineID);
	}
}

void EngineHandler::addNewEngine_slot(const QString &enginePath, const QString &scanParameter) {
	if (!enginePath.isEmpty() && !findExistingEngine(enginePath)) {
		auto engine = new Engine(engineCount, enginePath, scanParameter);
		qDebug() << "[ENGINE_HANDLER]\t" << "ADDING ENGINE:\t" << engineCount;
		connect(engine, &Engine::processDone_signal, this, &EngineHandler::handleEngineResult_slot);
		engineList->insert(engineCount, engine);
		enginePathList.insert(enginePath, engineCount++);
		engine->start();
	}
	for (const auto &engine : enginePathList.keys()) {
		qDebug() << "[ENGINE_HANDLER]\t" << "ENGINE:\t" << engine;
	}
}

void EngineHandler::handleNewTask_slot(const QString &task) {
	qDebug() << "[ENGINE_HANDLER]\t" << "ENGINE_COUNT:\t" << engineList->count();
	for (const auto &engine : engineList->keys()) {
		qDebug() << "[ENGINE_HANDLER]\t" << "ENGINE:\t" << engine;
	}
	if (!task.isEmpty()) {/*
		for (const auto &path : task.keys()) {
			qDebug() << "[ENGINE_HANDLER]\t" << "PATH:\t" << path;
			if (findExistingEngine(path)) {
				qDebug() << "[ENGINE_HANDLER]\t" << "ID:\t" << enginePathList.value(path);
				int engineID = enginePathList.value(path);
				emit engineList->value(engineID)->addNewWorker_signal(path);
			}
		}*/
		for (auto engineID : engineList->keys()) {
			emit engineList->value(engineID)->addNewWorker_signal(task);
		}
	}
}

bool EngineHandler::findExistingEngine(const QString &enginePath) {
	for (const auto &engine : enginePathList.keys()) {
		if (enginePath == engine) {
			return true;
		}
	}
	return false;
}

int EngineHandler::getEngineCount() {
	return engineList->count();
}
