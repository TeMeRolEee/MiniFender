#pragma once

#include <QtCore/QThread>
#include <QtCore/QProcess>
#include <QtCore/QJsonObject>
#include <QtCore/QMap>
#include <QUuid>

#include "workerthread.h"

class Engine : public QThread {
Q_OBJECT

public:
	Engine(int id, const QString &enginePath, const QString &engineName);

	~Engine() override;

	const QString &getEnginePath() const;

protected:
	void run() override;

private:
	int id = 0;

	QString enginePath;
	QString engineName;

private:
	WorkerThread *engine = nullptr;

public slots:

	void handleProcessDone_slot(QUuid uniqueId, QJsonObject result);

	void addNewWorker_slot();

	void deleteEngine_slot();

	void startScan_slot(QUuid uniqueId, const QString &filePath);

	void handleWorkerInit_slot(bool success);

signals:

	void processDone_signal(QUuid uniqueId, QJsonObject result);

	void startEngine_signal();

	void startScan_signal(QUuid uniqueId, const QString &filePath);

	void deleteEngine_signal();

	void deletingDone_signal(int id);

	void engineInitFinished_signal(bool success, const QString &engineName);
};

