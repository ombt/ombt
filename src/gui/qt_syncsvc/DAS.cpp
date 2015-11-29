#include "DAS.h"

// ctors and dtor
DAS::DAS(const QString &name, QObject *parent):
    QObject(parent),
    name_(name),
    syncmgr_(),
    syncmgrthread_(syncmgr_),
    filesystemmonitor_(),
    filenodemgr_(syncmgr_)
{
    assignSignalsAndSlots();
    syncmgrthread_.start();
}

DAS::~DAS()
{
    syncmgr_.done();
    syncmgrthread_.wait();
}

// set up connections
void DAS::assignSignalsAndSlots()
{
    // das <<==>> file system monitor
    connect(&filesystemmonitor_, SIGNAL(statusMessage(const QString &)),
            this, SLOT(onStatusMessage(const QString &)));

    // das <<==>> synch manager 
    connect(&syncmgr_, SIGNAL(statusMessage(const QString &)),
            this, SLOT(onStatusMessage(const QString &)));

    // das <<==>> file node manager 
    connect(&filenodemgr_, SIGNAL(statusMessage(const QString &)),
            this, SLOT(onStatusMessage(const QString &)));
    connect(this, SIGNAL(addDirectoryWatch(const QString &, const QString &)),
            &filenodemgr_, SLOT(onAddDirectoryWatch(const QString &, const QString &)));
    connect(this, SIGNAL(removeDirectoryWatch(const QString &, const QString &)),
            &filenodemgr_, SLOT(onRemoveDirectoryWatch(const QString &, const QString &)));
    connect(&filenodemgr_, SIGNAL(fileCreated(const QString &)),
            this, SLOT(onFileCreated(const QString &)));
    connect(&filenodemgr_, SIGNAL(directoryCreated(const QString &)),
            this, SLOT(onDirectoryCreated(const QString &)));
    connect(&filenodemgr_, SIGNAL(fileDeleted(const QString &)),
            this, SLOT(onFileDeleted(const QString &)));
    connect(&filenodemgr_, SIGNAL(directoryDeleted(const QString &)),
            this, SLOT(onDirectoryDeleted(const QString &)));
    connect(&filenodemgr_, SIGNAL(fileChanged(const QString &)),
            this, SLOT(onFileChanged(const QString &)));
    connect(&filenodemgr_, SIGNAL(directoryChanged(const QString &)),
            this, SLOT(onDirectoryChanged(const QString &)));
    connect(&filenodemgr_, SIGNAL(fileRenamed(const QString &, const QString &)),
            this, SLOT(onFileRenamed(const QString &, const QString &)));
    connect(&filenodemgr_, SIGNAL(directoryRenamed(const QString &, const QString &)),
            this, SLOT(onDirectoryRenamed(const QString &, const QString &)));
    connect(&filenodemgr_, 
            SIGNAL(fileNodeChanged(const QString &, const QString &, 
                                   const QString &, const QStringList &, 
                                   const QString &)),
            this, 
            SLOT(onFileNodeChanged(const QString &, const QString &, 
                                   const QString &, const QStringList &, 
                                   const QString &)));

    // file node manager <<==>> file system monitor
    connect(&filenodemgr_, SIGNAL(addDirectoryWatch(const QString &)),
            &filesystemmonitor_, SLOT(onAddDirectoryWatch(const QString &)));
    connect(&filenodemgr_, SIGNAL(removeDirectoryWatch(const QString &)),
            &filesystemmonitor_, SLOT(onRemoveDirectoryWatch(const QString &)));

    connect(&filesystemmonitor_, SIGNAL(fileCreated(const QString &)),
            &filenodemgr_, SLOT(onFileCreated(const QString &)));
    connect(&filesystemmonitor_, SIGNAL(directoryCreated(const QString &)),
            &filenodemgr_, SLOT(onDirectoryCreated(const QString &)));
    connect(&filesystemmonitor_, SIGNAL(fileDeleted(const QString &)),
            &filenodemgr_, SLOT(onFileDeleted(const QString &)));
    connect(&filesystemmonitor_, SIGNAL(directoryDeleted(const QString &)),
            &filenodemgr_, SLOT(onDirectoryDeleted(const QString &)));
    connect(&filesystemmonitor_, SIGNAL(fileChanged(const QString &)),
            &filenodemgr_, SLOT(onFileChanged(const QString &)));
    connect(&filesystemmonitor_, SIGNAL(directoryChanged(const QString &)),
            &filenodemgr_, SLOT(onDirectoryChanged(const QString &)));
    connect(&filesystemmonitor_, SIGNAL(fileRenamed(const QString &, const QString &)),
            &filenodemgr_, SLOT(onFileRenamed(const QString &, const QString &)));
    connect(&filesystemmonitor_, SIGNAL(directoryRenamed(const QString &, const QString &)),
            &filenodemgr_, SLOT(onDirectoryRenamed(const QString &, const QString &)));
}

