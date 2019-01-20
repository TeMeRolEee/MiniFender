#include <QtCore/QJsonDocument>
#include <QDebug>

#include "engine.h"

Engine::Engine(int id, const QString &enginePath)
        : id(id), enginePath(enginePath) {
    qDebug() << "[ENGINE]\t" << enginePath << id;

    engineProcesses = new QMap<int, WorkerThread*>();
    connect(this, &Engine::deleteEngine_signal, this, &Engine::deleteEngine_slot);
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

void Engine::handleProcessDone_slot(QJsonObject resultArray) {
    qDebug() << "[ENGINE]\t" << QJsonDocument(resultArray).toJson(QJsonDocument::JsonFormat::Compact);
}

void Engine::addNewWorker_slot(QStringList &params) {
    auto *workerThread = new WorkerThread(enginePath, params);

    qDebug() << "[ENGINE]\t" << "WorkerThread created.";
    engineProcesses->insert(engineProcesses->count(), workerThread);

    workerThread->start();
    qDebug() << "[ENGINE]\t" << "WorkerThread inserted into map.";

    connect(workerThread, &WorkerThread::processDone_signal, this, &Engine::handleProcessDone_slot);
    connect(this, &Engine::startEngine_signal, workerThread,&WorkerThread::startWorker_slot);
}

void Engine::deleteEngine_slot() {
    for (auto worker : engineProcesses->keys()) {
        engineProcesses->value(worker)->quit();
        engineProcesses->value(worker)->wait();
        delete engineProcesses->take(worker);
    }
}


