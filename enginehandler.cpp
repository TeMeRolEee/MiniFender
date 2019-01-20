#include <iostream>

#include <QDebug>
#include <QtCore/QJsonDocument>
#include <QtCore/QJsonObject>

#include "enginehandler.h"

EngineHandler::EngineHandler() {
    engineList = new QMap<int, Engine*>();
}

EngineHandler::~EngineHandler() {
	delete engineList;
}

void EngineHandler::run() {
	qDebug() << "[ENGINE_HANDLER]\t" << "Running";
	QThread::run();
}

void EngineHandler::handleEngineResult_slot(QJsonObject result) {

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

void EngineHandler::addNewEngine_slot(const QString &enginePath) {
	if (!enginePath.isEmpty()) {
		auto engine = new Engine(engineCount++, enginePath);
		connect(engine, &Engine::processDone_signal, this, &EngineHandler::handleEngineResult_slot);
		engine->start();
	}
}
