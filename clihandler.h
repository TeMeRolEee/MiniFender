#pragma once

#include <QtCore/QThread>

class CliHandler : public QThread {
Q_OBJECT

protected:
    void run() override;

public slots:

signals:

    void newTask_signal(const QString &filePath);
};


