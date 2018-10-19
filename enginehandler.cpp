#include <QDebug>
#include <QtCore/QJsonDocument>
#include <QtCore/QJsonObject>
#include <iostream>

#include "enginehandler.h"

EngineHandler::EngineHandler(const QString &programPath, const QStringList &arguments) :
	programPath(programPath),
	arguments(arguments) {
	QObject *parent = nullptr;

	engineProcess = new QProcess(parent);

	connect(engineProcess, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished), this, &EngineHandler::settingResult_slot);

	engineProcess->start(programPath, arguments);

	result = new QJsonObject();
}

EngineHandler::~EngineHandler() {
	delete engineProcess;
	delete result;
}

const QJsonObject &EngineHandler::getResult() const {
	return *result;
}

void EngineHandler::settingResult_slot() {
	QString tempString = engineProcess->readAllStandardOutput();

	*result = QJsonDocument::fromJson(tempString.toUtf8()).object();

	QJsonDocument qJsonDocument(*result);

	qDebug() << qJsonDocument.toJson();
}
