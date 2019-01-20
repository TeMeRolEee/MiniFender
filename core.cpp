#include "core.h"
#include <chrono>
#include <iostream>

#include <QDebug>

void Core::addNewEngine(const QString &enginePath) {

}

void Core::init() {
    qDebug() << "[CORE]\t" << "Starting the core...";
    this->start(Priority::HighestPriority);
    qDebug() << "[CORE]\t" << "Started the core...\t threadID is:" << QThread::currentThreadId();
    engineHandler = new EngineHandler();
    engineHandler->start();
}

void Core::handleEngineHandler_slot(QJsonObject result) {

}

void Core::run() {
    QThread::run();
}
