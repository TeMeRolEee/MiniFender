#include "workerthread.h"

WorkerThread::WorkerThread() {
    connect()
}

WorkerThread::~WorkerThread() {
    delete process;
}

void WorkerThread::run() {
    process = new QProcess();

    QThread::run();
}
