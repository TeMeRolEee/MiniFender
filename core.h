#pragma once

#include <QtCore/QObject>

#include "enginehandler.h"

class Core : public QObject {
Q_OBJECT
public:
	bool addNewEngine(const QString &enginePath, const QStringList &parameterList);

private:
	int engineIdIncrementer = 0;
	QMap<int, EngineHandler *> *engineHandler;
};

