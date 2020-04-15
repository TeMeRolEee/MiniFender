#include <QtCore/QJsonDocument>
#include <QtCore/QJsonObject>
#include <QUuid>
#include <QDebug>
#include <iostream>

#include "workerthread.h"

WorkerThread::WorkerThread(QUuid id, const QString &enginePath, const QStringList &paramList) :
		id(id),
		paramList(paramList),
		enginePath(enginePath) {
	process = new QProcess();
	//connect(process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished), this,&WorkerThread::processDone_slot, Qt::QueuedConnection);
	connect(process, qOverload<int, QProcess::ExitStatus>(&QProcess::finished), this,&WorkerThread::processDone_slot, Qt::QueuedConnection);
	//connect(process, &QProcess::stateChanged, this, &WorkerThread::processDone_slot, Qt::UniqueConnection);
	connect(this, &WorkerThread::startWorker_signal, this, &WorkerThread::startWorker_slot, Qt::QueuedConnection);
}

WorkerThread::~WorkerThread() {
	process->close();
	delete process;
}

void WorkerThread::run() {
	QThread::run();
}

void WorkerThread::processDone_slot() {
	//if (process->exitCode() == 0) {
		QString tempString = process->readAll();
		QJsonObject object = QJsonDocument::fromJson(tempString.toUtf8()).object();
		QJsonDocument qJsonDocument(object);

		qInfo() << tempString;

		emit processDone_signal(id, object);
	//}
}

void WorkerThread::startWorker_slot() {
	std::cout << "SHIIT";
	qInfo() << "SHIT";
	process->start(enginePath, paramList);
}
