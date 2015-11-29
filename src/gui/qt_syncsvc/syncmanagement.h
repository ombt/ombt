
#ifndef SYNCMANAGEMENT_H
#define SYNCMANAGEMENT_H

#include <QObject>
#include <QString>
#include <QThread>
#include <QStringList>
#include <QSharedPointer>

#include "msgqueue.h"
#include "dataaccessmanagement.h"
#include "transactionlog.h"
#include "syncservice.h"

class SyncManagement;

class SyncManagementThread: public QThread
{
    Q_OBJECT

public:
    explicit SyncManagementThread(SyncManagement &syncmgr);
    virtual ~SyncManagementThread();

    // run synch manager message loop here.
    virtual void run();

protected:
    SyncManagement &syncmgr_;
};

class SyncManagement : public QObject
{
    Q_OBJECT

public:
    enum Operations
    {
        Error = 0, // QString().toInt() returns 0 if converion fails.
        FileCreated,
        DirectoryCreated,
        FileDeleted,
        DirectoryDeleted,
        FileChanged,
        DirectoryChanged,
        FileRenamed,
        DirectoryRenamed,
        StartDaToDaSync
    };

    explicit SyncManagement(QObject *parent = 0);
    virtual ~SyncManagement();

    bool syncComplete(const QString &filenode, const QString &filenodepath, const QString &relativefilepath, QStringList &hashvalues, QString &translogblob);
    void startDaToDaSync(const QString &filenode, const QString &filenodepath, QString &translogblob, bool authoritative);
    void transferFinalStatus(const QString &filenode, const QString &filenodepath, const QString &relativefilepath, unsigned int status);

    void messageLoop();
    void done();

    void queueFileChanged(Operations oper, const QString &filenode, const QString &filenodepath, const QString &relativefilepath, const QString &path);
    void queueFileRenamed(Operations oper, const QString &filenode, const QString &filenodepath, const QString &oldPath,const QString &newPath);

signals:
    void statusMessage(const QString &message);

public slots:

protected:
    // main handling msg loop started in a separate thread
    // msg handlers.
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

protected:
    // utilities
    void generateHashValues(const QString &filenodepath, const QString &relativefilepath, QStringList &hashvalues);

protected:
    typedef QHash<QString, QSharedPointer<SyncService> > SyncServiceMachines;
    typedef SyncServiceMachines::iterator SyncServiceMachinesIt;

    MsgQueue queue_;
    DataAccessManagement *dataMgr_;
    TransactionLog *transLog_;
    SyncServiceMachines stateMachines_;
};

#endif // SYNCMANAGEMENT_H

