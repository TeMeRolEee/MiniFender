#pragma once

#include <QtCore/QThread>
#include <QtCore/QProcess>
#include <QtCore/QJsonObject>
#include "workerthread.h"

class Engine : public QThread {
Q_OBJECT
public:
    Engine(int id, const QString &enginePath);

    ~Engine() override;

    void setNewConfig(const QStringList &engineParams);

protected:
    void run() override;

private:
    int id = 0;
    const QString enginePath;

    QMap<int, WorkerThread*> *engineProcesses;

public slots:
    void handleProcessDone_slot(QJsonObject resultArray);

    void startEngine_slot(int id);

    void addNewEngine_slot(QStringList &params);


signals:
    void processDone_signal();

};

