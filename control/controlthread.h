#pragma once

#include <QThread>
#include <QObject>

class ControlThread : public QThread
{
    Q_OBJECT

public:
    explicit ControlThread(QObject *parent = nullptr);
    ~ControlThread();

    void stop();

protected:
    void run() override;

private:
    bool m_running;
};
