#pragma once

#include <QtCore/QObject>
#include <QtCore/QCoreApplication>
#include <QUuid>

#include "enginehandler.h"
#include "dbmanager.h"
#include "clihandler.h"
#include "authclient.h"

class Core : public QThread {
Q_OBJECT
public:
    Core(const QString &rootDirectory);

    ~Core();

    bool init(const QString &settingsFilePath, const QString &dbFilePath);

    void listEngineCount();

protected:
    void run() override;

private:
    bool readSettings(const QString &filePath);

    bool isRegistered = false;

    EngineHandler *engineHandler = nullptr;

    DBManager *dbManager = nullptr;

    CliHandler *cliHandler = nullptr;

    QMap<QUuid, QJsonObject> *scanMap = nullptr;

    QJsonObject calculateResult(QUuid id);

    AuthClient *authClient = nullptr;

private slots:

    void handleEngineResults_slot(QUuid uniqueId, const QJsonObject& result);

    void handleNewTask_slot(const QString& input);

	void handleAuthenticationResponse_slot(bool isGood);

    void result_slot(QUuid id);

signals:

    void addNewEngine_signal(const QString &enginePath, const QString &scanParameter, const QString &engineName);

    void startNewScanTask_signal(QUuid uniqueId, QString file);

    void removeEngines_signal();

    void startCalculateResult_signal(QUuid id);

    void checkedRegistration_signal();

	void sendSerialKey_signal(const QString &serial);
};

