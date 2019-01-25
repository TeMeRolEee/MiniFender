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

	bool findExistingEngine(const QString &enginePath);

	QMap<int, Engine*> *engineList;
	QMap<QString, int> enginePathList;
	int engineCount = 0;

	QMap<QString, QJsonArray*> *resultArray;

public slots:
	void handleEngineResult_slot(QJsonObject result);

    void deleteEngineHandler_slot();

    void addNewEngine_slot(const QString &enginePath);

    void handleNewTask_slot(QMap<QString, QStringList> taskList);

signals:
    void scanComplete_signal(QJsonObject resultData);
};


