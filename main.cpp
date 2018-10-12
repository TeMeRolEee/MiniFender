#include <iostream>

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

	core->addNewEngine();

	return QCoreApplication::exec();
}