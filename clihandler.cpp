#include "clihandler.h"
#include <iostream>

#include <QDebug>

CliHandler::CliHandler() {
    connect(this, &CliHandler::stopListening_signal, this , &CliHandler::stopListening_slot);
    init();
}

void CliHandler::run() {
    std::string input;
    while(continueListening) {
        qDebug() << "Waiting for input...";
        std::getline(std::cin, input);
        emit newTask_signal(QString::fromStdString(input));
    }
    QThread::run();
}

void CliHandler::stopListening_slot() {
    continueListening = false;
}

void CliHandler::init() {
    qDebug() << "[CLI]\t" << QThread::currentThreadId();
    this->start();
    qDebug() << "[CLI]\t" << QThread::currentThreadId();
}
