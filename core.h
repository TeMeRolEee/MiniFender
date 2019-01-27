#pragma once

#include <QtCore/QObject>

#include "enginehandler.h"

class Core : public QThread {
Q_OBJECT
public:
    void init(const QString &settingsFilePath);

	void addNewEngine(const QString &enginePath, const QString &scanParameter);

	void startNewScanTask(QString filePath);

	void listEngineCount();

protected:
	void run() override;

private:
    QString settingsFile;

    void readSettings();

	EngineHandler *engineHandler;

private slots:
    void handleResult_slot(QJsonObject result);

signals:
	void addNewEngine_signal(const QString &engineData, const QString &scanParameter);

    void startNewScanTask_signal(QString task);

    //void reportResult_signal(QJsonArray results);

};

