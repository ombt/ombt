#ifndef FILENODESMANAGER_H
#define FILENODESMANAGER_H

#include <QObject>

#include "DASQDebug.h"
#include "dataaccessmanagement.h"
#include "transactionlog.h"
#include "syncmanagement.h"

class FileNodesManager : public QObject
{
    Q_OBJECT
signals:
    void addDirectoryWatch(const QString& path);
    void removeDirectoryWatch(const QString& path);
    void fileNodeChanged( const QString& filenode, const QString &filenodepath, const QString &relativefilepath, const QStringList& hashValues, const QString& transactionLog ) ;
    void statusMessage(const QString &message);
    void exception(const QString &description, const QString &path);
    void fileCreated(const QString & path);
    void directoryCreated(const QString & path);
    void fileDeleted(const QString & path);
    void directoryDeleted(const QString & path);
    void fileChanged(const QString & path);
    void directoryChanged(const QString & path);
    void fileRenamed(const QString & oldPath,const QString & newPath);
    void directoryRenamed(const QString & oldPath, const QString & newPath);

public:
    enum HashTypes
    {
        Actual,
        Target,
        PrevActual,
        PrevTarget
    };

public:
    explicit FileNodesManager(SyncManagement &syngmgr, QObject *parent = 0);
    virtual ~FileNodesManager();

    void getFileNodePath(const QString& fileNode, QString& path) ;
    void getFileNodeName(QString& fileNode, const QString& path) ;
    void getFileDescriptionsAsXml(bool recurse, const QString& rootPath, QByteArray* formatterOutput) ;
    void requestFilesMetadata(bool recurse, const QString filenode, const QString& relativePath) ;
    void transferToSyncMate(const QString &filenode, const QString &filenodepath, const QString &relativefilepath);
    void transferFromSyncMate(const QString &filenode, const QString &filenodepath, const QString &relativefilepath);

private slots:
    void onServiceAdded(const QString& serviceType, const QString& serviceName);
    void onServiceRemoved(const QString& serviceType, const QString& serviceName) ;
    void onFilenodeAdded( const QString& nodeName, const QString& path);
    void onFilenodeRemoved(const QString& nodeName,const QString& path);
    void onAttachFilenodeToService(const QString& serviceType,const QString& serviceName, const QString& nodeName);
    void onDetachFilenodeFromService(const QString& serviceType,const QString& serviceName, const QString& nodeName , const QString& path);
    //void onServiceTableUpdated(const TableRows& tableRows);

    void onFileCreated( const QString & path );
    void onDirectoryCreated( const QString & path );
    void onFileDeleted( const QString & path );
    void onDirectoryDeleted( const QString & path );
    void onFileChanged( const QString & path );
    void onDirectoryChanged( const QString & path );
    void onFileRenamed( const QString & oldPath, const QString & newPath );
    void onDirectoryRenamed(const QString & oldPath, const QString & newPath );

    void onException(const QString &description, const QString &path);
    void onStatusMessage(const QString &message);

    void onAddDirectoryWatch(const QString &filenode, const QString &path);
    void onRemoveDirectoryWatch(const QString &filenode, const QString &path) ;

private:
    bool doFileNodeChanged(const QString & path);

private:
    SyncManagement &syncmgr_;
    DataAccessManagement *pdatamgr_;
    TransactionLog *ptranslog_;
};

#endif // FILENODESMANAGER_H
