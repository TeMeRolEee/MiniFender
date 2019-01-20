#pragma once

#include <QtCore/QObject>
#include <QtCore/QProcess>

#include "engine.h"

class EngineHandler : public QThread {
	Q_OBJECT

public:
	EngineHandler();

protected:
    void run() override;

public:
    ~EngineHandler() override;

private:
	void removeEngine(int id);

	QMap<int, Engine*> *engineList;
	int engineCount = 0;

private slots:
	void handleEngineResult_slot(QJsonObject result);

    void deleteEngineHandler_slot();

    void addNewEngine_slot(const QString &enginePath);

signals:
	//void reportResult_signal(QJsonArray results);

};


