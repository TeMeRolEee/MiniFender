
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

    core->init(rootDirectory + "/settings/settings.ini", rootDirectory + "/db/scanHistoryDB.sqlite");

    return QCoreApplication::exec();
}
