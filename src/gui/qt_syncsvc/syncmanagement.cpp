
#include "syncmanagement.h"

SyncManagement::SyncManagement(QObject *parent) :
    QObject(parent), queue_(), dataMgr_(NULL), transLog_(NULL)
{
    dataMgr_ = DataAccessManagement::instance();
    transLog_ = TransactionLog::instance();
}

SyncManagement::~SyncManagement()
{
}

bool 
SyncManagement::syncComplete(const QString &filenode, const QString &filenodepath, const QString &relativefilepath, QStringList &hashvalues, QString &translogblob)
{
    QTRACE();
    DataAccessManagement::SyncStateData data;
    bool found;
    dataMgr_->readSyncStates(filenodepath, data, found);
    if (found)
    {
        // sync in progress since record exists.
        return(false);
    }
    else
    {
        // no record so no sync in progress.
        generateHashValues(filenodepath, relativefilepath, hashvalues);
        transLog_->getTransactionLog(filenode, translogblob);
    }
}

void 
SyncManagement::startDaToDaSync(const QString &filenode, const QString &filenodepath, QString &translogblob, bool authoritative)
{
    queue_.enqueue(QString("%1:%2:%3:%4:%5").arg(filenode).arg(StartDaToDaSync).arg(filenodepath).arg(translogblob).arg(authoritative));
}

void
SyncManagement::transferFinalStatus(const QString &filenode, const QString &filenodepath, const QString &relativefilepath, unsigned int status)
{
}

void 
SyncManagement::queueFileChanged(Operations oper, const QString &filenode, const QString &filenodepath, const QString &relativefilepath, const QString &path)
{
    queue_.enqueue(QString("%1:%2:%3:%4:%5").arg(filenode).arg(oper).arg(filenodepath).arg(relativefilepath). arg(path));
}

void 
SyncManagement::queueFileRenamed(Operations oper, const QString &filenode, const QString &filenodepath, const QString &oldPath,const QString &newPath)
{
    queue_.enqueue(QString("%1:%2:%3:%4:%5").arg(filenode).arg(oper).arg(filenodepath).arg(oldPath). arg(newPath));
}

void
SyncManagement::removeDirectoryWatch(QStringList &msgparts)
{
    QDUMP(msgparts.join(""));
}

void
SyncManagement::addDirectoryWatch(QStringList &msgparts)
{
    QDUMP(msgparts.join(""));
}

void
SyncManagement::fileCreated(QStringList &msgparts)
{
    QDUMP(msgparts.join(""));
}

void
SyncManagement::directoryCreated(QStringList &msgparts)
{
    QDUMP(msgparts.join(""));
}

void
SyncManagement::fileDeleted(QStringList &msgparts)
{
    QDUMP(msgparts.join(""));
}

void
SyncManagement::directoryDeleted(QStringList &msgparts)
{
    QDUMP(msgparts.join(""));
}

void
SyncManagement::fileChanged(QStringList &msgparts)
{
    QDUMP(msgparts.join(""));
}

void
SyncManagement::directoryChanged(QStringList &msgparts)
{
    QDUMP(msgparts.join(""));
}

void
SyncManagement::fileRenamed(QStringList &msgparts)
{
    QDUMP(msgparts.join(""));
}

void
SyncManagement::directoryRenamed(QStringList &msgparts)
{
    QDUMP(msgparts.join(""));
}

void
SyncManagement::exception(QStringList &msgparts)
{
    QDUMP(msgparts.join(""));
}

void
SyncManagement::messageLoop()
{
    QString msg;
    while (queue_.dequeue(msg))
    {
        QStringList msgparts = 
            msg.split(QString(";"), QString::KeepEmptyParts, Qt::CaseSensitive);
        if (msgparts.count() < 2)
        {
            // less than minimum information: filenode, sync operation type
            continue;
        }
        QString filenode = msgparts.takeFirst();

        int status;
        SyncServiceMachinesIt mit = stateMachines_.find(filenode);
        if (mit != stateMachines_.end())
        {
            status = (*mit)->handler(msgparts);
        }
        else
        {
            // new sync request
            SyncServiceData *data = new SyncServiceData;
            QSharedPointer<SyncService> service(new SyncService(data));
            stateMachines_[filenode] = service;
            service->handler(msgparts);
        }
        if (status < 0)
        {
            // error and terminate
            QTRACE();
            stateMachines_.remove(filenode);
        }
        else if (status > 0)
        {
            // success but terminate
            QTRACE();
            stateMachines_.remove(filenode);
        }
        else
        {
            // success -- for now
            QTRACE();
        }
    }
}

void
SyncManagement::done()
{
    queue_.done();
}

void
SyncManagement::generateHashValues(const QString &filenodepath, const QString &relativefilepath, QStringList &hashvalues)
{
    QTRACE();
}

SyncManagementThread::SyncManagementThread(SyncManagement &syncmgr):
    syncmgr_(syncmgr)
{
    QTRACE();
}

SyncManagementThread::~SyncManagementThread()
{
    QTRACE();
} 

void
SyncManagementThread::run()
{
    QTRACE();
    syncmgr_.messageLoop();
    QTRACE();
}

