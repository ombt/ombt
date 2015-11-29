#include "DAS.h"

// ctors and dtor
DAS::DAS():
    QObject(), 
    filenode_(), 
    synchmsgqueue_(), 
    dbmgr_(), 
    filesystemwatcher_(dbmgr_), 
    filesystemmonitor_(dbmgr_, synchmsgqueue_),
    filenodemgr_(dbmgr_), 
    synchmgr_(dbmgr_, synchmsgqueue_)
{
    assignSignalsAndSlots();
}

DAS::~DAS()
{
}

// set up connections
void DAS::assignSignalsAndSlots()
{
    // das <<==>> db manager 
    connect(&dbmgr_, SIGNAL(statusMessage(const QString &)),
            this, SLOT(onStatusMessage(const QString &)));

    // das <<==>> file system monitor
    connect(&filesystemmonitor_, SIGNAL(statusMessage(const QString &)),
            this, SLOT(onStatusMessage(const QString &)));

    // das <<==>> file system watcher 
    connect(&filesystemwatcher_, SIGNAL(statusMessage(const QString &)),
            this, SLOT(onStatusMessage(const QString &)));

    // das <<==>> synch manager 
    connect(&synchmgr_, SIGNAL(statusMessage(const QString &)),
            this, SLOT(onStatusMessage(const QString &)));

    // das <<==>> file node manager 
    connect(&filenodemgr_, SIGNAL(statusMessage(const QString &)),
            this, SLOT(onStatusMessage(const QString &)));
    connect(this, SIGNAL(addDirectoryWatch(const QString &, const QString &)),
            &filenodemgr_, SLOT(onAddDirectoryWatch(const QString &, const QString &)));
    connect(this, SIGNAL(removeDirectoryWatch(const QString &, const QString &)),
            &filenodemgr_, SLOT(onRemoveDirectoryWatch(const QString &, const QString &)));

    // file node manager <<==>> file system monitor
    connect(&filenodemgr_, SIGNAL(addDirectoryWatch(const QString &, const QString &)),
            &filesystemmonitor_, SLOT(onAddDirectoryWatch(const QString &, const QString &)));
    connect(&filenodemgr_, SIGNAL(removeDirectoryWatch(const QString &, const QString &)),
            &filesystemmonitor_, SLOT(onRemoveDirectoryWatch(const QString &, const QString &)));
    connect(&filesystemmonitor_, SIGNAL(directoryDeleted(const QString &)),
            &filenodemgr_, SLOT(onDirectoryDeleted(const QString &)));
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

