#ifndef _DAS_DB_MGR_H
#define _DAS_DB_MGR_H

#include <QObject>
#include <QString>
#include <QSet>
#include <QMap>
#include <QHash>
#include <QReadWriteLock>
#include <QByteArray>

#include "DASQDebug.h"

namespace DASDbSchema
{

    enum SynchQueueState {
        Queued,
        Proecessing,
        NotInQueue
    };

    struct PathToFileNode
    {
        PathToFileNode(): 
            filenode_() { }
        PathToFileNode(const QString &filenode):
                filenode_(filenode) { }
        ~PathToFileNode() { }

        QString filenode_;
    };

    struct FileNodeToPath
    {
        FileNodeToPath(): 
            path_() { }
        FileNodeToPath(const QString &path):
                path_(path) { }
        ~FileNodeToPath() { }

        QString path_;
    };

    struct FileMetaHashData
    {
        FileMetaHashData(): 
            filenode_(), hash_(), dirty_(false), isdir_(false), 
            synchqstate_(NotInQueue) { }
        FileMetaHashData(const QString &filenode, 
            const QByteArray &hash,
            bool dirty, bool isdir,
            SynchQueueState synchqstate):
                filenode_(filenode),
                hash_(hash), dirty_(dirty), 
                isdir_(isdir), synchqstate_(synchqstate) { }
        ~FileMetaHashData() { }

        QString filenode_;
        QByteArray hash_;
        bool dirty_;
        bool isdir_;
        SynchQueueState synchqstate_;
        unsigned long long timer_;
    };

};

class DASDbMgr: public QObject
{
    Q_OBJECT

signals:
    void exception(const QString & description, const QString & path);

public:
    explicit DASDbMgr();
    virtual ~DASDbMgr();

    // single-operation per transaction and locked
    void readFileNodeToFiles(
        const QString &filenode, QSet<QString> &files, bool &found);
    void insertFileNodeToFiles(
        const QString &filenode, const QSet<QString> &files);
    void updateFileNodeToFiles(
        const QString &filenode, const QSet<QString> &files);
    void deleteFileNodeToFiles(const QString &filenode);

    void readFileMetaHash(
        const QString &file, DASDbSchema::FileMetaHashData &hash, bool &found);
    void insertFileMetaHash(
        const QString &file, const DASDbSchema::FileMetaHashData &hash);
    void updateFileMetaHash(
        const QString &file, const DASDbSchema::FileMetaHashData &hash);
    void deleteFileMetaHash(const QString &file);

    void readFileNodeToPath(
        const QString &file, DASDbSchema::FileNodeToPath &data, bool &found);
    void insertFileNodeToPath(
        const QString &file, const DASDbSchema::FileNodeToPath &data);
    void updateFileNodeToPath(
        const QString &file, const DASDbSchema::FileNodeToPath &data);
    void deleteFileNodeToPath(const QString &file);

    void readPathToFileNode(
        const QString &path, DASDbSchema::PathToFileNode &data, bool &found);
    void insertPathToFileNode(
        const QString &path, const DASDbSchema::PathToFileNode &data);
    void updatePathToFileNode(
        const QString &path, const DASDbSchema::PathToFileNode &data);
    void deletePathToFileNode(const QString &path);

    void insertFileIntoFileNodeToFiles(const QString &filenode, const QString &file);
    void deleteFileFromFileNodeToFiles(const QString &filenode, const QString &file);

    // multiple-operation per transaction and NOT locked
    void readFileNodeToFilesNoLock(
        const QString &filenode, QSet<QString> &files, bool &found);
    void insertFileNodeToFilesNoLock(
        const QString &filenode, const QSet<QString> &files);
    void updateFileNodeToFilesNoLock(
        const QString &filenode, const QSet<QString> &files);
    void deleteFileNodeToFilesNoLock(const QString &filenode);

    void readFileMetaHashNoLock(
        const QString &file, DASDbSchema::FileMetaHashData &hash, bool &found);
    void insertFileMetaHashNoLock(
        const QString &file, const DASDbSchema::FileMetaHashData &hash);
    void updateFileMetaHashNoLock(
        const QString &file, const DASDbSchema::FileMetaHashData &hash);
    void deleteFileMetaHashNoLock(const QString &file);

    void readFileNodeToPathNoLock(
        const QString &file, DASDbSchema::FileNodeToPath &data, bool &found);
    void insertFileNodeToPathNoLock(
        const QString &file, const DASDbSchema::FileNodeToPath &data);
    void updateFileNodeToPathNoLock(
        const QString &file, const DASDbSchema::FileNodeToPath &data);
    void deleteFileNodeToPathNoLock(const QString &file);

    void readPathToFileNodeNoLock(
        const QString &path, DASDbSchema::PathToFileNode &data, bool &found);
    void insertPathToFileNodeNoLock(
        const QString &path, const DASDbSchema::PathToFileNode &data);
    void updatePathToFileNodeNoLock(
        const QString &path, const DASDbSchema::PathToFileNode &data);
    void deletePathToFileNodeNoLock(const QString &path);

    // transaction control
    void lockForRead();
    void lockForWrite();
    void unlock();

protected:
    QReadWriteLock rwlock_;
    unsigned long long timer_;

    QHash<QString, QSet<QString> > filenode2files_;
    QHash<QString, DASDbSchema::FileMetaHashData> file2metahash_;
    QHash<QString, DASDbSchema::FileNodeToPath> filenode2path_;
    QHash<QString, DASDbSchema::PathToFileNode> path2filenode_;
};

class DASDbTransaction
{
public:
    DASDbTransaction(DASDbMgr &dasdbmgr, bool lockForWrite);
    ~DASDbTransaction();

private:
    DASDbMgr &dasdbmgr_;
};


#endif 
