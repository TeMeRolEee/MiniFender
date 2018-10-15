#pragma once

#include <QtCore/QObject>
#include <QtCore/QProcess>

class EngineHandler : public QObject {
	Q_OBJECT

public:
	EngineHandler(const QString &programPath, const QStringList &arguments);

	~EngineHandler() override;

	const QString &getResult() const;

private:
	QString programPath;
	QStringList arguments;
	QProcess *engineProcess;
	QString result;

public slots:

	void settingResult_slot();

signals:

	void reportSuccesssResult_signal();

	void reportFailureResult_signal();
};


