#include "DBMgr.h"

// transaction control
DBTransaction::DBTransaction(DBMgr &dbmgr, bool lockforwrite):
    dbmgr_(dbmgr)
{
    if (lockforwrite)
        dbmgr_.lockForWrite();
    else
        dbmgr_.lockForRead();
}

DBTransaction::~DBTransaction()
{
    dbmgr_.unlock();
}

// das db manager ctors and dtor
DBMgr::DBMgr()
{
    assignSignalsAndSlots();
}

DBMgr::~DBMgr()
{
}

// set up connections
void DBMgr::assignSignalsAndSlots()
{
}

// db locks
void 
DBMgr::lockForRead()
{
    rwlock_.lockForRead();
}

void 
DBMgr::lockForWrite()
{
    rwlock_.lockForWrite();
}

void 
DBMgr::unlock()
{
    rwlock_.unlock();
}

// db access with locks
void 
DBMgr::readFileNodeToPath(const QString &file, DASDBSchema::FileNodeToPath &data, bool &found)
{
    rwlock_.lockForRead();
    readFileNodeToPathNoLock(file, data, found);
    rwlock_.unlock();
}

void 
DBMgr::insertFileNodeToPath(const QString &file, const DASDBSchema::FileNodeToPath &data)
{
    rwlock_.lockForWrite();
    insertFileNodeToPathNoLock(file, data);
    rwlock_.unlock();
}

void 
DBMgr::updateFileNodeToPath(const QString &file, const DASDBSchema::FileNodeToPath &data)
{
    rwlock_.lockForWrite();
    updateFileNodeToPathNoLock(file, data);
    rwlock_.unlock();
}

void 
DBMgr::deleteFileNodeToPath(const QString &file)
{
    rwlock_.lockForWrite();
    deleteFileNodeToPathNoLock(file);
    rwlock_.unlock();
}

void 
DBMgr::readPathToFileNode(const QString &path, DASDBSchema::PathToFileNode &data, bool &found)
{
    rwlock_.lockForRead();
    readPathToFileNodeNoLock(path, data, found);
    rwlock_.unlock();
}

void 
DBMgr::insertPathToFileNode(const QString &path, const DASDBSchema::PathToFileNode &data)
{
    rwlock_.lockForWrite();
    insertPathToFileNodeNoLock(path, data);
    rwlock_.unlock();
}

void 
DBMgr::updatePathToFileNode(const QString &path, const DASDBSchema::PathToFileNode &data)
{
    rwlock_.lockForWrite();
    updatePathToFileNodeNoLock(path, data);
    rwlock_.unlock();
}

void 
DBMgr::deletePathToFileNode(const QString &path)
{
    rwlock_.lockForWrite();
    deletePathToFileNodeNoLock(path);
    rwlock_.unlock();
}

// db access without locks
void 
DBMgr::readFileNodeToPathNoLock(
    const QString &filenode, DASDBSchema::FileNodeToPath &data, bool &found)
{
    found = false;
    QHash<QString, DASDBSchema::FileNodeToPath>::iterator it = filenode2path_.find(filenode);
    if (it != filenode2path_.end())
    {
        found = true;
        data = *it;
    }
}

void 
DBMgr::insertFileNodeToPathNoLock(const QString &filenode, const DASDBSchema::FileNodeToPath &data)
{
    filenode2path_[filenode] = data;
}

void 
DBMgr::updateFileNodeToPathNoLock(const QString &filenode, const DASDBSchema::FileNodeToPath &data)
{
    filenode2path_[filenode] = data;
}

void 
DBMgr::deleteFileNodeToPathNoLock(const QString &filenode)
{
    QHash<QString, DASDBSchema::FileNodeToPath>::iterator it = filenode2path_.find(filenode);
    if (it != filenode2path_.end())
        filenode2path_.erase(it);
}

void 
DBMgr::readPathToFileNodeNoLock(
    const QString &path, DASDBSchema::PathToFileNode &data, bool &found)
{
    found = false;
    QHash<QString, DASDBSchema::PathToFileNode>::iterator it = path2filenode_.find(path);
    if (it != path2filenode_.end())
    {
        found = true;
        data = *it;
    }
}

void 
DBMgr::insertPathToFileNodeNoLock(const QString &path, const DASDBSchema::PathToFileNode &data)
{
    path2filenode_[path] = data;
}

void 
DBMgr::updatePathToFileNodeNoLock(const QString &path, const DASDBSchema::PathToFileNode &data)
{
    path2filenode_[path] = data;
}

void 
DBMgr::deletePathToFileNodeNoLock(const QString &path)
{
    QHash<QString, DASDBSchema::PathToFileNode>::iterator it = path2filenode_.find(path);
    if (it != path2filenode_.end())
        path2filenode_.erase(it);
}

