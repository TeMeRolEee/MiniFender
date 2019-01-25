#include "core.h"
#include <chrono>
#include <iostream>

#include <QDebug>
#include <QtCore/QJsonDocument>

void Core::addNewEngine(const QString &enginePath) {
    emit addNewEngine_signal(enginePath);
}

void Core::init(const QString &settingsFilePath) {
    qDebug() << "[CORE]\t" << "Starting the core...";
    this->start(Priority::HighestPriority);
    qDebug() << "[CORE]\t" << "Started the core...\t threadID is:" << QThread::currentThreadId();

    qDebug() << "[CORE]\t" << "Loading settings\t" << "File location:\t" << settingsFilePath;
    settingsFile = settingsFilePath;
    qDebug() << "[CORE]\t" << "Settings loaded";

    engineHandler = new EngineHandler();
    connect(this, &Core::addNewEngine_signal, engineHandler, &EngineHandler::addNewEngine_slot);
    connect(this, &Core::startNewScanTask_signal, engineHandler, &EngineHandler::handleNewTask_slot);
    connect(engineHandler, &EngineHandler::scanComplete_signal, this, &Core::handleResult_slot);
    engineHandler->start();
}

void Core::handleResult_slot(QJsonObject result) {
    qDebug() << "[CORE]\t" << QJsonDocument(result).toJson(QJsonDocument::JsonFormat::Indented);
}

void Core::run() {
    QThread::run();
}

void Core::startNewScanTask(const QString filePath) {
    QStringList params;
    params << "-s" << "/home/temerole/Academy/TestEngines/build/TestEngines";
    QString enginePath("/home/temerole/Academy/TestEngines/build/TestEngines");

    QMap<QString, QStringList> list;
    list.insert(enginePath, params);

    emit startNewScanTask_signal(list);
}

void Core::readSettings() {

}
