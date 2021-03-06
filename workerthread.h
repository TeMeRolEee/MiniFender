#pragma once

#include <QtCore/QProcess>
#include <QtCore/QThread>
#include <QtCore/QUuid>
#include <QtCore/QString>
#include <QtCore/QLibrary>

typedef void (*scanFile);

class WorkerThread : public QThread {
Q_OBJECT
public:
	WorkerThread(const QString &enginePath);

	~WorkerThread();

protected:
	void run() override;

private:
	QLibrary *engine;
	QString enginePath;

public slots:

	void process_slot(QUuid id, const QString &filePath);

	void startWorker_slot();

	void errorHandling(QProcess::ProcessError error);

signals:

	void processDone_signal(QUuid id, QJsonObject resultArray);

	void startWorker_signal();

	void startProcess_signal(QUuid id, const QString &filePath);

	void engineInitFinished_signal(bool success);
};


