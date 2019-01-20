#include "core.h"
#include <chrono>
#include <iostream>

bool Core::addNewEngine(const QString &enginePath, const QStringList &parameterList) {

	return false;
}

void Core::init() {
    engineHandler = new EngineHandler();
    engineHandler->start();
}

void Core::handleEngineHandler_slot(QJsonObject result) {

}
