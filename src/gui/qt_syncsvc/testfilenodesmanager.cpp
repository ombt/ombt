
#include "testfilenodesmanager.h"

FileNodesManager::FileNodesManager(
        SyncManagement &syncmgr, QObject *parent):
        QObject(parent), 
        syncmgr_(syncmgr), pdatamgr_(NULL), ptranslog_(NULL)
   
{
    QTRACE();
    pdatamgr_ = DataAccessManagement::instance(DataAccessManagement::Default);
    ptranslog_ = TransactionLog::instance();
}

FileNodesManager::~FileNodesManager()
{
    QTRACE();
}

void FileNodesManager::getFileDescriptionsAsXml(bool recurse, const QString& rootPath, QByteArray* formatterOutput)
{
    QTRACE();
}

void FileNodesManager::getFileNodePath(const QString& fileNode, QString& path)
{
    QTRACE();
}

void FileNodesManager::getFileNodeName(QString& fileNode,const  QString& path)
{
    QTRACE();
}

void FileNodesManager::onFilenodeAdded( const QString& nodeName, const QString& path)
{
    QTRACE();
}

void FileNodesManager::onFilenodeRemoved(const QString& nodeName,const QString& path)
{
    QTRACE();
}

void FileNodesManager::onServiceAdded(const QString& serviceType, const QString& serviceName)
{
    QTRACE();
}

void FileNodesManager::onServiceRemoved(const QString& serviceType, const QString& serviceName)
{
    QTRACE();
}

void FileNodesManager::onAttachFilenodeToService(const QString& serviceType,const QString& serviceName, const QString& nodeName)
{
    QTRACE();
}

void FileNodesManager::onDetachFilenodeFromService(const QString& serviceType,const QString& serviceName, const QString& nodeName, const QString& path)
{
    QTRACE();
}

//void FileNodesManager::onServiceTableUpdated(const TableRows& tableRows)
//{
//}

void FileNodesManager::requestFilesMetadata(bool recurse, const QString filenode, const QString& relativePath)
{
    QTRACE();
}

bool 
FileNodesManager::doFileNodeChanged(const QString &path)
{
    QDUMP(path);

    QString filenode;
    QString filenodepath;
    QString relativefilepath;
    bool found = pdatamgr_->getFileNodeData(path, filenode, filenodepath, relativefilepath);
    if (!found)
    {
        emit(statusMessage(QString("No File Node found for path %1").arg(path)));
        return(false);
    }

    QStringList hashvalues;
    QString translogblob;
    bool sendFileNodesChanged = syncmgr_.syncComplete(
        filenode, filenodepath, relativefilepath, hashvalues, translogblob);
    if (sendFileNodesChanged)
    {
        emit(fileNodeChanged(filenode, filenodepath, relativefilepath, 
                             hashvalues, translogblob));
    }

    return(true);
}

void 
FileNodesManager::onFileCreated( const QString & path )
{
    QDUMP(path);

    QString filenode;
    QString filenodepath;
    QString relativefilepath;
    bool found = pdatamgr_->getFileNodeData(path, filenode, filenodepath, relativefilepath);
    if (!found)
    {
        emit(statusMessage(QString("No File Node found for path %1").arg(path)));
        return;
    }

    ptranslog_->writeFileCreatedLogRecord(filenode, filenodepath, relativefilepath);
    syncmgr_.queueFileChanged(SyncManagement::FileCreated, filenode, filenodepath, relativefilepath, path);

    QStringList hashvalues;
    QString translogblob;
    bool sendFileNodesChanged = syncmgr_.syncComplete(
        filenode, filenodepath, relativefilepath, hashvalues, translogblob);
    if (sendFileNodesChanged)
    {
        emit(fileNodeChanged(filenode, filenodepath, relativefilepath, 
                             hashvalues, translogblob));
    }

    emit(fileCreated(path));
}

void 
FileNodesManager::onDirectoryCreated( const QString & path )
{
    QDUMP(path);

    QString filenode;
    QString filenodepath;
    QString relativefilepath;
    bool found = pdatamgr_->getFileNodeData(path, filenode, filenodepath, relativefilepath);
    if (!found)
    {
        emit(statusMessage(QString("No File Node found for path %1").arg(path)));
        return;
    }

    ptranslog_->writeDirectoryCreatedLogRecord(filenode, filenodepath, relativefilepath);
    syncmgr_.queueFileChanged(SyncManagement::DirectoryCreated, filenode, filenodepath, relativefilepath, path);

    QStringList hashvalues;
    QString translogblob;
    bool sendFileNodesChanged = syncmgr_.syncComplete(
        filenode, filenodepath, relativefilepath, hashvalues, translogblob);
    if (sendFileNodesChanged)
    {
        emit(fileNodeChanged(filenode, filenodepath, relativefilepath, 
                             hashvalues, translogblob));
    }

    emit(directoryCreated(path));
}

