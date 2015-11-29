#ifndef __DAS_H
#define __DAS_H

#include <QObject>

#include "filesystemmonitor.h"
#include "syncmanagement.h"
#include "testfilenodesmanager.h"

class DAS: public QObject
{
    Q_OBJECT

signals:
    void statusMessage(const QString &message);
    void addDirectoryWatch(const QString &filenode, const QString &path);
    void removeDirectoryWatch(const QString &filenode, const QString &path);
    void exception(const QString &description, const QString &path);
    void fileNodeChanged(const QString &filenode, const QString &filenodepath, 
                         const QString &relativefilepath, const QStringList &hashValues, 
                         const QString &transactionLog ) ;

public:
    explicit DAS(const QString &name, QObject *parent = NULL);
    virtual ~DAS();

    QString getName() const { return(name_); }

public slots:
    void onAddDirectoryWatch(const QString &filenode, const QString &path);
    void onRemoveDirectoryWatch(const QString &filenode, const QString &path);

    void onFileCreated(const QString &path);
    void onDirectoryCreated(const QString &path);
    void onFileDeleted(const QString &path);
    void onDirectoryDeleted(const QString &path);
    void onFileChanged(const QString &path);
    void onDirectoryChanged(const QString &path);
    void onFileRenamed(const QString &oldpath, const QString &newpath);
    void onDirectoryRenamed(const QString &oldpath, const QString &newpath);

    void onFileNodeChanged(const QString &filenode, const QString &filenodepath, 
                           const QString &relativefilepath, const QStringList &hashValues, 
                           const QString &transactionLog);

    void onException(const QString &description, const QString &path);
    void onStatusMessage(const QString &message);

private:
    void assignSignalsAndSlots();

protected:
    const QString name_;
    SyncManagement syncmgr_;
    SyncManagementThread syncmgrthread_;
    FileSystemMonitor filesystemmonitor_;
    FileNodesManager filenodemgr_;
};

#endif
