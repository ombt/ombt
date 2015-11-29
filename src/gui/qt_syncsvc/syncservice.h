#ifndef __SYNC_SERVICE_MACHINE_H
#define __SYNC_SERVICE_MACHINE_H

#include <QObject>
#include <QSharedPointer>
#include <QString>
#include <QStringList>

#include "DASQDebug.h"

class SyncService;

class SyncServiceData
{
public:
    friend class SyncServiceState;

    SyncServiceData();
    ~SyncServiceData();

protected:
    int dummy_;
};

class SyncServiceState
{
public:
    virtual ~SyncServiceState() { }

    virtual int handleRemoveDirectoryWatch(SyncService *service, QStringList &msgparts) = 0;
    virtual int handleAddDirectoryWatch(SyncService *service, QStringList &msgparts) = 0;

    virtual int handleFileCreated(SyncService *service, QStringList &msgparts) = 0;
    virtual int handleDirectoryCreated(SyncService *service, QStringList &msgparts) = 0;
    virtual int handleFileDeleted(SyncService *service, QStringList &msgparts) = 0;
    virtual int handleDirectoryDeleted(SyncService *service, QStringList &msgparts) = 0;
    virtual int handleFileChanged(SyncService *service, QStringList &msgparts) = 0;
    virtual int handleDirectoryChanged(SyncService *service, QStringList &msgparts) = 0;
    virtual int handleFileRenamed(SyncService *service, QStringList &msgparts) = 0;
    virtual int handleDirectoryRenamed(SyncService *service, QStringList &msgparts) = 0;
    virtual int handleException(SyncService *service, QStringList &msgparts) = 0;

    virtual int startDaToDaSync(SyncService *service, QStringList &msgparts) = 0;
    virtual int fileTransferCompleted(SyncService *service, QStringList &msgparts) = 0;
    virtual int metadataTransferCompleted(SyncService *service, QStringList &msgparts) = 0;
    virtual int metadataRequest(SyncService *service, QStringList &msgparts) = 0;
    virtual int syncComplete(SyncService *service, QStringList &msgparts) = 0;
};

// initial state when a DA-to-DA sync process is started
class SyncServiceStartState: public SyncServiceState
{
public:
    SyncServiceStartState(SyncServiceData *data);
    virtual ~SyncServiceStartState();

    virtual int handleRemoveDirectoryWatch(SyncService *service, QStringList &msgparts);
    virtual int handleAddDirectoryWatch(SyncService *service, QStringList &msgparts);

    virtual int handleFileCreated(SyncService *service, QStringList &msgparts);
    virtual int handleDirectoryCreated(SyncService *service, QStringList &msgparts);
    virtual int handleFileDeleted(SyncService *service, QStringList &msgparts);
    virtual int handleDirectoryDeleted(SyncService *service, QStringList &msgparts);
    virtual int handleFileChanged(SyncService *service, QStringList &msgparts);
    virtual int handleDirectoryChanged(SyncService *service, QStringList &msgparts);
    virtual int handleFileRenamed(SyncService *service, QStringList &msgparts);
    virtual int handleDirectoryRenamed(SyncService *service, QStringList &msgparts);
    virtual int handleException(SyncService *service, QStringList &msgparts);

    virtual int startDaToDaSync(SyncService *service, QStringList &msgparts);
    virtual int fileTransferCompleted(SyncService *service, QStringList &msgparts);
    virtual int metadataTransferCompleted(SyncService *service, QStringList &msgparts);
    virtual int metadataRequest(SyncService *service, QStringList &msgparts);
    virtual int syncComplete(SyncService *service, QStringList &msgparts);

protected:
    SyncServiceData *data_;
};

class SyncService
{
public:
    explicit SyncService(SyncServiceData *data);
    ~SyncService();

    int handler(QStringList &msgparts);

    int handleRemoveDirectoryWatch(QStringList &msgparts);
    int handleAddDirectoryWatch(QStringList &msgparts);

    int handleFileCreated(QStringList &msgparts);
    int handleDirectoryCreated(QStringList &msgparts);
    int handleFileDeleted(QStringList &msgparts);
    int handleDirectoryDeleted(QStringList &msgparts);
    int handleFileChanged(QStringList &msgparts);
    int handleDirectoryChanged(QStringList &msgparts);
    int handleFileRenamed(QStringList &msgparts);
    int handleDirectoryRenamed(QStringList &msgparts);
    int handleException(QStringList &msgparts);

    int startDaToDaSync(QStringList &msgparts);
    int fileTransferCompleted(QStringList &msgparts);
    int metadataTransferCompleted(QStringList &msgparts);
    int metadataRequest(QStringList &msgparts);
    int syncComplete(QStringList &msgparts);

private:
    QSharedPointer<SyncServiceData> data_;
    QSharedPointer<SyncServiceState> state_;
};

#endif 
