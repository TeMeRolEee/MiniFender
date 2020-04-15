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
	Engine(int id, const QString &enginePath, const QString &scanParameter);

	~Engine() override;

	const QString &getEnginePath() const;

protected:
	void run() override;

private:
	int id = 0;

	QString enginePath;
	QString scanParameter;

private:
	QMap<QUuid, WorkerThread *> *engineProcesses;

public slots:

	void handleProcessDone_slot(QUuid uniqueId, QJsonObject result);

	void addNewWorker_slot(QUuid uniqueId, const QString &parameter);

	void deleteEngine_slot();

signals:

	void processDone_signal(QUuid uniqueId, QJsonObject result);

	void startEngine_signal();

	void deleteEngine_signal();

	void deletingDone_signal(int id);
};

