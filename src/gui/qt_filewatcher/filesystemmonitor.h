#ifndef FILESYSTEMMONITOR_H
#define FILESYSTEMMONITOR_H

#include <QObject>

#include "linuxfilesystemwatcher.h"
#include "DASDbMgr.h"
#include "DASSynchMgr.h"

enum FileSystemChangeState
{
    Created,
    Deleted,
    Changed,
    Renamed,
    Moved
};

class FileSystemWatcher;

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
    void exception(const QString & description, const QString & path);

public:
    explicit  FileSystemMonitor(DASDbMgr &dasdbmgr, DASSynchMgr &synchMgr, QObject *parent = 0);
    virtual ~FileSystemMonitor() ;

public slots:

private slots:
    void onAddDirectoryWatch(const QString &filenode, const QString & path);
    void onRemoveDirectoryWatch(const QString &filenode, const QString & path);

    void onFileCreated(const QString & path);
    void onDirectoryCreated(const QString & path);
    void onFileDeleted(const QString & path);
    void onDirectoryDeleted(const QString & path);
    void onFileChanged(const QString & path);
    void onDirectoryChanged(const QString & path);
    void onFileRenamed(const QString & oldPath,const QString & newPath);
    void onDirectoryRenamed(const QString & oldPath, const QString & newPath);
    void onException(const QString & description, const QString & path);

private:
    void assignSignalsAndSlots();
    void markPathAsDirty(const QString &path);

private:
    DASDbMgr &_dasDbMgr;
    DASSynchMgr &_synchMgr;
    FileSystemWatcher _fileSystemWatcher;
};

#endif // FILESYSTEMMONITOR_H
