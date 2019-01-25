#pragma once

#include <QtCore/QObject>

#include "enginehandler.h"

class Core : public QThread {
Q_OBJECT
public:
    void init(const QString &settingsFilePath);

	void addNewEngine(const QString &enginePath);

	void startNewScanTask(QString filePath);

protected:
	void run() override;

private:
    QString settingsFile;

    void readSettings();

	EngineHandler *engineHandler;

private slots:
    void handleResult_slot(QJsonObject result);

signals:
	void addNewEngine_signal(const QString &enginePath);

    void startNewScanTask_signal(QMap<QString, QStringList> taskList);

    //void reportResult_signal(QJsonArray results);

};

