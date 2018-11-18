#pragma once

#include <QtCore/QJsonObject>

class ReadConfiguration {
public:
	~ReadConfiguration();

	int readConfigurationFile(const QString &filePath);

	QJsonObject getEngineDatas();

private:
	QJsonObject *configuration;
};
