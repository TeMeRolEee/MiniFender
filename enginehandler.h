#pragma once

#include <QtCore/QObject>
#include <QtCore/QProcess>

#include "engine.h"

class EngineHandler : public QObject {
	Q_OBJECT

public:
	EngineHandler();

    bool addNewEngine(const QString &enginePath, const QStringList &parameterList);

	~EngineHandler() override;

private:
	QMap<int, Engine*> *engineList;
	int engineCount = 0;

private slots:

signals:

	void reportSuccesssResult_signal();

	void reportFailureResult_signal();
};


