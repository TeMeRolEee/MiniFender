#include "webserver.h"

WebServer::WebServer(QObject *parent) : QThread(parent) {
	svr = new Server();
}

WebServer::~WebServer() {
	if (svr->is_running()) {
		svr->stop();
	}

	delete svr;
}

int WebServer::init() {
	return 0;
}

void WebServer::initWebServer_slot() {

}

void WebServer::setWebServer_slot(int port, QHostAddress ipAddress) {

}

void WebServer::deInitWebServer_slot() {

}
