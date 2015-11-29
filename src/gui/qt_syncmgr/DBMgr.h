#ifndef __DB_MGR_H
#define __DB_MGR_H

#include <QObject>
#include <QString>
#include <QSet>
#include <QMap>
#include <QHash>
#include <QReadWriteLock>
#include <QByteArray>

#include "DASQDebug.h"

namespace DASDBSchema
{
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
};

class DBMgr: public QObject
{
    Q_OBJECT

signals:
    void statusMessage(const QString &message);
    void exception(const QString &description, const QString & path);

public:
    explicit DBMgr();
    virtual ~DBMgr();

    // single-operation per transaction and locked
    void readPathToFileNode(
        const QString &path, DASDBSchema::PathToFileNode &data, bool &found);
    void insertPathToFileNode(
        const QString &path, const DASDBSchema::PathToFileNode &data);
    void updatePathToFileNode(
        const QString &path, const DASDBSchema::PathToFileNode &data);
    void deletePathToFileNode(const QString &path);

    void readFileNodeToPath(
        const QString &file, DASDBSchema::FileNodeToPath &data, bool &found);
    void insertFileNodeToPath(
        const QString &file, const DASDBSchema::FileNodeToPath &data);
    void updateFileNodeToPath(
        const QString &file, const DASDBSchema::FileNodeToPath &data);
    void deleteFileNodeToPath(const QString &file);


    // multiple-operation per transaction and NOT locked
    void readPathToFileNodeNoLock(
        const QString &path, DASDBSchema::PathToFileNode &data, bool &found);
    void insertPathToFileNodeNoLock(
        const QString &path, const DASDBSchema::PathToFileNode &data);
    void updatePathToFileNodeNoLock(
        const QString &path, const DASDBSchema::PathToFileNode &data);
    void deletePathToFileNodeNoLock(const QString &path);

    void readFileNodeToPathNoLock(
        const QString &file, DASDBSchema::FileNodeToPath &data, bool &found);
    void insertFileNodeToPathNoLock(
        const QString &file, const DASDBSchema::FileNodeToPath &data);
    void updateFileNodeToPathNoLock(
        const QString &file, const DASDBSchema::FileNodeToPath &data);
    void deleteFileNodeToPathNoLock(const QString &file);

    // transaction control
    void lockForRead();
    void lockForWrite();
    void unlock();

private:
    void assignSignalsAndSlots();

protected:
    QReadWriteLock rwlock_;
    unsigned long long timer_;

    QHash<QString, DASDBSchema::FileNodeToPath> filenode2path_;
    QHash<QString, DASDBSchema::PathToFileNode> path2filenode_;
};

class DBTransaction
{
public:
    DBTransaction(DBMgr &dbmgr, bool lockForWrite);
    ~DBTransaction();

private:
    DBMgr &dbmgr_;
};


#endif 
