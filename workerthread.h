#pragma once

#include <QtCore/QProcess>
#include <QtCore/QThread>

class WorkerThread : public QThread {
public:
    WorkerThread();

    ~WorkerThread();

protected:
    void run() override;

private:
    QProcess *process;
};


