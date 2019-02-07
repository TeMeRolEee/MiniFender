#include "clihandler.h"
#include <iostream>

#include <QDebug>

CliHandler::CliHandler() {
    connect(this, &CliHandler::startListening_signal, this, &CliHandler::startListening_slot);
    connect(this, &CliHandler::stopListening_signal, this , &CliHandler::stopListening_slot);
}

void CliHandler::run() {
    QThread::run();
}

void CliHandler::startListening_slot() {
    std::string input;
    while(continueListening) {
        qDebug() << "Waiting for input...";
        std::getline(std::cin, input);
        emit newTask_signal(QString::fromStdString(input));
    }
}

void CliHandler::stopListening_slot() {
    continueListening = false;
}

void CliHandler::init() {
    qDebug() << "[CLI]\t" << QThread::currentThreadId();
    this->start();
    qDebug() << "[CLI]\t" << QThread::currentThreadId();
}
