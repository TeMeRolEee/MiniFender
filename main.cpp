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

    QStringList params;

    params  << "-s" << "/home/temerole/Academy/TestEngines/build/TestEngines";

    QString enginePath("/home/temerole/Academy/TestEngines/build/TestEngines");

    //auto workerThread = std::make_unique<WorkerThread>(enginePath, params);

    auto core = std::make_unique<Core>();

    //qDebug() << QCoreApplication::applicationDirPath();

    /*
     *  Example ini:
     *  [MoodyEngine]
     *  path="/home/temerole/Academy/TestEngines/build/TestEngines"
     *  scan_parameter="-s"
     *  [TestingEngine]
     *  path="/home/temerole/Academy/TestEngines/build/TestEngines"
     *  scan_parameter="-s"
     */

    core->init(QCoreApplication::applicationDirPath() + "/settings/settings.ini");

    //core->addNewEngine(enginePath, "-s");

    core->startNewScanTask("/home/temerole/Academy/TestEngines/build/TestEngines");

	return QCoreApplication::exec();
}