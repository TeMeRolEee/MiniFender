#pragma once

#include <QtCore/QObject>

#include "enginehandler.h"
#include "dbmanager.h"
#include "clihandler.h"

class Core : public QThread {
Q_OBJECT
public:
	~Core();

    bool init(const QString &settingsFilePath, const QString &dbFilePath);

    void listEngineCount();

protected:
	void run() override;

private:
    void readSettings(const QString &filePath);

	EngineHandler *engineHandler;

	DBManager *dbManager;

	CliHandler *cliHandler;

private slots:
    void handleResult_slot(QJsonObject result);

    void handleNewTask_slot(QString input);

signals:
	void addNewEngine_signal(const QString &enginePath, const QString &scanParameter, const QString &engineName);

    void startNewScanTask_signal(QString file);

    void removeEngines_signal();


};