// callbacks
void DAS::onAddDirectoryWatch(const QString &filenode, const QString &path)
{
    // progress message for window
    emit(statusMessage(QString("Adding File node %1 for path %2").arg(filenode).arg(path)));

    // forward operation to file node manager
    emit(addDirectoryWatch(filenode, path));
}

void DAS::onRemoveDirectoryWatch(const QString &filenode, const QString &path)
{
    // send message to window for display
    emit(statusMessage(QString("Removing File node %1 for path %2").arg(filenode).arg(path)));

    // forward operation to file node manager
    emit(removeDirectoryWatch(filenode, path));
}

void DAS::onStatusMessage(const QString &message)
{
    emit(statusMessage(message));
}

void 
DAS::onFileCreated( const QString & path )
{
    QDUMP(path);
    emit(statusMessage(QString("%1: %2").arg(__FUNCTION__).arg(path)));
}

void 
DAS::onDirectoryCreated( const QString & path )
{
    QDUMP(path);
    emit(statusMessage(QString("%1: %2").arg(__FUNCTION__).arg(path)));
}

void 
DAS::onFileDeleted( const QString & path )
{
    QDUMP(path);
    emit(statusMessage(QString("%1: %2").arg(__FUNCTION__).arg(path)));
}

void 
DAS::onDirectoryDeleted( const QString & path )
{
    QDUMP(path);
    emit(statusMessage(QString("%1: %2").arg(__FUNCTION__).arg(path)));
}

void 
DAS::onFileChanged( const QString & path )
{
    QDUMP(path);
    emit(statusMessage(QString("%1: %2").arg(__FUNCTION__).arg(path)));
}

void 
DAS::onDirectoryChanged( const QString & path )
{
    QDUMP(path);
    emit(statusMessage(QString("%1: %2").arg(__FUNCTION__).arg(path)));
}

void 
DAS::onFileRenamed( const QString & oldPath, const QString & newPath )
{
    QDUMP2(oldPath, newPath);
    emit(statusMessage(QString("%1: %2 to %3").arg(__FUNCTION__).arg(oldPath).arg(newPath)));
}

void 
DAS::onDirectoryRenamed(const QString & oldPath, const QString & newPath )
{
    QDUMP2(oldPath, newPath);
    emit(statusMessage(QString("%1: %2 to %3").arg(__FUNCTION__).arg(oldPath).arg(newPath)));
}

void 
DAS::onException(const QString &description, const QString &path)
{
    QDUMP2(description, path);
    emit(exception(description, path));
}

void 
DAS::onFileNodeChanged(const QString &filenode, const QString &filenodepath, 
                       const QString &relativefilepath, const QStringList &hashValues, 
                       const QString &transactionLog)
{
    QDUMP2(filenode, filenodepath);
    QDUMP(relativefilepath);
    emit(fileNodeChanged(filenode, filenodepath, relativefilepath, hashValues, transactionLog));
}
