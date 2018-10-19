#include <iostream>
#include <chrono>

#include <QtCore/QCoreApplication>
#include <QtCore/QCommandLineParser>
#include <memory>
#include "core.h"

int main(int argc, char *argv[]) {
	QCoreApplication app(argc, argv);
	QCoreApplication::setApplicationName("MiniFender");
	QCoreApplication::setApplicationVersion("0.1");

	QCommandLineParser parser;
	parser.setApplicationDescription("Handles multiple scan engines");
	parser.addHelpOption();
	parser.addVersionOption();

	auto core = std::make_unique<Core>();

	QStringList params;

	params  << "-s" << "/home/temerole/Downloads/Custom_Engines_v4_wrapper_Sellable_Symantec_Custom_Engine_27487_artifacts.zip";

	core->addNewEngine("/home/temerole/Academy/BlackListEngine/cmake-build-debug/BlackListEngine", params);

	return QCoreApplication::exec();
}