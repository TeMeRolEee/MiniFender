#include <QtCore/QFile>
#include <QtCore/QJsonDocument>

#include "readconfiguration.h"


int ReadConfiguration::readConfigurationFile(const QString &filePath = "config.conf") {
	QFile configFile(filePath);
	QByteArray fileContent = nullptr;

	if (configFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
		fileContent = configFile.readAll();
		configFile.close();
		*configuration = QJsonDocument::fromJson(fileContent).object();
		return 0;
	}
	return 1;

}

ReadConfiguration::~ReadConfiguration() {
	delete configuration;
}

QJsonObject ReadConfiguration::getEngineDatas() {
	auto *engines = new QJsonObject();

	if (configuration != nullptr) {
		for (auto engine : *configuration) {
			QJsonObject temp = engine.toObject();
			engines->insert(temp.value("engineName").toString(), temp.value("enginePath").toString());
		}
		return *engines;
	}

	return QJsonObject();
}


