#pragma once

#include <QtCore/QObject>
#include <QtCore/QProcess>

#include "engine.h"

class EngineHandler : public QThread {
	Q_OBJECT

public:
	EngineHandler();
	~EngineHandler() override;

	int getEngineCount();

protected:
    void run() override;

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

    void addNewEngine_slot(const QString &enginePath, const QString &scanParameter);

    void handleNewTask_slot(const QString &task);

signals:
    void scanComplete_signal(QJsonObject resultData);
};


