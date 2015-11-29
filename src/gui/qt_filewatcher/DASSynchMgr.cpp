
#include "DASSynchMgr.h"

const QString DASSynchMgr::RemoveDirectoryWatch = "removedirectorywatch";
const QString DASSynchMgr::AddDirectoryWatch = "adddirectorywatch";
const QString DASSynchMgr::FileCreated = "filecreated";
const QString DASSynchMgr::DirectoryCreated = "directorycreated";
const QString DASSynchMgr::FileDeleted = "filedeleted";
const QString DASSynchMgr::DirectoryDeleted = "directorydeleted";
const QString DASSynchMgr::FileChanged = "filechanged";
const QString DASSynchMgr::DirectoryChanged = "directorychanged";
const QString DASSynchMgr::FileRenamed = "filerenamed";
const QString DASSynchMgr::DirectoryRenamed = "directoryrenamed";
const QString DASSynchMgr::Exception = "exception";

DASSynchMgr::DASSynchMgr(DASDbMgr &dasdbmgr): dasdbmgr_(dasdbmgr) { } 
DASSynchMgr::~DASSynchMgr() { }

void 
DASSynchMgr::enqueue(const QString &msg)
{
    QDUMP(msg);
    msgqueue_.enqueue(msg);
    QTRACE();
}

bool 
DASSynchMgr::dequeue(QString &msg)
{
    QTRACE();
    bool status = msgqueue_.dequeue(msg);
    QDUMP(msg);
    return(status);
}

bool 
DASSynchMgr::head(QString &msg)
{
    QTRACE();
    bool status = msgqueue_.head(msg);
    QDUMP(msg);
    return(status);
}

MsgQueue &
DASSynchMgr::getMsgQueue()
{
    return msgqueue_;
}

void
DASSynchMgr::removeDirectoryWatch(QStringList &msgparts)
{
    QDUMP(msgparts.join(""));
}

void
DASSynchMgr::addDirectoryWatch(QStringList &msgparts)
{
    QDUMP(msgparts.join(""));

    if (msgparts.isEmpty())
    {
        QERRORDUMP("no filenode and path given", msgparts.join(""));
        return;
    }
    QString filenode(msgparts.front());

    msgparts.pop_front();
    if (msgparts.isEmpty())
    {
        QERRORDUMP("no path given", msgparts.join(""));
        return;
    }
    QString dirpath(msgparts.front());
    QDUMP2(filenode, dirpath);

    bool found;
    DASDbSchema::PathToFileNode path2fn;
    dasdbmgr_.readPathToFileNode(dirpath, path2fn, found);
    if (found)
    {
        QERRORDUMP("Path already monitored", dirpath);
        return;
    }

    QCryptographicHash pathhash(QCryptographicHash::Sha1);
    QDir dir(dirpath);
    DASHash::calcMetaHashForPath(dir.canonicalPath(), pathhash, &dasdbmgr_);

    DASDbTransaction transaction(dasdbmgr_, true);
    dasdbmgr_.insertPathToFileNodeNoLock(
        dirpath, DASDbSchema::PathToFileNode(filenode));
    dasdbmgr_.insertFileNodeToPathNoLock(
        filenode, DASDbSchema::FileNodeToPath(dirpath));
}

void
DASSynchMgr::fileCreated(QStringList &msgparts)
{
    QDUMP(msgparts.join(""));
}

void
DASSynchMgr::directoryCreated(QStringList &msgparts)
{
    QDUMP(msgparts.join(""));
}

void
DASSynchMgr::fileDeleted(QStringList &msgparts)
{
    QDUMP(msgparts.join(""));
}

void
DASSynchMgr::directoryDeleted(QStringList &msgparts)
{
    QDUMP(msgparts.join(""));
}

void
DASSynchMgr::fileChanged(QStringList &msgparts)
{
    QDUMP(msgparts.join(""));
}

void
DASSynchMgr::directoryChanged(QStringList &msgparts)
{
    QDUMP(msgparts.join(""));
}

void
DASSynchMgr::fileRenamed(QStringList &msgparts)
{
    QDUMP(msgparts.join(""));
}

void
DASSynchMgr::directoryRenamed(QStringList &msgparts)
{
    QDUMP(msgparts.join(""));
}

void
DASSynchMgr::exception(QStringList &msgparts)
{
    QDUMP(msgparts.join(""));
}

void
DASSynchMgr::messageLoop()
{
    QTRACE();
    QString msg;
    while (dequeue(msg))
    {
        QDUMP(msg);
        QStringList msgparts = 
            msg.split(QString(";"), QString::KeepEmptyParts, Qt::CaseSensitive);
        if (msgparts.isEmpty())
        {
            // skip empty message.
            QTRACE();
            continue;
        }
        QString cmd = msgparts.front();
        msgparts.pop_front();
        QDUMP(cmd);
        if (cmd == RemoveDirectoryWatch)
        {
            removeDirectoryWatch(msgparts);
        }
        else if (cmd == AddDirectoryWatch)
        {
            addDirectoryWatch(msgparts);
        }
        else if (cmd == FileCreated)
        {
            fileCreated(msgparts);
        }
        else if (cmd == DirectoryCreated)
        {
            directoryCreated(msgparts);
        }
        else if (cmd == FileDeleted)
        {
            fileDeleted(msgparts);
        }
        else if (cmd == DirectoryDeleted)
        {
            directoryDeleted(msgparts);
        }
        else if (cmd == FileChanged)
        {
            fileChanged(msgparts);
        }
        else if (cmd == DirectoryChanged)
        {
            directoryChanged(msgparts);
        }
        else if (cmd == FileRenamed)
        {
            fileRenamed(msgparts);
        }
        else if (cmd == DirectoryRenamed)
        {
            directoryRenamed(msgparts);
        }
        else if (cmd == Exception)
        {
            exception(msgparts);
        }
        else
        {
            QCRITDUMP(msg);
        }
    }
    QTRACE();
}

DASSynchMgrThread::DASSynchMgrThread(DASSynchMgr &synchmgr):
    synchmgr_(synchmgr)
{
    QTRACE();
}

DASSynchMgrThread::~DASSynchMgrThread()
{
    QTRACE();
} 

void
DASSynchMgrThread::run()
{
    QTRACE();
    synchmgr_.messageLoop();
    QTRACE();
}
