#pragma once

#include <QtCore/QObject>

#include "enginehandler.h"

class Core : public QObject {
Q_OBJECT
public:
    void init();

	bool addNewEngine(const QString &enginePath, const QStringList &parameterList);

private:
	EngineHandler *engineHandler;

private slots:
    void handleEngineHandler_slot(QJsonObject result);

signals:
    //void reportResult_signal(QJsonArray results);

};

