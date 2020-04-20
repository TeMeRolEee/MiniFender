#include <QtCore/QJsonDocument>
#include <QDebug>

#include "engine.h"

Engine::Engine(int id, const QString &enginePath, const QString &engineName)
		: id(id), enginePath(enginePath), engineName(engineName) {
	engine = new WorkerThread(enginePath);
	connect(this, &Engine::deleteEngine_signal, this, &Engine::deleteEngine_slot);
}

Engine::~Engine() {
	delete engine;
}

void Engine::run() {
	QThread::run();
}

void Engine::handleProcessDone_slot(QUuid uniqueId, QJsonObject result) {
	//qInfo() << __FUNCTION__ << __FILE__;
	emit processDone_signal(uniqueId, std::move(result));
}

void Engine::addNewWorker_slot() {
	//qInfo() << "[" << __FUNCTION__  << "|" << __FILE__ << "]";
	engine->start();

	connect(engine, &WorkerThread::processDone_signal, this, &Engine::handleProcessDone_slot,Qt::QueuedConnection);
	connect(engine, &WorkerThread::engineInitFinished_signal, this, &Engine::handleWorkerInit_slot,Qt::QueuedConnection);
	connect(this, &Engine::startEngine_signal, engine, &WorkerThread::startWorker_slot, Qt::QueuedConnection);
	connect(this, &Engine::startScan_signal, engine, &WorkerThread::process_slot, Qt::QueuedConnection);
	connect(engine, &WorkerThread::finished, engine, &WorkerThread::deleteLater);

	emit engine->startWorker_signal();
}

void Engine::deleteEngine_slot() {
	engine->quit();
	engine->wait();
	delete engine;
	emit deletingDone_signal(id);
}

const QString &Engine::getEnginePath() const {
	return enginePath;
}

void Engine::startScan_slot(QUuid uniqueId, const QString &filePath) {
	//qInfo() << "[" << __FUNCTION__  << "|" << __FILE__ << "]";
	emit startScan_signal(uniqueId, filePath);
}

void Engine::handleWorkerInit_slot(bool success) {
	//qInfo() << "[" << __FUNCTION__  << "|" << __FILE__ << "]" << success;
	emit engineInitFinished_signal(success, engineName);
}


