#include <QtCore/QJsonDocument>
#include <QDebug>

#include "engine.h"

Engine::Engine(int id, const QString &enginePath)
        : id(id), enginePath(enginePath) {
    qDebug() << "[ENGINE]\t" << enginePath << id;

    engineProcesses = new QMap<int, WorkerThread*>();
    connect(this, &Engine::deleteEngine_signal, this, &Engine::deleteEngine_slot);
    connect(this, &Engine::addNewWorker_signal, this, &Engine::addNewWorker_slot);
}

Engine::~Engine() {
    delete engineProcesses;
}

void Engine::run() {
    QThread::run();
}

void Engine::startEngine_slot(int id) {
    qDebug() << "[ENGINE]\t" << "Engine id:\t" << id << "\tstarting...";
     engineProcesses->value(id)->start();
}

void Engine::handleProcessDone_slot(QJsonObject result) {
    qDebug() << "[ENGINE]\t" << QJsonDocument(result).toJson(QJsonDocument::JsonFormat::Compact);
    emit processDone_signal(result);
}

void Engine::addNewWorker_slot(QStringList &params) {
    if (!params.isEmpty()) {
        auto *workerThread = new WorkerThread(enginePath, params);

        qDebug() << "[ENGINE]\t" << "WorkerThread created.";
        engineProcesses->insert(engineProcesses->count(), workerThread);

        workerThread->start();
        qDebug() << "[ENGINE]\t" << "WorkerThread inserted into map.";

        connect(workerThread, &WorkerThread::processDone_signal, this, &Engine::handleProcessDone_slot);
        connect(this, &Engine::startEngine_signal, workerThread,&WorkerThread::startWorker_slot);
    }
}

void Engine::deleteEngine_slot() {
    for (auto worker : engineProcesses->keys()) {
        engineProcesses->value(worker)->quit();
        engineProcesses->value(worker)->wait();
        delete engineProcesses->take(worker);
    }
}

const QString &Engine::getEnginePath() const {
    return enginePath;
}


