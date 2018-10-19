#include "core.h"
#include <chrono>
#include <iostream>

bool Core::addNewEngine(const QString &enginePath, const QStringList &parameterList) {
	auto *newEngine = new EngineHandler(enginePath, parameterList);

	return false;
}
