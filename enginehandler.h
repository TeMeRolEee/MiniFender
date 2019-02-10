#pragma once

#include <QtCore/QObject>
#include <QtCore/QProcess>
#include <QUuid>

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

	bool findExistingEngine(const QString &engineName);

	QMap<int, Engine*> *engineList;
	QMap<QString, int> engineNameList;
	QMap<QString, QJsonArray*> *resultMap;
	QVector<int> *scanIdList;
	int engineCount = 0;

public slots:
	void handleEngineResult_slot(QUuid uniqueId, QJsonObject result);

    void deleteEngineHandler_slot();

    void addNewEngine_slot(const QString &enginePath, const QString &scanParameter, const QString &engineName);

    void handleNewTask_slot(QUuid uniqueId, const QString &file);

signals:
    void scanComplete_signal(QUuid id, QJsonObject resultData);

    void newTask_signal(QUuid uniqueId, const QString &file);

};


