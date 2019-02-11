#include "clihandler.h"
#include <iostream>

#include <QDebug>

void CliHandler::run() {
    std::string input;
    while (true) {
        qDebug() << "Waiting for input...";
        std::getline(std::cin, input);
        emit newTask_signal(QString::fromStdString(input));
    }
    QThread::run();
}

