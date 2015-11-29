
#include "syncservice.h"
#include "syncmanagement.h"
#include "DASQDebug.h"

// sync service data
SyncServiceData::SyncServiceData(): 
    dummy_(0)
{
}

SyncServiceData::~SyncServiceData()
{
}

// sync service start state handler routines
SyncServiceStartState::SyncServiceStartState(SyncServiceData *data): 
    data_(data)
{
}

SyncServiceStartState::~SyncServiceStartState()
{
}

int 
SyncServiceStartState::handleRemoveDirectoryWatch(SyncService *service, QStringList &msgparts)
{
    return(1);
}

int 
SyncServiceStartState::handleAddDirectoryWatch(SyncService *service, QStringList &msgparts)
{
    return(1);
}

int 
SyncServiceStartState:: handleFileCreated(SyncService *service, QStringList &msgparts)
{
    return(1);
}

int 
SyncServiceStartState::handleDirectoryCreated(SyncService *service, QStringList &msgparts)
{
    return(1);
}

int 
SyncServiceStartState::handleFileDeleted(SyncService *service, QStringList &msgparts)
{
    return(1);
}

int 
SyncServiceStartState::handleDirectoryDeleted(SyncService *service, QStringList &msgparts)
{
    return(1);
}

int 
SyncServiceStartState::handleFileChanged(SyncService *service, QStringList &msgparts)
{
    return(1);
}

int 
SyncServiceStartState::handleDirectoryChanged(SyncService *service, QStringList &msgparts)
{
    return(1);
}

int 
SyncServiceStartState::handleFileRenamed(SyncService *service, QStringList &msgparts)
{
    return(1);
}

int 
SyncServiceStartState::handleDirectoryRenamed(SyncService *service, QStringList &msgparts)
{
    return(1);
}

int 
SyncServiceStartState::handleException(SyncService *service, QStringList &msgparts)
{
    return(1);
}

int 
SyncServiceStartState::startDaToDaSync(SyncService *service, QStringList &msgparts)
{
    return(1);
}

int 
SyncServiceStartState::fileTransferCompleted(SyncService *service, QStringList &msgparts)
{
    return(1);
}

int 
SyncServiceStartState::metadataTransferCompleted(SyncService *service, QStringList &msgparts)
{
    return(1);
}

int 
SyncServiceStartState::metadataRequest(SyncService *service, QStringList &msgparts)
{
    return(1);
}

int 
SyncServiceStartState::syncComplete(SyncService *service, QStringList &msgparts)
{
    return(1);
}

// sync service as presented to the outside
SyncService::SyncService(SyncServiceData *data):
    data_(data), state_(NULL)
{
    state_ = QSharedPointer<SyncServiceState>(new SyncServiceStartState(data_.data()));
}

SyncService::~SyncService()
{
}

int
SyncService::handler(QStringList &msgparts)
{
    QString stroper = msgparts.takeFirst();
    SyncManagement::Operations oper =
        static_cast<SyncManagement::Operations>(stroper.toInt());
    switch (oper)
    {
    case SyncManagement::FileCreated:
        return handleFileCreated(msgparts);
    case SyncManagement::DirectoryCreated:
        return handleDirectoryCreated(msgparts);
    case SyncManagement::FileDeleted:
        return handleFileDeleted(msgparts);
    case SyncManagement::DirectoryDeleted:
        return handleDirectoryDeleted(msgparts);
    case SyncManagement::FileChanged:
        return handleFileChanged(msgparts);
    case SyncManagement::DirectoryChanged:
        return handleDirectoryChanged(msgparts);
    case SyncManagement::FileRenamed:
        return handleFileRenamed(msgparts);
    case SyncManagement::DirectoryRenamed:
        return handleDirectoryRenamed(msgparts);
    case SyncManagement::StartDaToDaSync:
        return startDaToDaSync(msgparts);
    case SyncManagement::Error:
    default:
        QCRITDUMP(QString("ERROR: %1").arg(msgparts.join("\n")));
        return(-1);
    }
}

int
SyncService::handleRemoveDirectoryWatch(QStringList &msgparts)
{
    return(-1);
}

int 
SyncService::handleAddDirectoryWatch(QStringList &msgparts)
{
   return(-1);
}

int 
SyncService::handleFileCreated(QStringList &msgparts)
{
    return(state_->handleFileCreated(this, msgparts));
}

int 
SyncService::handleDirectoryCreated(QStringList &msgparts)
{
    return(state_->handleDirectoryCreated(this, msgparts));
}

int 
SyncService::handleFileDeleted(QStringList &msgparts)
{
    return(state_->handleFileDeleted(this, msgparts));
}

int 
SyncService::handleDirectoryDeleted(QStringList &msgparts)
{
    return(state_->handleDirectoryDeleted(this, msgparts));
}

int 
SyncService::handleFileChanged(QStringList &msgparts)
{
    return(state_->handleFileChanged(this, msgparts));
}

int 
SyncService::handleDirectoryChanged(QStringList &msgparts)
{
    return(state_->handleDirectoryChanged(this, msgparts));
}

int 
SyncService::handleFileRenamed(QStringList &msgparts)
{
    return(state_->handleFileRenamed(this, msgparts));
}

int 
SyncService::handleDirectoryRenamed(QStringList &msgparts)
{
    return(state_->handleDirectoryRenamed(this, msgparts));
}

int 
SyncService::handleException(QStringList &msgparts)
{
    return(state_->handleException(this, msgparts));
}

int 
SyncService::startDaToDaSync(QStringList &msgparts)
{
    return(state_->startDaToDaSync(this, msgparts));
}

int 
SyncService::fileTransferCompleted(QStringList &msgparts)
{
    return(state_->fileTransferCompleted(this, msgparts));
}

int 
SyncService::metadataTransferCompleted(QStringList &msgparts)
{
    return(state_->metadataTransferCompleted(this, msgparts));
}

int 
SyncService::metadataRequest(QStringList &msgparts)
{
    return(state_->metadataRequest(this, msgparts));
}

int 
SyncService::syncComplete(QStringList &msgparts)
{
    return(state_->syncComplete(this, msgparts));
}

