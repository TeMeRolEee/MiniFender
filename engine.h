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

    const QString &getEnginePath() const;

protected:
    void run() override;

private:
    int id = 0;
    int workerCount = 0;

    QString enginePath;

private:
    QMap<int, WorkerThread *> *engineProcesses;

public slots:

    void handleProcessDone_slot(QJsonObject result);

    void startEngine_slot(int id);

    void addNewWorker_slot(QStringList &params);

    void deleteEngine_slot();

signals:
    void addNewWorker_signal(QStringList &params);

    void processDone_signal(QJsonObject result);

    void startEngine_signal();

    void deleteEngine_signal();
};

