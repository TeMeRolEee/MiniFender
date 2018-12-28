#pragma once

#include <QtCore/QObject>
#include <QtCore/QProcess>

#include "engine.h"

class EngineHandler : public QObject {
	Q_OBJECT

public:
	EngineHandler(const QString &programPath, const QStringList &arguments);

    bool addNewEngine(const QString &enginePath, const QStringList &parameterList);

	~EngineHandler() override;

private:
	QString programPath;
	QStringList arguments;
	QVector<Engine*> *engineList;
	QJsonObject *result;
	int engineCount = 0;

private slots:

	void settingResult_slot();

signals:

	void reportSuccesssResult_signal();

	void reportFailureResult_signal();
};


