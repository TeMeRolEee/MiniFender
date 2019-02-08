#include "core.h"
#include <chrono>
#include <iostream>

#include <QDebug>
#include <QtCore/QJsonDocument>
#include <QtCore/QSettings>
#include <QtCore/QFile>

Core::~Core() {
    emit removeEngines_signal();

    engineHandler->quit();
    engineHandler->wait();
    cliHandler->quit();
    cliHandler->wait();

    delete cliHandler;
    delete engineHandler;
    delete dbManager;
}

bool Core::init(const QString &settingsFilePath, const QString &dbFilePath) {
    qDebug() << "[CORE]\t" << "Starting the core...";
    this->start(Priority::HighestPriority);
    qDebug() << "[CORE]\t" << "Started the core...\t threadID is:" << QThread::currentThreadId();

    qDebug() << "[CORE]\t" << "Connecting to database";
    dbManager = new DBManager(dbFilePath);
    if (!dbManager->init()) {
        qDebug() << "[CORE]\t" << "Exiting...";
        return false;
    }

    engineHandler = new EngineHandler();
    connect(this, &Core::addNewEngine_signal, engineHandler, &EngineHandler::addNewEngine_slot);
    connect(this, &Core::startNewScanTask_signal, engineHandler, &EngineHandler::handleNewTask_slot, Qt::QueuedConnection);
    connect(engineHandler, &EngineHandler::scanComplete_signal, this, &Core::handleResult_slot);
    connect(this, &Core::removeEngines_signal, engineHandler, &EngineHandler::deleteEngineHandler_slot);
    engineHandler->start();

    qDebug() << "[CORE]\t" << "Loading settings\t" << "File location:\t" << settingsFilePath;
    readSettings(settingsFilePath);
    qDebug() << "[CORE]\t" << "Settings loaded";

    qDebug() << "[CORE]\t" << "Starting CLI handler";
    cliHandler = new CliHandler();
    connect(cliHandler, &CliHandler::newTask_signal, this, &Core::handleNewTask_slot);
    cliHandler->start();

    emit cliHandler->startListening_signal();

    listEngineCount();

    return true;
}

void Core::handleResult_slot(QJsonObject result) {
    qDebug() << "[CORE]\t" << QJsonDocument(result).toJson(QJsonDocument::JsonFormat::Compact);
}

void Core::run() {
    QThread::run();
}

void Core::readSettings(const QString &filePath) {
    QSettings settings(filePath, QSettings::IniFormat);
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
        emit addNewEngine_signal(engineData[0], engineData[1], groupName);
    }
}

void Core::listEngineCount() {
    qDebug() << "[CORE]\t" << "Engine count is:\t" << engineHandler->getEngineCount();
}

void Core::handleNewTask_slot(QString input) {
    if (QFile::exists(input)) {
        qDebug() << "[CORE]\t" << "file is:\t" << input;
        emit startNewScanTask_signal(input);
        qDebug() << "[CORE]\t" << "Valid file received, passing to the engines";
    } else {
        qCritical()  << "[CORE]\t" << "ERROR:\t" << input << "\t not found. Scan cannot be performed.";
    }
}
