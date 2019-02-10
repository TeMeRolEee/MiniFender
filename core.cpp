#include "core.h"

#include <chrono>
#include <iostream>

#include <QDebug>
#include <QtCore/QJsonDocument>
#include <QtCore/QSettings>
#include <QtCore/QFile>
#include <QtCore/QMap>

Core::~Core() {
    emit removeEngines_signal();

    engineHandler->quit();
    engineHandler->wait();
    cliHandler->quit();
    cliHandler->wait();

    delete cliHandler;
    delete engineHandler;
    delete dbManager;
    delete scanMap;
}

bool Core::init(const QString &settingsFilePath, const QString &dbFilePath) {
    connect(this, &Core::finished, this, &Core::deleteLater);
    qDebug() << "[CORE]\t" << "Starting the core...";
    //this->start(Priority::HighestPriority);
    qDebug() << "[CORE]\t" << "Started the core...\t threadID is:" << QThread::currentThreadId();

    qDebug() << "[CORE]\t" << "Connecting to database";
    dbManager = new DBManager(dbFilePath);
    if (!dbManager->init()) {
        qDebug() << "[CORE]\t" << "Exiting...";
        return false;
    }

    scanMap = new QMap<QUuid, QJsonObject>();

    engineHandler = new EngineHandler();
    connect(this, &Core::addNewEngine_signal, engineHandler, &EngineHandler::addNewEngine_slot);
    connect(this, &Core::startNewScanTask_signal, engineHandler, &EngineHandler::handleNewTask_slot, Qt::QueuedConnection);
    connect(engineHandler, &EngineHandler::scanComplete_signal, this, &Core::handleResult_slot, Qt::QueuedConnection);
    connect(engineHandler, &EngineHandler::finished, engineHandler, &EngineHandler::deleteLater);
    engineHandler->start();

    qDebug() << "[CORE]\t" << "Starting CLI handler";
    cliHandler = new CliHandler();
    connect(cliHandler, &CliHandler::newTask_signal, this, &Core::handleNewTask_slot);
    connect(cliHandler, &CliHandler::finished, cliHandler, &CliHandler::deleteLater);

    qDebug() << "[CORE]\t" << "Loading settings\t" << "File location:\t" << settingsFilePath;
    if (!readSettings(settingsFilePath)) {
         qCritical() << "[CORE]\t" << "Could not load any engine. Shutting down!";
         return false;
     }
    qDebug() << "[CORE]\t" << "Settings loaded";

    listEngineCount();

    return true;
}

void Core::handleResult_slot(QUuid uniqueId, QJsonObject result) {
    for (auto id : scanMap->keys()) {
        qDebug() << "[CORE]\t" << id;
    }
    qDebug() << "[CORE]\t" << uniqueId << QJsonDocument(result).toJson(QJsonDocument::JsonFormat::Compact);
}

void Core::run() {
    QThread::run();
}

bool Core::readSettings(const QString &filePath) {
    QVector<QString> badEngines;
    QSettings settings(filePath, QSettings::IniFormat);
    QStringList keys = settings.childGroups();

    for (const auto &groupName : keys) {
        qDebug() << "[CORE]\t" << groupName;
        settings.beginGroup(groupName);
        QMap<QString, QString> engineData;

        if (settings.childKeys().contains("path") && settings.childKeys().contains("scan_parameter")) {
            for (const auto &key : settings.childKeys()) {
                engineData.insert(key, settings.value(key).toString());
            }
            qInfo() << "[CORE]\t" << "Adding engine:" << groupName;
            emit addNewEngine_signal(engineData.value("path"), engineData.value("scan_parameter"), groupName);
        } else {
            badEngines.append(groupName);
            qWarning() << "[CORE]\t" << "Engine:" << groupName << "cannot be started.\t Skipping";
        }
        settings.endGroup();
    }

    if (badEngines.count() > 0) {
        QString message = "Engines that could not be loaded:";
        for (const auto &engine : badEngines) {
            message.append(" " + engine);
        }
        qWarning() << "[CORE]\t" << message;
    }

    return (badEngines.count() != settings.childGroups().count() && settings.childGroups().count() != 0);
}

void Core::listEngineCount() {
    qDebug() << "[CORE]\t" << "Engine count is:\t" << engineHandler->getEngineCount();
}

void Core::handleNewTask_slot(QString input) {
    if (QFile::exists(input)) {
        QUuid uniqueId = QUuid::createUuid();
        qDebug() << "[CORE]\t" << "unique ID for input:" << input << "is:" << uniqueId;
        QJsonObject scanData;
        scanData.insert("input", input);
        scanMap->insert(uniqueId, scanData);
        emit startNewScanTask_signal(uniqueId, input);
    } else {
        qCritical()  << "[CORE]\t" << "ERROR:\t" << input << "\t not found. Scan cannot be performed.";
    }
}

