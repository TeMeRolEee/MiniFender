#include <QtCore/QJsonDocument>
#include <QtCore/QJsonObject>
#include <QUuid>
#include <QDebug>
#include <QtCore/QFile>
#include <QtCore/QFileInfo>
#include <QtCore/QDir>
#include <QtCore/QSysInfo>

#include "workerthread.h"

typedef bool (*InitEngine)();

typedef void (*DeInitEngine)();

typedef bool (*Scan)(const QString &filepath, QJsonObject *result);

InitEngine initEngine;
DeInitEngine deInitEngine;
Scan scan;

WorkerThread::WorkerThread(const QString &enginePath) :
		enginePath(QDir::toNativeSeparators(enginePath)) {
	engine = new QLibrary(this);
	connect(this, &WorkerThread::startWorker_signal, this, &WorkerThread::startWorker_slot, Qt::QueuedConnection);
	connect(this, &WorkerThread::startProcess_signal, this, &WorkerThread::process_slot, Qt::QueuedConnection);
}

WorkerThread::~WorkerThread() {
	engine->unload();
	delete engine;
}

void WorkerThread::run() {
	QThread::run();
}

void WorkerThread::process_slot(QUuid id, const QString &filePath) {
	qInfo() << "[" << __FUNCTION__  << "|" << __FILE__ << "]";

	//QJsonObject *result = new QJsonObject();
	QJsonObject result;

	if (scan(filePath, &result)) {
		qInfo() << "[" << __FUNCTION__  << "|" << __FILE__ << "]" << "SCAN SUCCESSFUL";
		emit processDone_signal(id, result);
	}

	emit processDone_signal(id, QJsonObject());
}

void WorkerThread::startWorker_slot() {
	QFileInfo engineInfo(enginePath);
	qInfo() << "[" << __FUNCTION__  << "|" << __FILE__ << "]" << QLibrary::isLibrary(engineInfo.absoluteFilePath());
	engine->setFileName(engineInfo.absoluteFilePath());

	if (engine->load()) {
		initEngine = (InitEngine) engine->resolve("init");
		deInitEngine = (DeInitEngine) engine->resolve("deInit");
		scan = (Scan) engine->resolve("scanFile");

		qInfo() << "[" << __FUNCTION__  << "|" << __FILE__ << "]" << enginePath << "loaded successfully";

		emit engineInitFinished_signal(initEngine());
	} else {
		qInfo() << "[" << __FUNCTION__  << "|" << __FILE__ << "]" << enginePath << "could not be loaded successfully";

		emit engineInitFinished_signal(false);
	}
}

void WorkerThread::errorHandling(QProcess::ProcessError error) {
	qInfo() << "[" << __FUNCTION__  << "|" << __FILE__ << "]";
	qCritical() << "[" << __FUNCTION__  << "|" << __FILE__ << "]" << error;
}
