#pragma once

#include <QtCore/QObject>

#include "enginehandler.h"
#include "dbmanager.h"

class Core : public QThread {
Q_OBJECT
public:
	~Core();

    bool init(const QString &settingsFilePath, const QString &dbFilePath);

	void addNewEngine(const QString &enginePath, const QString &scanParameter, const QString &engineName);

	void startNewScanTask(QString filePath);

	void listEngineCount();

protected:
	void run() override;

private:
    void readSettings(const QString &filePath);

	EngineHandler *engineHandler;

	DBManager *dbManager;

private slots:
    void handleResult_slot(QJsonObject result);

signals:
	void addNewEngine_signal(const QString &enginePath, const QString &scanParameter, const QString &engineName);

    void startNewScanTask_signal(QString task);

    void removeEngines_signal();


};

