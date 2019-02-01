#pragma once

#include <QtCore/QObject>

#include "enginehandler.h"
#include "dbmanager.h"

class Core : public QThread {
Q_OBJECT
public:
	~Core();

    void init(const QString &settingsFilePath, const QString &dbFilePath);

	void addNewEngine(const QString &enginePath, const QString &scanParameter);

	void startNewScanTask(QString filePath);

	void listEngineCount();

protected:
	void run() override;

private:
    QString settingsFile;

    void readSettings();

	EngineHandler *engineHandler;

	DBManager *dbManager;

private slots:
    void handleResult_slot(QJsonObject result);

signals:
	void addNewEngine_signal(const QString &engineData, const QString &scanParameter);

    void startNewScanTask_signal(QString task);

    void removeEngines_signal();


};

