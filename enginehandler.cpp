#include <iostream>

#include <QDebug>
#include <QtCore/QJsonDocument>
#include <QtCore/QJsonObject>

#include "enginehandler.h"

EngineHandler::EngineHandler() {

    //engineList = new QVector<Engine*>();

	//result = new QJsonObject();
}

EngineHandler::~EngineHandler() {
	delete engineList;
	//delete result;
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
