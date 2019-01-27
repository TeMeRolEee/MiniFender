#pragma once

#include <QtCore/QThread>
#include <QtCore/QProcess>
#include <QtCore/QJsonObject>
#include <QtCore/QMap>
#include "workerthread.h"

class Engine : public QThread {
Q_OBJECT
public:
    Engine(int id, const QString &enginePath, const QString &scanParameter);

    ~Engine() override;

    const QString &getEnginePath() const;

protected:
    void run() override;

private:
    int id = 0;
    int workerCount = 0;

    QString enginePath;
    QString scanParameter;

private:
    QMap<int, WorkerThread *> *engineProcesses;

public slots:

    void handleProcessDone_slot(int id, QJsonObject result);

    void addNewWorker_slot(const QString &parameter);

    void deleteEngine_slot();

signals:
    void addNewWorker_signal(const QString &params);

    void processDone_signal(QJsonObject result);

    void startEngine_signal();

    void deleteEngine_signal();
};

