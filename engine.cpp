#include <QtCore/QJsonDocument>
#include <QDebug>

#include "engine.h"

Engine::Engine(int id, const QString &enginePath)
        : id(id), enginePath(enginePath) {
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

void Engine::setNewConfig(const QStringList &engineParams) {
    //this->engineParams = engineParams;
}

void Engine::startEngine_slot(int id) {
    engineProcesses->value(id)->start();
    //process->start(enginePath, engineParams);
}

void Engine::handleProcessDone_slot(QJsonObject resultArray) {
    qDebug() << QJsonDocument(resultArray).toJson(QJsonDocument::JsonFormat::Indented);
}

void Engine::addNewEngine_slot(QStringList &params) {
    auto workerThread = new WorkerThread(enginePath, params);

    engineProcesses->insert(id++, workerThread);

    connect(workerThread, &WorkerThread::processDone_signal, this, &Engine::handleProcessDone_slot);
}


