#include <QtCore/QJsonDocument>
#include <QDebug>

#include "engine.h"

Engine::Engine(int id, const QString &enginePath)
        : id(id), enginePath(enginePath) {
    qDebug() << enginePath << id;

    engineProcesses = new QMap<int, WorkerThread*>();
    //connect(process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished), this,
            //&Engine::handleProcessDone_slot);
}

Engine::~Engine() {
    delete engineProcesses;
}

void Engine::run() {
    QThread::run();
}

void Engine::startEngine_slot(int id) {
    for (auto key : engineProcesses->keys()) {
        qDebug() << key;
    }

    engineProcesses->value(id)->start();
    //process->start(enginePath, engineParams);
}

void Engine::handleProcessDone_slot(QJsonObject resultArray) {
    qDebug() << QJsonDocument(resultArray).toJson(QJsonDocument::JsonFormat::Indented);
}

void Engine::addNewEngine_slot(QStringList &params) {
    auto *workerThread = new WorkerThread(enginePath, params);

    qDebug() << "WorkerThread created.";
    engineProcesses->insert(engineProcesses->count(), workerThread);

    workerThread->start();
    qDebug() << "WorkerThread inserted into map, hopefully.";

    connect(workerThread, &WorkerThread::processDone_signal, this, &Engine::handleProcessDone_slot);
    connect(this, &Engine::startEngine_signal, workerThread,&WorkerThread::startWorker_slot);
}


