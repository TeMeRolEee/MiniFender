#include <iostream>
#include <chrono>
#include <memory>

#include <QtCore/QCoreApplication>
#include <QtCore/QCommandLineParser>
#include <QDebug>
#include <QtCore/QDir>
#include "core.h"

#include "workerthread.h"

int main(int argc, char *argv[]) {
	QCoreApplication app(argc, argv);
	QCoreApplication::setApplicationName("MiniFender");
	QCoreApplication::setApplicationVersion("0.5");

	QCommandLineParser parser;
	parser.setApplicationDescription("Handles multiple scan engines");
	parser.addHelpOption();
	parser.addVersionOption();

	auto core = std::make_unique<Core>();

	QString rootDir = QCoreApplication::applicationDirPath();

	if (!QDir(rootDir + "/db/").exists()) {
		QDir().mkdir(rootDir + "/db/");
	}

	if (!QDir(rootDir + "/settings/").exists()) {
		QDir().mkdir(rootDir + "/settings/");
	}
	core->start();
	if (!(core->init(rootDir + "/settings/settings.ini", rootDir + "/db/scanHistoryDB.sqlite"))) {
		qDebug() << "SHUTTING DOWN";
		core->quit();
		core->wait();
		QCoreApplication::exit(1);
		QCoreApplication::quit();
		return 1;
	}

	return QCoreApplication::exec();
}
