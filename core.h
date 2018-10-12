#pragma once

#include <QtCore/QObject>
#include "enginehandler.h"

class Core : public QObject {
	Q_OBJECT
public:
	bool addNewEngine(const QString &enginePath);

private:
	int engineIdIncrementer;
	QMap<int, EngineHandler*> *engineHandler;
};

