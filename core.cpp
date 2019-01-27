#include "core.h"
#include <chrono>
#include <iostream>

#include <QDebug>
#include <QtCore/QJsonDocument>
#include <QtCore/QSettings>

Core::~Core() {
    emit removeEngines_signal();
    engineHandler->quit();
    engineHandler->wait();

    delete engineHandler;
}

void Core::addNewEngine(const QString &enginePath, const QString &scanParameter) {
    emit addNewEngine_signal(enginePath, scanParameter);
}

void Core::init(const QString &settingsFilePath) {
    qDebug() << "[CORE]\t" << "Starting the core...";
    this->start(Priority::HighestPriority);
    qDebug() << "[CORE]\t" << "Started the core...\t threadID is:" << QThread::currentThreadId();

    engineHandler = new EngineHandler();
    connect(this, &Core::addNewEngine_signal, engineHandler, &EngineHandler::addNewEngine_slot);
    connect(this, &Core::startNewScanTask_signal, engineHandler, &EngineHandler::handleNewTask_slot);
    connect(engineHandler, &EngineHandler::scanComplete_signal, this, &Core::handleResult_slot);
    connect(this, &Core::removeEngines_signal, engineHandler, &EngineHandler::deleteEngineHandler_slot);
    engineHandler->start();

    qDebug() << "[CORE]\t" << "Loading settings\t" << "File location:\t" << settingsFilePath;
    settingsFile = settingsFilePath;
    readSettings();
    qDebug() << "[CORE]\t" << "Settings loaded";
}

void Core::handleResult_slot(QJsonObject result) {
    qDebug() << "[CORE]\t" << QJsonDocument(result).toJson(QJsonDocument::JsonFormat::Compact);
}

void Core::run() {
    QThread::run();
}

void Core::startNewScanTask(const QString filePath) {
    emit startNewScanTask_signal(filePath);
}

void Core::readSettings() {
    QSettings settings(settingsFile, QSettings::IniFormat);
    QStringList keys = settings.childGroups();
    for (const auto &groupName : keys) {
        qDebug() << "[CORE]\t" << groupName;
        settings.beginGroup(groupName);
        QStringList engineData;
        for (const auto &key : settings.childKeys()) {
            qDebug() << "[CORE]\t" << "KEY:\t" << key << "SETTING IS:\t" << settings.value(key).toString();
            engineData.append(settings.value(key).toString());
        }
        settings.endGroup();
        addNewEngine(engineData[0], engineData[1]);
    }
}

void Core::listEngineCount() {
    qDebug() << "[CORE]\t" << "Engine count is:\t" << engineHandler->getEngineCount();
}
