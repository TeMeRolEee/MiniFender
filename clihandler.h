#pragma once

#include <QtCore/QThread>

class CliHandler : public QThread {
Q_OBJECT

public:
    CliHandler();

    void init();
protected:
    void run() override;

private:
    bool continueListening = true;

public slots:

    void startListening_slot();

    void stopListening_slot();

signals:
    void newTask_signal(const QString &filePath);

    void startListening_signal();

    void stopListening_signal();
};


