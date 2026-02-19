#pragma once

#include <QObject>
#include <QTextStream>
#include <QStringList>

class DebugConsole : public QObject
{
    Q_OBJECT

public:
    explicit DebugConsole(QObject *parent = nullptr);
    ~DebugConsole();

    void start();
    void stop();
    void processCommand(const QString &command);

signals:
    void commandReceived(const QString &command);

private:
    void printHelp();
    void printWelcome();
    bool parseAndExecute(const QString &command);
    void writeLine(const QString &text);

    QTextStream *m_inputStream;
    QTextStream *m_outputStream;
    bool m_running;
};
