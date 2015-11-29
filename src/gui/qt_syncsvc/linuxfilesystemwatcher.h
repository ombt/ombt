#ifndef __FILE_SYSTEM_WATCHER_H
#define __FILE_SYSTEM_WATCHER_H

#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <sys/inotify.h>
#include <iostream>

#include <QObject>
#include <QDebug>
#include <QThread>
#include <QSocketNotifier>
#include <QDir>
#include <QMap>
#include <QHash>
#include <QSet>
#include <QMutex>
#include <QMutexLocker>
#include <QFileSystemWatcher>

#include "dataaccessmanagement.h"

class FileWatcherThread;

class FileWatcher: public QObject
{
    Q_OBJECT

signals:
    void fileCreated(const QString &path);
    void directoryCreated(const QString &path);
    void fileDeleted(const QString &path);
    void directoryDeleted(const QString &path);
    void fileChanged(const QString &path);
    void directoryChanged(const QString &path);
    void fileRenamed(const QString &oldpath, const QString &newpath);
    void directoryRenamed(const QString &oldpath, const QString &newpath);
    void exception(const QString &description, const QString &path);
    void statusMessage(const QString &message);

public:
    explicit FileWatcher();
    virtual ~FileWatcher();

    void addPath(const QString &path);
    void removePath(const QString &path);
    void addFileWatcher(const QString &path, unsigned long mask);
    void removeFileWatcher(const QString &path);
    void addDirectoryWatcher(const QString &path, unsigned long mask);
    void removeDirectoryWatcher(const QString &path);

    friend class FileWatcherThread;

private slots:
    void onDataReceived();

protected:
    int inotifyfd_;

    typedef int ID;
    typedef QString WP;
    typedef QSet<WP> WPS;
    typedef QHash<WP, ID> WP2ID;
    typedef QHash<ID, WP> ID2WP;
    typedef QHash<quint64, QString> C2OP;

    QMutex mutex_;
    WPS watchpoints_;
    WP2ID watchpoint2id_;
    ID2WP id2watchpoint_;
    C2OP cookie2oldpath_;
};

class FileWatcherThread: public QThread
{
    Q_OBJECT

public:
    explicit FileWatcherThread(FileWatcher &filewatcher);
    virtual ~FileWatcherThread();

    virtual void run();

protected:
    FileWatcher &filewatcher_;
};

class FileSystemWatcher: public QObject
{
    Q_OBJECT

signals:
    void fileCreated(const QString &path);
    void directoryCreated(const QString &path);
    void fileDeleted(const QString &path);
    void directoryDeleted(const QString &path);
    void fileChanged(const QString &path);
    void directoryChanged(const QString &path);
    void fileRenamed(const QString &oldPath,const QString &newPath);
    void directoryRenamed(const QString &oldPath, const QString &newPath);
    void exception(const QString &description, const QString &path);
    void statusMessage(const QString &message);

public:
    explicit FileSystemWatcher(QObject *parent = 0);
    virtual ~FileSystemWatcher();

    void addDirectoryWatch(const QString &path);
    void endDirectoryWatch(const QString &path);

    void addDirectoryWatcher(const QString &path);
    void endDirectoryWatcher(const QString &path);

    void beginRecursiveDirectoryWatch(const QString &path);
    void endRecursiveDirectoryWatch(const QString &path);

public:
    static bool isValidPath(const QString &path) {
        return QDir(path).exists();
    }

private slots:
    void onWatcherDirectoryChanged(const QString &path);

    void onFileCreated(const QString &path);
    void onDirectoryCreated(const QString &path);
    void onFileDeleted(const QString &path);
    void onDirectoryDeleted(const QString &path);
    void onFileChanged(const QString &path);
    void onDirectoryChanged(const QString &path);
    void onFileRenamed(const QString &oldpath, const QString &newpath);
    void onDirectoryRenamed(const QString &oldpath, const QString &newpath);
    void onException(const QString &description, const QString &path);

private:
    void assignSignalsAndSlots();
    bool isDirectoryWatched(const QString &path);

private:
    DataAccessManagement *dataMgr_;
    FileWatcher filewatcher_;
    FileWatcherThread filewatcherthread_;
};

#endif 
