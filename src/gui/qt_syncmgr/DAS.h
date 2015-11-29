#ifndef __DAS_H
#define __DAS_H

#include <QObject>

#include "FileNode.h"
#include "MsgQueue.h"
#include "DBMgr.h"
#include "FileNodeMgr.h"
#include "FileSystemMonitor.h"
#include "FileSystemWatcher.h"
#include "SynchMgr.h"

class DAS: public QObject
{
    Q_OBJECT

signals:
    void statusMessage(const QString &message);
    void addDirectoryWatch(const QString &filenode, const QString &path);
    void removeDirectoryWatch(const QString &filenode, const QString &path);

public:
    explicit DAS();
    virtual ~DAS();

public slots:
    void onAddDirectoryWatch(const QString &filenode, const QString &path);
    void onRemoveDirectoryWatch(const QString &filenode, const QString &path);
    void onStatusMessage(const QString &message);

private:
    void assignSignalsAndSlots();

protected:
    FileNode filenode_;
    MsgQueue synchmsgqueue_;
    DBMgr dbmgr_;
    FileSystemWatcher filesystemwatcher_;
    FileSystemMonitor filesystemmonitor_;
    FileNodeMgr filenodemgr_;
    SynchMgr synchmgr_;
};

#endif