void 
FileNodesManager::onFileDeleted( const QString & path )
{
    QDUMP(path);

    QString filenode;
    QString filenodepath;
    QString relativefilepath;
    bool found = pdatamgr_->getFileNodeData(path, filenode, filenodepath, relativefilepath);
    if (!found)
    {
        emit(statusMessage(QString("No File Node found for path %1").arg(path)));
        return;
    }

    ptranslog_->writeFileDeletedLogRecord(filenode, filenodepath, relativefilepath);
    syncmgr_.queueFileChanged(SyncManagement::FileDeleted, filenode, filenodepath, relativefilepath, path);

    QStringList hashvalues;
    QString translogblob;
    bool sendFileNodesChanged = syncmgr_.syncComplete(
        filenode, filenodepath, relativefilepath, hashvalues, translogblob);
    if (sendFileNodesChanged)
    {
        emit(fileNodeChanged(filenode, filenodepath, relativefilepath, 
                             hashvalues, translogblob));
    }

    emit(fileDeleted(path));
}

void 
FileNodesManager::onDirectoryDeleted( const QString & path )
{
    QDUMP(path);

    QString filenode;
    QString filenodepath;
    QString relativefilepath;
    bool found = pdatamgr_->getFileNodeData(path, filenode, filenodepath, relativefilepath);
    if (!found)
    {
        emit(statusMessage(QString("No File Node found for path %1").arg(path)));
        return;
    }

    ptranslog_->writeDirectoryDeletedLogRecord(filenode, filenodepath, relativefilepath);
    syncmgr_.queueFileChanged(SyncManagement::DirectoryDeleted, filenode, filenodepath, relativefilepath, path);

    QStringList hashvalues;
    QString translogblob;
    bool sendFileNodesChanged = syncmgr_.syncComplete(
        filenode, filenodepath, relativefilepath, hashvalues, translogblob);
    if (sendFileNodesChanged)
    {
        emit(fileNodeChanged(filenode, filenodepath, relativefilepath, 
                             hashvalues, translogblob));
    }

    emit(directoryDeleted(path));
}

void 
FileNodesManager::onFileChanged( const QString & path )
{
    QDUMP(path);

    QString filenode;
    QString filenodepath;
    QString relativefilepath;
    bool found = pdatamgr_->getFileNodeData(path, filenode, filenodepath, relativefilepath);
    if (!found)
    {
        emit(statusMessage(QString("No File Node found for path %1").arg(path)));
        return;
    }

    ptranslog_->writeFileChangedLogRecord(filenode, filenodepath, relativefilepath);
    syncmgr_.queueFileChanged(SyncManagement::FileChanged, filenode, filenodepath, relativefilepath, path);

    QStringList hashvalues;
    QString translogblob;
    bool sendFileNodesChanged = syncmgr_.syncComplete(
        filenode, filenodepath, relativefilepath, hashvalues, translogblob);
    if (sendFileNodesChanged)
    {
        emit(fileNodeChanged(filenode, filenodepath, relativefilepath, 
                             hashvalues, translogblob));
    }

    emit(fileChanged(path));
}

void 
FileNodesManager::onDirectoryChanged( const QString & path )
{
    QDUMP(path);

    QString filenode;
    QString filenodepath;
    QString relativefilepath;
    bool found = pdatamgr_->getFileNodeData(path, filenode, filenodepath, relativefilepath);
    if (!found)
    {
        emit(statusMessage(QString("No File Node found for path %1").arg(path)));
        return;
    }

    ptranslog_->writeDirectoryChangedLogRecord(filenode, filenodepath, relativefilepath);
    syncmgr_.queueFileChanged(SyncManagement::DirectoryChanged, filenode, filenodepath, relativefilepath, path);

    QStringList hashvalues;
    QString translogblob;
    bool sendFileNodesChanged = syncmgr_.syncComplete(
        filenode, filenodepath, relativefilepath, hashvalues, translogblob);
    if (sendFileNodesChanged)
    {
        emit(fileNodeChanged(filenode, filenodepath, relativefilepath, 
                             hashvalues, translogblob));
    }

    emit(directoryChanged(path));
}

void 
FileNodesManager::onFileRenamed( const QString & oldPath, const QString & newPath )
{
    QDUMP2(oldPath, newPath);
    emit(fileRenamed(oldPath, newPath));
}

void 
FileNodesManager::onDirectoryRenamed(const QString & oldPath, const QString & newPath )
{
    QDUMP2(oldPath, newPath);
    emit(directoryRenamed(oldPath, newPath));
}

void 
FileNodesManager::onAddDirectoryWatch(const QString &filenode, const QString &path)
{
    QDUMP(path);
    pdatamgr_->addFilenode(filenode, path);
    emit(addDirectoryWatch(path));
}

void 
FileNodesManager::onRemoveDirectoryWatch(const QString &filenode, const QString &path)
{
    QDUMP(path);
    pdatamgr_->removeFilenode(filenode, path);
    emit(removeDirectoryWatch(path));
}

void 
FileNodesManager::onException(const QString &description, const QString &path)
{
    QDUMP2(description, path);
    emit(exception(description, path));
}

void 
FileNodesManager::onStatusMessage(const QString &message)
{
    QDUMP(message);
    emit(statusMessage(message));
}


void 
FileNodesManager::transferToSyncMate(const QString &filenode, const QString &filenodepath, const QString &relativefilepath)
{
}

void 
FileNodesManager::transferFromSyncMate(const QString &filenode, const QString &filenodepath, const QString &relativefilepath)
{
}

