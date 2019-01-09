#pragma once

#include <QtCore/QThread>
#include <QtCore/QProcess>
#include <QtCore/QJsonObject>

class Engine : public QThread {
Q_OBJECT
public:
    Engine(QObject *parent, int id, const QString &enginePath);

    ~Engine() override;

    void setNewConfig(const QStringList &engineParams);

protected:
    void run() override;

private:
    QProcess *process = nullptr;
    int id;
    const QString enginePath;
    QStringList engineParams;
    QJsonObject tempResult;

    QMap<int, QProcess*> *engineProcesses;

public slots:
    void handleProcessDone_slot();

    void startEngine_slot();

signals:
    void processDone_signal();

};

