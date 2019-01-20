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
    int workerCount = 0;

    QString enginePath;
    QMap<int, WorkerThread *> *engineProcesses;

public slots:

    void handleProcessDone_slot(QJsonObject resultArray);

    void startEngine_slot(int id);

    void addNewWorker_slot(QStringList &params);

    void deleteEngine_slot();

signals:

    void processDone_signal();

    void startEngine_signal();

    void deleteEngine_signal();
};

