#include <iostream>
#include <chrono>
#include <memory>

#include <QtCore/QCoreApplication>
#include <QtCore/QCommandLineParser>
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

    QString enginePath = "/home/temerole/Academy/TestEngines/build/TestEngines";

    params  << "-s" << "/home/temerole/Academy/TestEngines/build/TestEngines";

    //auto workerThread = std::make_unique<WorkerThread>(enginePath, params);

    auto engine = new Engine(1, enginePath);

    engine->addNewEngine_slot(params);

	engine->startEngine_slot(0);

    //auto workerThread = new WorkerThread(enginePath, params);

	//workerThread->start();

	//core->addNewEngine("/home/temerole/Academy/TestEngines/cmake-build-debug/TestEngines", params);



	return QCoreApplication::exec();
}