#include <QtCore/QJsonDocument>
#include <QtCore/QJsonObject>
#include <QDebug>

#include "workerthread.h"

WorkerThread::WorkerThread(int id, const QString &enginePath, const QStringList &paramList) :
        id(id),
        paramList(paramList),
        enginePath(enginePath) {
    qDebug() << "[WORKER]\t" << "WorkerThread constructor";

    process = new QProcess();
    connect(process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished), this,
            &WorkerThread::processDone_slot);
    connect(this, &WorkerThread::startWorker_signal, this, &WorkerThread::startWorker_slot, Qt::QueuedConnection);
}

WorkerThread::~WorkerThread() {
    qDebug() << "[WORKER]\t" << "Deleting worker thread";

    process->close();
    delete process;
}

void WorkerThread::run() {
    qDebug() << "[WORKER]\t" << QThread::currentThreadId();

    QThread::run();
}

void WorkerThread::processDone_slot() {
    qDebug() << "[WORKER]\t" << "Starting the engine";

    QString tempString = process->readAllStandardOutput();

    QJsonObject object = QJsonDocument::fromJson(tempString.toUtf8()).object();

    QJsonDocument qJsonDocument(object);

    qDebug() << "[WORKER]\t" << qJsonDocument.toJson(QJsonDocument::JsonFormat::Compact);

    emit processDone_signal(id, object);
}

void WorkerThread::startWorker_slot() {
    qDebug() << "[WORKER]\t" << "Starting the engine";
    process->start(enginePath, paramList);
}
