#ifndef __FILE_SYSTEM_MONITOR_H
#define __FILE_SYSTEM_MONITOR_H

#include <QObject>
#include "DBMgr.h"
#include "MsgQueue.h"
#include "FileSystemWatcher.h"

class FileSystemMonitor: public QObject
{
    Q_OBJECT

signals:
    void directoryDeleted(const QString &path);
    void directoryRenamed(const QString &oldpath, const QString &newpath);
    void statusMessage(const QString &message);
    void exception(const QString &description, const QString &path);

public:
    explicit FileSystemMonitor(DBMgr &dbmgr, MsgQueue &synchmsgqueue);
    virtual ~FileSystemMonitor();

public slots:
    void onAddDirectoryWatch(const QString &filenode, const QString &path);
    void onRemoveDirectoryWatch(const QString &filenode, const QString &path);
    void onStatusMessage(const QString &message);

    void onFileCreated(const QString &path);
    void onDirectoryCreated(const QString &path);
    void onFileDeleted(const QString &path);
    void onDirectoryDeleted(const QString &path);
    void onFileChanged(const QString &path);
    void onDirectoryChanged(const QString &path);
    void onFileRenamed(const QString &oldpath,const QString &newpath);
    void onDirectoryRenamed(const QString &oldpath, const QString &newpath);
    void onException(const QString &description, const QString &path);

private:
    void assignSignalsAndSlots();

protected:
    DBMgr &dbmgr_;
    MsgQueue &synchmsgqueue_;
    FileSystemWatcher filesystemwatcher_;
};

#endif
