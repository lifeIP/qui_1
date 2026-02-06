#include "control/controlthread.h"
#include "control/debugconsole.h"

#include <QDebug>

ControlThread::ControlThread(QObject *parent)
    : QThread(parent)
    , m_running(true)
{
}

ControlThread::~ControlThread()
{
    stop();
    wait();
}

void ControlThread::stop()
{
    m_running = false;
}

void ControlThread::run()
{
    qDebug() << "ControlThread: Starting control thread";
    
    DebugConsole console;
    console.start();
    
    // Основной цикл потока
    while (m_running) {
        QThread::msleep(100); // Небольшая задержка для проверки состояния
    }
    
    console.stop();
    qDebug() << "ControlThread: Control thread stopped";
}
