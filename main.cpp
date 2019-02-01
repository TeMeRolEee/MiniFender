#include <iostream>
#include <chrono>
#include <memory>

#include <QtCore/QCoreApplication>
#include <QtCore/QCommandLineParser>
#include <QDebug>
#include "core.h"

#include "workerthread.h"

int main(int argc, char *argv[]) {
	QCoreApplication app(argc, argv);
	QCoreApplication::setApplicationName("MiniFender");
	QCoreApplication::setApplicationVersion("0.2");

	QCommandLineParser parser;
	parser.setApplicationDescription("Handles multiple scan engines");
	parser.addHelpOption();
	parser.addVersionOption();

	/*
    *   QStringList params;
    *
    *   params  << "-s" << "/home/temerole/Academy/TestEngines/build/TestEngines";
    *
    *   QString enginePath("/home/temerole/Academy/TestEngines/build/TestEngines");
    */

    /*
     *  Example ini:
     *  [MoodyEngine]
     *  path="/home/temerole/Academy/TestEngines/build/TestEngines"
     *  scan_parameter="-s"
     *  [TestingEngine]
     *  path="/home/temerole/Academy/TestEngines/build/TestEngines"
     *  scan_parameter="-s"
     */

    auto core = std::make_unique<Core>();

    QString rootDir = QCoreApplication::applicationDirPath();

    core->init(rootDir + "/settings/settings.ini", rootDir + "/db/scanHistoryDB.sqlite");

    core->startNewScanTask("/home/temerole/Academy/TestEngines/build/TestEngines");

	core->startNewScanTask("/home/temerole/Academy/TestEngines/build/TestEngines");

	core->startNewScanTask("/home/temerole/Academy/TestEngines/build/TestEngines");

	core->startNewScanTask("/home/temerole/Academy/TestEngines/build/TestEngines");

	core->startNewScanTask("/home/temerole/Academy/TestEngines/build/TestEngines");

	core->listEngineCount();

	return QCoreApplication::exec();
}
