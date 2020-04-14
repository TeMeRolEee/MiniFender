#include <QtCore/QTimer>
#include "authclient.h"

AuthClient::AuthClient() {
	localSocket = new QTcpSocket();
	connect(localSocket, &QTcpSocket::disconnected, localSocket, &QTcpSocket::deleteLater);
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
	localSocket->connectToHost(ipAddress, 50137);

	if (localSocket->waitForConnected() && localSocket->isOpen()) {
		QByteArray block;
		QDataStream out(&block, QIODevice::ReadWrite);
		out.setVersion(QDataStream::Qt_DefaultCompiledVersion);

		out << QByteArray(serial.toLocal8Bit()).toBase64();

		localSocket->waitForConnected();
		localSocket->write(block);
		localSocket->waitForBytesWritten();
		localSocket->flush();
		localSocket->waitForReadyRead();

		QByteArray block2;
		QDataStream out2(&block2, QIODevice::ReadWrite);

		out2 << localSocket->readAll();

		QString response = QString(QByteArray::fromBase64(block2));

		localSocket->close();
		qDebug() << "[AUTH]\t" << response << (response == ("ACCEPTED"));
		emit recievedResponse(response == ("ACCEPTED"));
	}

	emit recievedResponse(false);
}
