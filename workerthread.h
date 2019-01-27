#pragma once

#include <QtCore/QProcess>
#include <QtCore/QThread>

class WorkerThread : public QThread {
Q_OBJECT
public:
    WorkerThread(int id, const QString &enginePath, const QStringList &paramList);

    ~WorkerThread();

protected:
    void run() override;

private:
    QProcess *process;
    QString enginePath;
    QStringList paramList;
    int id;

public slots:
    void processDone_slot();

    void startWorker_slot();

signals:
    void processDone_signal(int id, QJsonObject resultArray);

    void startWorker_signal();

};


