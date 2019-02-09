#include <QtCore/QJsonDocument>
#include <QDebug>

#include "engine.h"

Engine::Engine(int id, const QString &enginePath, const QString &scanParameter)
        : id(id), enginePath(enginePath), scanParameter(scanParameter) {
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

void Engine::handleProcessDone_slot(int id, QJsonObject result) {
    qDebug() << "[ENGINE]\t" << QJsonDocument(result).toJson(QJsonDocument::JsonFormat::Compact);

    engineProcesses->value(id)->quit();
    engineProcesses->value(id)->wait();
    delete engineProcesses->take(id);

    qDebug() << "[ENGINE]\t" << "Worker ID: [" << id << "] deleted from: [" << this->id << "] ID engine";

    emit processDone_signal(result);
}

void Engine::addNewWorker_slot(int scanId, const QString &parameter) {
    if (!parameter.isEmpty()) {
        auto *workerThread = new WorkerThread(workerCount, enginePath, QStringList() << scanParameter << parameter);

        qDebug() << "[ENGINE]\t" << "WorkerThread created.";
        engineProcesses->insert(workerCount++, workerThread);

        workerThread->start();
        qDebug() << "[ENGINE]\t" << "WorkerThread inserted into map.";

        connect(workerThread, &WorkerThread::processDone_signal, this, &Engine::handleProcessDone_slot);
        connect(this, &Engine::startEngine_signal, workerThread, &WorkerThread::startWorker_slot);
        connect(workerThread, &WorkerThread::finished, workerThread, &WorkerThread::deleteLater);
        emit workerThread->startWorker_signal();
    }
}

void Engine::deleteEngine_slot() {
    for (auto worker : engineProcesses->keys()) {
        engineProcesses->value(worker)->quit();
        engineProcesses->value(worker)->wait();
        delete engineProcesses->take(worker);
    }
    emit deletingDone_signal(id);
}

const QString &Engine::getEnginePath() const {
    return enginePath;
}


