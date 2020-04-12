#include "authclient.h"

AuthClient::AuthClient() {
	localSocket = new QLocalSocket();
}

AuthClient::~AuthClient() {
	if (localSocket->isOpen()) {
		localSocket->close();
	}

	delete localSocket;
}

bool AuthClient::init(const QString &address) {
	this->ipAddress = address;
	return true;
}

void AuthClient::sendSerialKey_slot(const QString &serial) {
	localSocket->connectToServer(ipAddress);
	qDebug() << "Connection to server:" << localSocket->isOpen();
	QByteArray block;
	QDataStream out(&block, QIODevice::ReadWrite);
	//out.setVersion(QDataStream::Qt_5_12);
	out << serial;
	localSocket->write(block);
	connect(localSocket, &QLocalSocket::disconnected, localSocket, &QLocalSocket::deleteLater);
	localSocket->flush();
	localSocket->waitForBytesWritten(3000);

	QString response = localSocket->readAll();
	qDebug() << response;

	emit recievedResponse(response == ("OK"));
}
