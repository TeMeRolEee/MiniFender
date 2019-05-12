

#include <QtCore/QJsonDocument>

#include "engine.h"

Engine::Engine(int id, const QString &enginePath, const QString &scanParameter)
        : id(id), enginePath(enginePath), scanParameter(scanParameter) {
    engineProcesses = new QMap<QUuid, WorkerThread *>();
    connect(this, &Engine::deleteEngine_signal, this, &Engine::deleteEngine_slot);
}

Engine::~Engine() {
    delete engineProcesses;
}

void Engine::run() {
    QThread::run();
}

void Engine::handleProcessDone_slot(QUuid uniqueId, QJsonObject result) {
    engineProcesses->value(uniqueId)->quit();
    engineProcesses->value(uniqueId)->wait();
    delete engineProcesses->take(uniqueId);

    emit processDone_signal(uniqueId, std::move(result));
}

void Engine::addNewWorker_slot(QUuid uniqueId, const QString &parameter) {
    if (!parameter.isEmpty()) {
        auto *workerThread = new WorkerThread(uniqueId, enginePath, QStringList() << scanParameter << parameter);
        engineProcesses->insert(uniqueId, workerThread);
        workerThread->start();

        connect(workerThread, &WorkerThread::processDone_signal, this, &Engine::handleProcessDone_slot, Qt::QueuedConnection);
        connect(this, &Engine::startEngine_signal, workerThread, &WorkerThread::startWorker_slot, Qt::QueuedConnection);
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


