#include <QtCore/QJsonDocument>
#include <QtCore/QJsonObject>
#include <QDebug>

#include "workerthread.h"

WorkerThread::WorkerThread(const QString &enginePath, const QStringList &paramList) :
        paramList(paramList),
        enginePath(enginePath) {
    moveToThread(this);
    process = new QProcess();
    connect(process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished), this,
            &WorkerThread::processDone_slot);
}

WorkerThread::~WorkerThread() {
    process->close();
    delete process;
}

void WorkerThread::run() {

    qDebug() << "Starting the engine";
    process->start(enginePath, paramList);

    //QThread::run();
}

void WorkerThread::processDone_slot() {
    QString tempString = process->readAllStandardOutput();

    QJsonObject object = QJsonDocument::fromJson(tempString.toUtf8()).object();

    QJsonDocument qJsonDocument(object);

    qDebug() << qJsonDocument.toJson(QJsonDocument::JsonFormat::Indented);

    //emit processDone_signal(object);
}
