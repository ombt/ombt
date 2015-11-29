#include "DASDbMgr.h"

// transaction control
DASDbTransaction::DASDbTransaction(DASDbMgr &dasdbmgr, bool lockforwrite):
    dasdbmgr_(dasdbmgr)
{
    if (lockforwrite)
        dasdbmgr_.lockForWrite();
    else
        dasdbmgr_.lockForRead();
}

DASDbTransaction::~DASDbTransaction()
{
    dasdbmgr_.unlock();
}

// das db manager
DASDbMgr::DASDbMgr() { }
DASDbMgr::~DASDbMgr() { rwlock_.unlock(); }

// db locks
void 
DASDbMgr::lockForRead()
{
    rwlock_.lockForRead();
}

void 
DASDbMgr::lockForWrite()
{
    rwlock_.lockForWrite();
}

void 
DASDbMgr::unlock()
{
    rwlock_.unlock();
}

// db access with locks
void 
DASDbMgr::readFileNodeToFiles(
    const QString &filenode, QSet<QString> &files, bool &found)
{
    rwlock_.lockForRead();
    readFileNodeToFilesNoLock(filenode, files, found);
    rwlock_.unlock();
}

void 
DASDbMgr::insertFileNodeToFiles(
    const QString &filenode, const QSet<QString> &files)
{
    rwlock_.lockForWrite();
    insertFileNodeToFilesNoLock(filenode, files);
    rwlock_.unlock();
}

void 
DASDbMgr::updateFileNodeToFiles(
    const QString &filenode, const QSet<QString> &files)
{
    rwlock_.lockForWrite();
    updateFileNodeToFilesNoLock(filenode, files);
    rwlock_.unlock();
}

void 
DASDbMgr::deleteFileNodeToFiles(const QString &filenode)
{
    rwlock_.lockForWrite();
    deleteFileNodeToFilesNoLock(filenode);
    rwlock_.unlock();
}

void 
DASDbMgr::insertFileIntoFileNodeToFiles(
    const QString &filenode, const QString &file)
{
    rwlock_.lockForWrite();
    rwlock_.unlock();
}

void 
DASDbMgr::deleteFileFromFileNodeToFiles(
    const QString &filenode, const QString &file)
{
    rwlock_.lockForWrite();
    rwlock_.unlock();
}

void 
DASDbMgr::readFileMetaHash(const QString &file, DASDbSchema::FileMetaHashData &hash, bool &found)
{
    rwlock_.lockForRead();
    readFileMetaHashNoLock(file, hash, found);
    rwlock_.unlock();
}

void 
DASDbMgr::insertFileMetaHash(const QString &file, const DASDbSchema::FileMetaHashData &hash)
{
    rwlock_.lockForWrite();
    insertFileMetaHashNoLock(file, hash);
    rwlock_.unlock();
}

void 
DASDbMgr::updateFileMetaHash(const QString &file, const DASDbSchema::FileMetaHashData &hash)
{
    rwlock_.lockForWrite();
    updateFileMetaHashNoLock(file, hash);
    rwlock_.unlock();
}

void 
DASDbMgr::deleteFileMetaHash(const QString &file)
{
    rwlock_.lockForWrite();
    deleteFileMetaHashNoLock(file);
    rwlock_.unlock();
}

void 
DASDbMgr::readFileNodeToPath(const QString &file, DASDbSchema::FileNodeToPath &data, bool &found)
{
    rwlock_.lockForRead();
    readFileNodeToPathNoLock(file, data, found);
    rwlock_.unlock();
}

void 
DASDbMgr::insertFileNodeToPath(const QString &file, const DASDbSchema::FileNodeToPath &data)
{
    rwlock_.lockForWrite();
    insertFileNodeToPathNoLock(file, data);
    rwlock_.unlock();
}

void 
DASDbMgr::updateFileNodeToPath(const QString &file, const DASDbSchema::FileNodeToPath &data)
{
    rwlock_.lockForWrite();
    updateFileNodeToPathNoLock(file, data);
    rwlock_.unlock();
}

void 
DASDbMgr::deleteFileNodeToPath(const QString &file)
{
    rwlock_.lockForWrite();
    deleteFileNodeToPathNoLock(file);
    rwlock_.unlock();
}

void 
DASDbMgr::readPathToFileNode(const QString &path, DASDbSchema::PathToFileNode &data, bool &found)
{
    rwlock_.lockForRead();
    readPathToFileNodeNoLock(path, data, found);
    rwlock_.unlock();
}

void 
DASDbMgr::insertPathToFileNode(const QString &path, const DASDbSchema::PathToFileNode &data)
{
    rwlock_.lockForWrite();
    insertPathToFileNodeNoLock(path, data);
    rwlock_.unlock();
}

void 
DASDbMgr::updatePathToFileNode(const QString &path, const DASDbSchema::PathToFileNode &data)
{
    rwlock_.lockForWrite();
    updatePathToFileNodeNoLock(path, data);
    rwlock_.unlock();
}

