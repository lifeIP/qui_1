#include "errorlogio.h"

#include <QCoreApplication>
#include <QDateTime>
#include <QDir>
#include <QFile>
#include <QMutex>
static QString logsDir()
{
    return QCoreApplication::applicationDirPath() + QString::fromUtf8("/logs");
}

QString ErrorLogIO::errorsFilePath()
{
    return logsDir() + QString::fromUtf8("/errors.log");
}

QString ErrorLogIO::archiveFilePath()
{
    return logsDir() + QString::fromUtf8("/archive.log");
}

static QString consoleLogFilePath()
{
    return QCoreApplication::applicationDirPath() + QString::fromUtf8("/logs/console.log");
}

void ErrorLogIO::ensureDefaultFilesExist()
{
    QDir dir;
    dir.mkpath(logsDir());

    QString errPath = errorsFilePath();
    if (!QFile::exists(errPath)) {
        QFile f(errPath);
        if (f.open(QIODevice::WriteOnly)) {
            const QByteArray data = QString::fromUtf8(
                "19.02.25 14:24:17|Нет связи с контроллером|#e74c3c|1\n"
                "19.02.25 13:09:29|Не удается установить необходимую температуру нагрева|#e74c3c|1\n"
                "19.02.25 12:15:00|Ошибка связи|#e74c3c|1\n"
                "19.02.25 11:30:22|Ошибка связи|#f1c40f|1\n"
                "19.02.25 10:57:00|Ошибка связи|#e74c3c|1\n"
                "19.02.25 10:34:06|Ошибка связи|#e74c3c|0\n"
            ).toUtf8();
            f.write(data);
            f.close();
        }
    }

    QString archPath = archiveFilePath();
    if (!QFile::exists(archPath)) {
        QFile f(archPath);
        if (f.open(QIODevice::WriteOnly)) {
            const QByteArray data = QString::fromUtf8(
                "19.02.25 14:24:17|Нет связи с контроллером\n"
                "19.02.25 13:09:29|Не удается установить необходимую температуру нагрева\n"
                "19.02.25 12:33:48|Ошибка связи\n"
                "19.02.25 10:57:00|Ошибка связи\n"
                "19.02.25 10:34:06|Ошибка связи\n"
            ).toUtf8();
            f.write(data);
            f.close();
        }
    }
}

QList<ErrorLogEntry> ErrorLogIO::readErrorsFile()
{
    QList<ErrorLogEntry> entries;
    QFile f(errorsFilePath());
    if (!f.open(QIODevice::ReadOnly))
        return entries;

    QString content = QString::fromUtf8(f.readAll());
    f.close();

    const QStringList lines = content.split(QString::fromUtf8("\n"));
    for (const QString &line : lines) {
        QString trimmed = line.trimmed();
        if (trimmed.isEmpty())
            continue;
        QStringList parts = trimmed.split(QString::fromUtf8("|"));
        if (parts.size() >= 2) {
            ErrorLogEntry e;
            e.time = parts[0].trimmed();
            e.message = parts[1].trimmed();
            e.color = parts.size() >= 3 ? parts[2].trimmed() : QString::fromUtf8("#e74c3c");
            e.resettable = parts.size() >= 4 && parts[3].trimmed() == QString::fromUtf8("1");
            entries.append(e);
        }
    }
    return entries;
}

QList<ArchiveEntry> ErrorLogIO::readArchiveFile()
{
    QList<ArchiveEntry> entries;
    QFile f(archiveFilePath());
    if (!f.open(QIODevice::ReadOnly))
        return entries;

    QString content = QString::fromUtf8(f.readAll());
    f.close();

    const QStringList lines = content.split(QString::fromUtf8("\n"));
    for (const QString &line : lines) {
        QString trimmed = line.trimmed();
        if (trimmed.isEmpty())
            continue;
        QStringList parts = trimmed.split(QString::fromUtf8("|"));
        if (parts.size() >= 2) {
            ArchiveEntry e;
            e.time = parts[0].trimmed();
            e.message = parts[1].trimmed();
            entries.append(e);
        }
    }
    return entries;
}

void ErrorLogIO::appendToErrorsLog(const QString &message, const QString &color, bool resettable)
{
    ensureDefaultFilesExist();
    QString timeStr = QDateTime::currentDateTime().toString(QString::fromUtf8("dd.MM.yy HH:mm:ss"));
    QString col = color.isEmpty() ? QString::fromUtf8("#e74c3c") : color;
    QString line = timeStr + QString::fromUtf8("|") + message + QString::fromUtf8("|") + col
        + QString::fromUtf8("|") + (resettable ? QString::fromUtf8("1") : QString::fromUtf8("0")) + QString::fromUtf8("\n");

    QFile f(errorsFilePath());
    if (f.open(QIODevice::Append)) {
        f.write(line.toUtf8());
        f.close();
    }
}

void ErrorLogIO::appendToArchive(const QString &message)
{
    ensureDefaultFilesExist();
    QString timeStr = QDateTime::currentDateTime().toString(QString::fromUtf8("dd.MM.yy HH:mm:ss"));
    QString line = timeStr + QString::fromUtf8("|") + message + QString::fromUtf8("\n");

    QFile f(archiveFilePath());
    if (f.open(QIODevice::Append)) {
        f.write(line.toUtf8());
        f.close();
    }
}

static QMutex s_consoleLogMutex;

void ErrorLogIO::appendToConsoleLog(const QString &line)
{
    QMutexLocker lock(&s_consoleLogMutex);
    QDir dir;
    dir.mkpath(QCoreApplication::applicationDirPath() + QString::fromUtf8("/logs"));

    QString timeStr = QDateTime::currentDateTime().toString(QString::fromUtf8("yyyy-MM-dd HH:mm:ss"));
    QString logLine = timeStr + QString::fromUtf8(" | ") + line + QString::fromUtf8("\n");

    QFile f(consoleLogFilePath());
    if (f.open(QIODevice::Append)) {
        f.write(logLine.toUtf8());
        f.close();
    }
}
