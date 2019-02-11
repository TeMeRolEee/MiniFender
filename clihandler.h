#pragma once

#include <QtCore/QThread>

class CliHandler : public QThread {
Q_OBJECT

protected:
    void run() override;

private:
    bool continueListening = true;

public slots:

    void stopListening_slot();

signals:
    void newTask_signal(const QString &filePath);

    void stopListening_signal();
};


