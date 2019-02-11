#include <QtCore/QJsonDocument>
#include <QtCore/QJsonObject>
#include <QUuid>
#include <QDebug>

#include "workerthread.h"

WorkerThread::WorkerThread(QUuid id, const QString &enginePath, const QStringList &paramList) :
        id(id),
        paramList(paramList),
        enginePath(enginePath) {
    process = new QProcess();
    connect(process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished), this,
            &WorkerThread::processDone_slot);
    connect(this, &WorkerThread::startWorker_signal, this, &WorkerThread::startWorker_slot, Qt::QueuedConnection);
}

WorkerThread::~WorkerThread() {
    process->close();
    delete process;
}

void WorkerThread::run() {
    QThread::run();
}

void WorkerThread::processDone_slot() {
    QString tempString = process->readAllStandardOutput();
    QJsonObject object = QJsonDocument::fromJson(tempString.toUtf8()).object();
    QJsonDocument qJsonDocument(object);

    emit processDone_signal(id, object);
}

void WorkerThread::startWorker_slot() {
    process->start(enginePath, paramList);
}
