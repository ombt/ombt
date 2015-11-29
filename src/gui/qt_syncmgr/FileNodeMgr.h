#ifndef __FILE_NODE_MGR_H
#define __FILE_NODE_MGR_H

#include <QObject>
#include "DBMgr.h"

class FileNodeMgr: public QObject
{
    Q_OBJECT

signals:
    void addDirectoryWatch(const QString &filenode, const QString &path);
    void removeDirectoryWatch(const QString &filenode, const QString &path);
    void statusMessage(const QString &message);

public:
    explicit FileNodeMgr(DBMgr &dbmgr);
    virtual ~FileNodeMgr();

public slots:
    void onAddDirectoryWatch(const QString &filenode, const QString &path);
    void onRemoveDirectoryWatch(const QString &filenode, const QString &path);
    void onDirectoryDeleted(const QString &path);
    void onDirectoryRenamed(const QString &oldpath, const QString &newpath);
    void onStatusMessage(const QString &message);

private:
    void assignSignalsAndSlots();

protected:
    DBMgr &dbmgr_;
};

#endif
