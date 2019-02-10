#pragma once

#include <QtCore/QObject>
#include <QUuid>

#include "enginehandler.h"
#include "dbmanager.h"
#include "clihandler.h"
#include <httplib.h>

class Core : public QThread {
Q_OBJECT
public:
	~Core();

    bool init(const QString &settingsFilePath, const QString &dbFilePath);

    void listEngineCount();

protected:
	void run() override;

private:
    bool readSettings(const QString &filePath);

	EngineHandler *engineHandler;

	DBManager *dbManager;

	CliHandler *cliHandler;

	QMap<QUuid, QJsonArray*> *scanMap;

	httplib::Server server;

	QJsonObject calculateResult(QUuid id);

private slots:
    void handleEngineResults_slot(QUuid uniqueId, QJsonObject result);

    void handleNewTask_slot(QString input);

    void result_slot(QUuid id);

signals:
	void addNewEngine_signal(const QString &enginePath, const QString &scanParameter, const QString &engineName);

    void startNewScanTask_signal(QUuid uniqueId, QString file);

    void removeEngines_signal();

    void startCalculateResult_signal(QUuid id);

    void startWebServer_signal();
};

