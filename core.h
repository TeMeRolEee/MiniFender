#pragma once

#include <QtCore/QObject>

#include "enginehandler.h"

class Core : public QThread {
Q_OBJECT
public:
    void init();

protected:
	void run() override;

private:
	void addNewEngine(const QString &enginePath);

	EngineHandler *engineHandler;

private slots:
    void handleEngineHandler_slot(QJsonObject result);

signals:
	void addNewEngine_signal();

    void startNewTask_signal(QMap<int, QStringList> taskList);

    //void reportResult_signal(QJsonArray results);

};

