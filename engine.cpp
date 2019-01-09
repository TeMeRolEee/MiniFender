#include <QtCore/QJsonDocument>
#include <QDebug>

#include "engine.h"

Engine::Engine(QObject *parent, int id, const QString &enginePath)
        : QThread(parent), id(id), enginePath(enginePath) {
    connect(process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished), this,
            &Engine::handleProcessDone_slot);
}

Engine::~Engine() {
    delete process;
}

void Engine::run() {
    QThread::run();
}

void Engine::setNewConfig(const QStringList &engineParams) {
    this->engineParams = engineParams;
}

void Engine::startEngine_slot() {
    process->start(enginePath, engineParams);
}

void Engine::handleProcessDone_slot() {

}