void 
DASDbMgr::deletePathToFileNode(const QString &path)
{
    rwlock_.lockForWrite();
    deletePathToFileNodeNoLock(path);
    rwlock_.unlock();
}

// db access without locks
void 
DASDbMgr::readFileNodeToFilesNoLock(
    const QString &filenode, QSet<QString> &files, bool &found)
{
    files.clear();
    found = false;
    QHash<QString, QSet<QString> >::iterator it = filenode2files_.find(filenode);
    if (it != filenode2files_.end())
    {
        found = true;
        files = *it;
    }
}

void 
DASDbMgr::insertFileNodeToFilesNoLock(
    const QString &filenode, const QSet<QString> &files)
{
    filenode2files_[filenode] = files;
}

void 
DASDbMgr::updateFileNodeToFilesNoLock(
    const QString &filenode, const QSet<QString> &files)
{
    filenode2files_[filenode] = files;
}

void 
DASDbMgr::deleteFileNodeToFilesNoLock(const QString &filenode)
{
    QHash<QString, QSet<QString> >::iterator it = filenode2files_.find(filenode);
    if (it != filenode2files_.end())
        filenode2files_.erase(it);
}

void 
DASDbMgr::readFileMetaHashNoLock(
    const QString &file, DASDbSchema::FileMetaHashData &hash, bool &found)
{
    QCRITDUMP(file);
    found = false;
    QHash<QString, DASDbSchema::FileMetaHashData>::iterator it = file2metahash_.find(file);
    if (it != file2metahash_.end())
    {
        found = true;
        hash = *it;
    }
    QCRITDUMP(found);
}

void 
DASDbMgr::insertFileMetaHashNoLock(const QString &file, const DASDbSchema::FileMetaHashData &hash)
{
    QCRITDUMP2(file, hash.filenode_);
    file2metahash_[file] = hash;
    file2metahash_[file].timer_ = ++timer_;
}

void 
DASDbMgr::updateFileMetaHashNoLock(const QString &file, const DASDbSchema::FileMetaHashData &hash)
{
    QCRITDUMP2(file, hash.filenode_);
    file2metahash_[file] = hash;
    file2metahash_[file].timer_ = ++timer_;
}

void 
DASDbMgr::deleteFileMetaHashNoLock(const QString &file)
{
    QCRITDUMP(file);
    QHash<QString, DASDbSchema::FileMetaHashData>::iterator it = file2metahash_.find(file);
    if (it != file2metahash_.end())
        file2metahash_.erase(it);
}

void 
DASDbMgr::readFileNodeToPathNoLock(
    const QString &filenode, DASDbSchema::FileNodeToPath &data, bool &found)
{
    QCRITDUMP2(filenode, data.path_);
    found = false;
    QHash<QString, DASDbSchema::FileNodeToPath>::iterator it = filenode2path_.find(filenode);
    if (it != filenode2path_.end())
    {
        found = true;
        data = *it;
    }
    QCRITDUMP(found);
}

void 
DASDbMgr::insertFileNodeToPathNoLock(const QString &filenode, const DASDbSchema::FileNodeToPath &data)
{
    QCRITDUMP2(filenode, data.path_);
    filenode2path_[filenode] = data;
}

void 
DASDbMgr::updateFileNodeToPathNoLock(const QString &filenode, const DASDbSchema::FileNodeToPath &data)
{
    QCRITDUMP2(filenode, data.path_);
    filenode2path_[filenode] = data;
}

void 
DASDbMgr::deleteFileNodeToPathNoLock(const QString &filenode)
{
    QCRITDUMP(filenode);
    QHash<QString, DASDbSchema::FileNodeToPath>::iterator it = filenode2path_.find(filenode);
    if (it != filenode2path_.end())
        filenode2path_.erase(it);
}

void 
DASDbMgr::readPathToFileNodeNoLock(
    const QString &path, DASDbSchema::PathToFileNode &data, bool &found)
{
    QCRITDUMP2(path, data.filenode_);
    found = false;
    QHash<QString, DASDbSchema::PathToFileNode>::iterator it = path2filenode_.find(path);
    if (it != path2filenode_.end())
    {
        found = true;
        data = *it;
    }
    QCRITDUMP(found);
}

void 
DASDbMgr::insertPathToFileNodeNoLock(const QString &path, const DASDbSchema::PathToFileNode &data)
{
    QCRITDUMP2(path, data.filenode_);
    path2filenode_[path] = data;
}

void 
DASDbMgr::updatePathToFileNodeNoLock(const QString &path, const DASDbSchema::PathToFileNode &data)
{
    QCRITDUMP2(path, data.filenode_);
    path2filenode_[path] = data;
}

void 
DASDbMgr::deletePathToFileNodeNoLock(const QString &path)
{
    QCRITDUMP(path);
    QHash<QString, DASDbSchema::PathToFileNode>::iterator it = path2filenode_.find(path);
    if (it != path2filenode_.end())
        path2filenode_.erase(it);
}

