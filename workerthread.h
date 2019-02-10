#pragma once

#include <QtCore/QProcess>
#include <QtCore/QThread>
#include <QtCore/QUuid>

class WorkerThread : public QThread {
Q_OBJECT
public:
    WorkerThread(QUuid id, const QString &enginePath, const QStringList &paramList);

    ~WorkerThread();

protected:
    void run() override;

private:
    QProcess *process;
    QString enginePath;
    QStringList paramList;
    QUuid id;

public slots:
    void processDone_slot();

    void startWorker_slot();

signals:
    void processDone_signal(QUuid id, QJsonObject resultArray);

    void startWorker_signal();

};


