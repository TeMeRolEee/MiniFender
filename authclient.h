#pragma once

#include <QtCore/QThread>
#include <QtCore/QDataStream>
#include <QtNetwork/QLocalSocket>

class AuthClient : public QThread {
Q_OBJECT

public:
	explicit AuthClient();
	~AuthClient() override;

	bool init(const QString &address);

private:
	QLocalSocket *localSocket = nullptr;
	QString ipAddress;

public slots:
	void sendSerialKey_slot(const QString &serial);

signals:
	void recievedResponse(bool isGood);
};


