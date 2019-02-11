#include <iostream>
#include <chrono>
#include <memory>

#include <QtCore/QCoreApplication>
#include <QtCore/QCommandLineParser>
#include <QDebug>
#include <QtCore/QDir>
#include "core.h"

int main(int argc, char *argv[]) {
	QCoreApplication app(argc, argv);
	QCoreApplication::setApplicationName("MiniFender");
	QCoreApplication::setApplicationVersion("0.8");

	QCommandLineParser parser;
	parser.setApplicationDescription("Handles multiple scan engines");
	parser.addHelpOption();
	parser.addVersionOption();

    QString rootDirectory = QCoreApplication::applicationDirPath();

    auto core = std::make_unique<Core>(rootDirectory);
    core->start();

	if (!(core->init(rootDirectory + "/settings/settings.ini", rootDirectory + "/db/scanHistoryDB.sqlite"))) {
		core->quit();
		core->wait(1000);
		QCoreApplication::exit(1);
		QCoreApplication::quit();
		return 1;
	}

	return QCoreApplication::exec();
}
