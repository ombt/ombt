#ifndef FILESYSTEMMONITOR_H
#define FILESYSTEMMONITOR_H

// contains the Q_OS definitions
#include <QtGlobal>

#if defined(Q_OS_WIN32)
    #include "../FileSystemMonitoring/winfilesystemwatcher.h"
#elif defined(Q_OS_LINUX)
    #include "linuxfilesystemwatcher.h"
#else
    #include "../FileSystemMonitoring/filesystemwatcher.h"
#endif

enum FileSystemChangeState
{
    Created,
    Deleted,
    Changed,
    Renamed,
    Moved
};

class FileSystemMonitor  : public QObject
{
    Q_OBJECT
signals:
    void fileCreated(const QString & path);
    void directoryCreated(const QString & path);
    void fileDeleted(const QString & path);
    void directoryDeleted(const QString & path);
    void fileChanged(const QString & path);
    void directoryChanged(const QString & path);
    void fileRenamed(const QString & oldPath,const QString & newPath);
    void directoryRenamed(const QString & oldPath, const QString & newPath);
    void statusMessage(const QString &message);

public:
    explicit  FileSystemMonitor(QObject *parent = 0);
    virtual ~FileSystemMonitor() ;

public slots:

private slots:
    void onAddDirectoryWatch(const QString & path);
    void onRemoveDirectoryWatch(const QString & path);

    void onFileCreated(const QString & path);
    void onDirectoryCreated(const QString & path);
    void onFileDeleted(const QString & path);
    void onDirectoryDeleted(const QString & path);
    void onFileChanged(const QString & path);
    void onDirectoryChanged(const QString & path);
    void onFileRenamed(const QString & oldPath,const QString & newPath);
    void onDirectoryRenamed(const QString & oldPath, const QString & newPath);

private:
    void assignSignalsAndSlots();
    FileSystemWatcher  _fileSystemWatcher;
};

#endif // FILESYSTEMMONITOR_H
