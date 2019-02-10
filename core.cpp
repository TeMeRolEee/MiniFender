#include "core.h"

#include <chrono>
#include <iostream>

#include <QDebug>
#include <QtCore/QJsonDocument>
#include <QtCore/QSettings>
#include <QtCore/QFile>
#include <QtCore/QMap>
#include <QtCore/QDateTime>

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
    connect(this, &Core::startCalculateResult_signal, this, &Core::calculateResult_slot, Qt::QueuedConnection);
    connect(this, &Core::finished, this, &Core::deleteLater);

    //qDebug() << "[CORE]\t" << "Connecting to database";
    dbManager = new DBManager(dbFilePath);
    if (!dbManager->init()) {
        //qDebug() << "[CORE]\t" << "Exiting...";
        return false;
    }

    scanMap = new QMap<QUuid, QJsonArray*>();

    engineHandler = new EngineHandler();
    connect(this, &Core::addNewEngine_signal, engineHandler, &EngineHandler::addNewEngine_slot);
    connect(this, &Core::startNewScanTask_signal, engineHandler, &EngineHandler::handleNewTask_slot, Qt::QueuedConnection);
    connect(engineHandler, &EngineHandler::scanComplete_signal, this, &Core::handleEngineResults_slot, Qt::QueuedConnection);
    connect(engineHandler, &EngineHandler::finished, engineHandler, &EngineHandler::deleteLater);
    engineHandler->start();

    //qDebug() << "[CORE]\t" << "Starting CLI handler";
    cliHandler = new CliHandler();
    connect(cliHandler, &CliHandler::newTask_signal, this, &Core::handleNewTask_slot);
    connect(cliHandler, &CliHandler::finished, cliHandler, &CliHandler::deleteLater);

    //qDebug() << "[CORE]\t" << "Loading settings\t" << "File location:\t" << settingsFilePath;
    if (!readSettings(settingsFilePath)) {
         qCritical() << "[CORE]\t" << "Could not load any engine. Shutting down!";
         return false;
     }
    //qDebug() << "[CORE]\t" << "Settings loaded";

    listEngineCount();

    return true;
}

void Core::handleEngineResults_slot(QUuid uniqueId, QJsonObject result) {
    for (auto id : scanMap->keys()) {
        //qDebug() << "[CORE]\t" << id;
    }
    //qDebug() << "[CORE]\t" << uniqueId << QJsonDocument(result).toJson(QJsonDocument::JsonFormat::Compact);
    if (scanMap->find(uniqueId) != scanMap->end()) {
        scanMap->value(uniqueId)->push_back(result);
    }
    //qDebug() << "[CORE]\t" << scanMap->value(uniqueId)->count() - 1;
    if (scanMap->value(uniqueId)->count() - 1 == engineHandler->getEngineCount()) {
        //qDebug() << "[CORE]\t" << "Started calculating result";
        emit startCalculateResult_signal(uniqueId);
    }
}

void Core::run() {
    QThread::run();
}

bool Core::readSettings(const QString &filePath) {
    QVector<QString> badEngines;
    QSettings settings(filePath, QSettings::IniFormat);
    QStringList keys = settings.childGroups();

    for (const auto &groupName : keys) {
        //qDebug() << "[CORE]\t" << groupName;
        settings.beginGroup(groupName);
        QMap<QString, QString> engineData;

        if (settings.childKeys().contains("path") && settings.childKeys().contains("scan_parameter")) {
            for (const auto &key : settings.childKeys()) {
                engineData.insert(key, settings.value(key).toString());
            }
            //qInfo() << "[CORE]\t" << "Adding engine:" << groupName;
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
    //qDebug() << "[CORE]\t" << "Engine count is:\t" << engineHandler->getEngineCount();
}

void Core::handleNewTask_slot(QString input) {
    if (QFile::exists(input)) {
        QUuid uniqueId = QUuid::createUuid();
        //qDebug() << "[CORE]\t" << "unique ID for input:" << input << "is:" << uniqueId;
        auto *array = new QJsonArray();
        QJsonObject initialData;
        initialData.insert("scanDate", QDateTime::currentSecsSinceEpoch());
        array->push_back(initialData);
        scanMap->insert(uniqueId, array);
        emit startNewScanTask_signal(uniqueId, input);
    } else {
        qCritical()  << "[CORE]\t" << "ERROR:\t" << input << "\t not found. Scan cannot be performed.";
    }
}

void Core::calculateResult_slot(QUuid id) {
    //qDebug() << "[CORE]\t" << "FUCKIN' PIECE OF SHIT!";
    int infectedCount = 0;
    QJsonArray *resultArray = scanMap->value(id);
    QJsonObject finalResult;

    for (int i = 1; i < resultArray->count(); i++) {
        QJsonObject temp = resultArray->at(i).toObject();
        QJsonArray scan_result = temp.value("scan_result").toArray();
        if (scan_result.at(0).toObject().value("verdict").toInt() == 1) {
            infectedCount++;
        }
    }

    if (infectedCount > 0) {
        finalResult.insert("scanResult", 1);
    } else {
        finalResult.insert("scanResult", 0);
    }

    QJsonArray result;

    for (int i = 1; i < resultArray->count() - 1; i++) {
        result.push_back(resultArray->at(i).toObject());
    }

    finalResult.insert("scanDate", resultArray->at(0).toObject().value("scanDate"));
    finalResult.insert("engineResults", result);

    //qDebug() << QJsonDocument(finalResult).toJson(QJsonDocument::JsonFormat::Compact);

    std::cout << QJsonDocument(finalResult).toJson(QJsonDocument::JsonFormat::Compact).toStdString() << std::endl;
    std::flush(std::cout);

    dbManager->addScanData(finalResult);
}

