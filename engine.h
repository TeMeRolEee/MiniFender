#pragma once

#include <QtCore/QThread>
#include <QtCore/QProcess>
#include <QtCore/QJsonObject>
#include <QtCore/QMap>
#include "workerthread.h"

class Engine : public QThread {
Q_OBJECT
public:
    Engine(int id, const QString &enginePath);

    ~Engine() override;

protected:
    void run() override;

private:
    int id = 0;
    QString enginePath;

    QMap<int, WorkerThread*> *engineProcesses;

public slots:
    void handleProcessDone_slot(QJsonObject resultArray);

    void startEngine_slot(int id);

    void addNewEngine_slot(QStringList &params);


signals:
    void processDone_signal();

    void startEngine_signal();
};

