#include "FileNodeMgr.h"

// ctors and dtor
FileNodeMgr::FileNodeMgr(DBMgr &dbmgr):
    QObject(), dbmgr_(dbmgr)
{
    assignSignalsAndSlots();
}

FileNodeMgr::~FileNodeMgr()
{
}

// set up connections
void FileNodeMgr::assignSignalsAndSlots()
{
}

void 
FileNodeMgr::onAddDirectoryWatch(const QString &filenode, const QString &path)
{
    bool found;
    DASDBSchema::PathToFileNode data;
    dbmgr_.readPathToFileNode(path, data, found);
    if (found)
    {
        emit(statusMessage(QString("Path %1 already assigned to file node %2").arg(path).arg(data.filenode_)));
    }
    else
    {
        emit(addDirectoryWatch(filenode, path));
    }
}

void 
FileNodeMgr::onRemoveDirectoryWatch(const QString &filenode, const QString &path)
{
    bool found;
    DASDBSchema::FileNodeToPath data;
    dbmgr_.readFileNodeToPath(path, data, found);
    if (!found)
    {
        emit(statusMessage(QString("File node %1 not found").arg(filenode)));
    }
    else if (data.path_ != path)
    {
        emit(statusMessage(QString("File node %1 path %2 is not the same as %3").arg(filenode).arg(data.path_).arg(path)));
    }
    {
        emit(removeDirectoryWatch(filenode, path));
    }
}

void 
FileNodeMgr::onStatusMessage(const QString &message)
{
    emit(statusMessage(message));
}

void 
FileNodeMgr::onDirectoryDeleted(const QString &path)
{
    bool found;
    DASDBSchema::PathToFileNode data;
    dbmgr_.readPathToFileNode(path, data, found);
    if (!found)
    {
        // not associated with a filenode -- ignore it.
        return;
    }

    // delete from file node table
    DBTransaction transaction(dbmgr_, true);
    dbmgr_.deletePathToFileNodeNoLock(path);
    dbmgr_.deleteFileNodeToPathNoLock(data.filenode_);

    emit(statusMessage(QString("File node %1 path %2 deleted by user").arg(data.filenode_).arg(path)));
}

void 
FileNodeMgr::onDirectoryRenamed(const QString &oldpath, const QString &newpath)
{
    // check if the old path was associated with a file node
    bool found;
    DASDBSchema::PathToFileNode p2fndata;
    dbmgr_.readPathToFileNodeNoLock(oldpath, p2fndata, found);
    if (!found)
    {
        // not associated with a filenode -- ignore it.
        return;
    }
    DASDBSchema::FileNodeToPath fn2pdata;
    dbmgr_.readFileNodeToPathNoLock(p2fndata.filenode_, fn2pdata, found);
    if (!found)
    {
        // inconsistent data - report it.
        emit(statusMessage(QString("File node %1 for old path %2 found in PathToFileNode but NOT in FileNodeToPath").arg(p2fndata.filenode_).arg(oldpath)));
        return;
    }

    // rename file node 
    DBTransaction transaction(dbmgr_, true);
    dbmgr_.deletePathToFileNodeNoLock(oldpath);
    dbmgr_.deleteFileNodeToPathNoLock(p2fndata.filenode_);
    fn2pdata.path_ = newpath;
    dbmgr_.insertPathToFileNodeNoLock(newpath, p2fndata);
    dbmgr_.insertFileNodeToPathNoLock(p2fndata.filenode_, fn2pdata);

    emit(statusMessage(QString("File node %1 updated: old path %2, new path %3").arg(p2fndata.filenode_).arg(oldpath).arg(newpath)));
}

