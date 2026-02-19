#pragma once

#include <QStringList>

struct ErrorLogEntry {
    QString time;
    QString message;
    QString color;
    bool resettable;
};

struct ArchiveEntry {
    QString time;
    QString message;
};

namespace ErrorLogIO {

QString errorsFilePath();
QString archiveFilePath();

void ensureDefaultFilesExist();

QList<ErrorLogEntry> readErrorsFile();
QList<ArchiveEntry> readArchiveFile();

// Append new entries (writes UTF-8)
void appendToErrorsLog(const QString &message, const QString &color = QString(), bool resettable = true);
void appendToArchive(const QString &message);

}
