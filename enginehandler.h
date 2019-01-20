#pragma once

#include <QtCore/QObject>
#include <QtCore/QProcess>

#include "engine.h"

class EngineHandler : public QObject {
	Q_OBJECT

public:
	EngineHandler();

	~EngineHandler() override;

private:
	void addNewEngine(const QString &enginePath);

	void removeEngine(int id);

	QMap<int, Engine*> *engineList;
	int engineCount = 0;

private slots:
	void handleEngineResult_slot(QJsonObject result);

signals:
	//void reportResult_signal(QJsonArray results);

};


