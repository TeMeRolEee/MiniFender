#include "enginehandler.h"

EngineHandler::EngineHandler(const QString &programPath, const QStringList &arguments) :
	programPath(programPath),
	arguments(arguments) {
	QObject *parent = nullptr;

	engineProcess = new QProcess(parent);

	connect(engineProcess, &QProcess::finished, this, &EngineHandler::settingResult_slot);

	engineProcess->start(programPath, arguments);

}

EngineHandler::~EngineHandler() {
	delete engineProcess;
}

const QString &EngineHandler::getResult() const {
	return result;
}

void EngineHandler::settingResult_slot() {
	result = engineProcess->exitStatus();
}
