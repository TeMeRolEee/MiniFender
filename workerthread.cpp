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

WorkerThread::WorkerThread(QUuid id, const QString &enginePath, const QStringList &paramList) :
		id(id),
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
	auto scan = (Scan) engine->resolve("scanFile");
	auto *result = new QJsonObject();

	if (scan(filePath, result)) {

	}

	emit processDone_signal(id, QJsonObject());
}

void WorkerThread::startWorker_slot(QUuid id, const QString &filePath) {
	QFileInfo engineInfo(enginePath);
	qInfo() << QLibrary::isLibrary(engineInfo.absoluteFilePath());
	engine->setFileName(engineInfo.absoluteFilePath());
	engine->load();
	auto initEngine = (InitEngine) engine->resolve("init");

	if (!initEngine) {
		emit processDone_signal(id, QJsonObject());
	}

	emit startProcess_signal(id, filePath);
}

void WorkerThread::errorHandling(QProcess::ProcessError error) {
	qCritical() << __FUNCTION__ << error;
}
