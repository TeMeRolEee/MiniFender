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

    params  << "-s" << "/home/temerole/Academy/TestEngines/build/TestEngines";

    QString enginePath("/home/temerole/Academy/TestEngines/build/TestEngines");

    //auto workerThread = std::make_unique<WorkerThread>(enginePath, params);

    auto engine = new Engine(1, enginePath);

    engine->start();

    engine->addNewEngine_slot(params);

	engine->startEngine_slot(0);

	// getting a little crazy

	engine->addNewEngine_slot(params);

	engine->startEngine_slot(1);

	engine->addNewEngine_slot(params);

	engine->startEngine_slot(2);

	engine->addNewEngine_slot(params);

	engine->startEngine_slot(3);

	engine->addNewEngine_slot(params);

	engine->startEngine_slot(4);

	engine->addNewEngine_slot(params);

	engine->startEngine_slot(5);

	engine->startEngine_signal();

    //auto workerThread = new WorkerThread(enginePath, params);

	//workerThread->start();

	//core->addNewEngine("/home/temerole/Academy/TestEngines/cmake-build-debug/TestEngines", params);



	return QCoreApplication::exec();
}