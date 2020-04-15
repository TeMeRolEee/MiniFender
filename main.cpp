
#include <memory>

#include <QtCore/QCoreApplication>
#include <QtCore/QCommandLineParser>
#include "core.h"

int main(int argc, char *argv[]) {
	QCoreApplication app(argc, argv);
	QCoreApplication::setApplicationName("MiniFender");
	QCoreApplication::setApplicationVersion("0.85");

	QCommandLineParser parser;
	parser.setApplicationDescription("Handles multiple scan engines");
	parser.addHelpOption();
	parser.addVersionOption();

	QString rootDirectory = QCoreApplication::applicationDirPath();

	auto core = std::make_unique<Core>(rootDirectory);
	core->start();

	auto *isRegistered = new bool();
	*isRegistered = false;

	auto isChecked = new bool();
	*isChecked = false;

	if (!core->parseSerial(rootDirectory + "/settings/auth.ini", isRegistered, isChecked)) {
		qCritical() << "[MAIN]\t" << "Cannot authenticate the product. Shutting down";
		core->quit();
		core->wait();
		QCoreApplication::exit(1);
		return 1;
	}

	for (int i = 0; i < 10; ++i) {
		if (*isChecked) {
			break;
		} else {
			std::this_thread::sleep_for(std::chrono_literals::operator ""ms(250));
		}
	}

	if (!*isRegistered) {
		qCritical() << "[MAIN]\t" << "Not registered product. Shutting down";
		delete isRegistered;
		core->quit();
		core->wait();
		QCoreApplication::exit(1);
		return 1;
	}

	if (!core->init(rootDirectory + "/settings/settings.ini")) {
		qCritical() << "[MAIN]\t" << "Cannot parse settings. Shutting down";
		core->quit();
		core->wait();
		QCoreApplication::exit(1);
		return 1;
	}

	return QCoreApplication::exec();
}
