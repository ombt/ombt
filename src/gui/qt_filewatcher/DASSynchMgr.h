#ifndef __DAS_SYNCH_MGR_H
#define __DAS_SYNCH_MGR_H

// headers
#include <QObject>
#include <QMutex>
#include <QList>
#include <QString>
#include <QThread>
#include <QStringList>

#include "DASQDebug.h"
#include "DASDbMgr.h"
#include "DASHash.h"
#include "MsgQueue.h"

class DASSynchMgr;

class DASSynchMgrThread: public QThread
{
    Q_OBJECT

public:
    explicit DASSynchMgrThread(DASSynchMgr &synchmgr);
    virtual ~DASSynchMgrThread();

    // run synch manager message loop here.
    virtual void run();

protected:
    DASSynchMgr &synchmgr_;
};

class DASSynchMgr: public QObject
{
    Q_OBJECT

public:
    // message types -- use this for now.
    static const QString RemoveDirectoryWatch;
    static const QString AddDirectoryWatch;
    static const QString FileCreated;
    static const QString DirectoryCreated;
    static const QString FileDeleted;
    static const QString DirectoryDeleted;
    static const QString FileChanged;
    static const QString DirectoryChanged;
    static const QString FileRenamed;
    static const QString DirectoryRenamed;
    static const QString Exception;

public:
    explicit DASSynchMgr(DASDbMgr &dasdbmgr);
    virtual ~DASSynchMgr();

    // queueing messages for sync manager to process
    void enqueue(const QString &msg);
    bool dequeue(QString &msg);
    bool head(QString &msg);

    // main handling msg loop started in a separate thread
    // msg handlers.
    void messageLoop();
    void removeDirectoryWatch(QStringList &msgparts);
    void addDirectoryWatch(QStringList &msgparts);
    void fileCreated(QStringList &msgparts);
    void directoryCreated(QStringList &msgparts);
    void fileDeleted(QStringList &msgparts);
    void directoryDeleted(QStringList &msgparts);
    void fileChanged(QStringList &msgparts);
    void directoryChanged(QStringList &msgparts);
    void fileRenamed(QStringList &msgparts);
    void directoryRenamed(QStringList &msgparts);
    void exception(QStringList &msgparts);

    // misc
    MsgQueue &getMsgQueue();

protected:
    DASDbMgr &dasdbmgr_;
    MsgQueue msgqueue_;
};

#endif
