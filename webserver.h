#pragma once

#include <QtCore/QThread>
#include <QtNetwork>

#include <httplib.h>

using namespace httplib;

class WebServer : public QThread  {
	Q_OBJECT

public:
	WebServer(QObject* parent);
	~WebServer() override;

private:
	Server *svr;

	int init();

public slots:
	void initWebServer_slot();
	void deInitWebServer_slot();

	void setWebServer_slot(int port, QHostAddress ipAddress);

signals:
	void initWebServer_signal();

	void deInitWebServer_signal();
};


