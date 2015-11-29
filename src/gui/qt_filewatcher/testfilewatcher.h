#ifndef __TESTFILEWATCHER_H
#define __TESTFILEWATCHER_H

#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <sys/inotify.h>
#include <iostream>

#include <QCoreApplication>
#include <QObject>
#include <QDebug>
#include <QThread>
#include <QSocketNotifier>
#include <QDir>
#include <QMap>
#include <QSet>
#include <QMutex>
#include <QMutexLocker>

#include "DASQDebug.h"

class FileWatcherThread;

class FileWatcher: public QObject
{
    Q_OBJECT

public:
    explicit FileWatcher();
    virtual ~FileWatcher();

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
    typedef QMap<WP, ID> WP2ID;
    typedef QMap<ID, WP> ID2WP;

    QMutex mutex_;
    WPS watchpoints_;
    WP2ID watchpoint2id_;
    ID2WP id2watchpoint_;
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

#endif
