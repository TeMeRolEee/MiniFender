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

void EngineHandler::addNewEngine(const QString &enginePath) {

	auto engine = new Engine(engineCount++, enginePath);

	engine->start();

	/*auto *newEngine = new Engine(this, engineCount, enginePath);

	engineList->insert(engineCount++, newEngine);

	//connect(this, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished), this, &EngineHandler::settingResult_slot);

	//engineList->start(programPath, arguments);*/
}

void EngineHandler::handleEngineResult_slot(QJsonObject result) {

}

void EngineHandler::removeEngine(int id) {
	emit engineList->value(id)->deleteEngine_signal();
	engineList->value(id)->quit();
	engineList->value(id)->wait();

	delete engineList->take(id);
}
