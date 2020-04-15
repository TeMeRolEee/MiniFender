#include "clihandler.h"
#include <iostream>

#include <QDebug>

void CliHandler::run() {
	std::string input;
	while (!stopCli) {
		//qDebug() << "Waiting for input...";
		std::getline(std::cin, input);
		emit newTask_signal(QString::fromStdString(input));
	}
	QThread::run();
}

void CliHandler::stopCli_slot() {
	stopCli = true;
}

