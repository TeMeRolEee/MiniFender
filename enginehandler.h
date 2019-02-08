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

	bool findExistingEngine(const QString &engineName);

	QMap<int, Engine*> *engineList;
	QMap<QString, int> engineNameList;
	QMap<QString, QJsonArray*> *resultMap;
	QVector<int> *scanIdList;
	int scanId = 0;
	int engineCount = 0;

public slots:
	void handleEngineResult_slot(QJsonObject result);

    void deleteEngineHandler_slot();

    void addNewEngine_slot(const QString &enginePath, const QString &scanParameter, const QString &engineName);

    void handleNewTask_slot(const QString &file);

signals:
    void scanComplete_signal(QJsonObject resultData);

    void newTask_signal(int id, const QString &file);
};


