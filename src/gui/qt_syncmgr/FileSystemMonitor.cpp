
#include "FileSystemMonitor.h"

// ctors and dtor
FileSystemMonitor::FileSystemMonitor(DBMgr &dbmgr, MsgQueue &synchmsgqueue):
    QObject(), 
    dbmgr_(dbmgr), synchmsgqueue_(synchmsgqueue), 
    filesystemwatcher_(dbmgr_)
{
    assignSignalsAndSlots();
}

FileSystemMonitor::~FileSystemMonitor()
{
}

// set up connections
void FileSystemMonitor::assignSignalsAndSlots()
{
    // file system monitor <<==>> file system watcher
    connect(&filesystemwatcher_, SIGNAL(fileCreated(const QString&)),
            this, SLOT(onFileCreated(const QString&)));
    connect(&filesystemwatcher_, SIGNAL(directoryCreated(const QString&)),
            this, SLOT(onDirectoryCreated(const QString&)));
    connect(&filesystemwatcher_, SIGNAL(fileDeleted(const QString&)),
            this, SLOT(onFileDeleted(const QString&)));
    connect(&filesystemwatcher_, SIGNAL(directoryDeleted(const QString&)),
            this, SLOT(onDirectoryDeleted(const QString&)));
    connect(&filesystemwatcher_, SIGNAL(fileChanged(const QString&)),
            this, SLOT(onFileChanged(const QString&)));
    connect(&filesystemwatcher_, SIGNAL(directoryChanged(const QString&)),
            this, SLOT(onDirectoryChanged(const QString&)));
    connect(&filesystemwatcher_, SIGNAL(fileRenamed(const QString&, const QString&)),
            this, SLOT(onFileRenamed(const QString&, const QString&)));
    connect(&filesystemwatcher_, SIGNAL(directoryRenamed(const QString&, const QString&)),
            this, SLOT(onDirectoryRenamed(const QString&, const QString&)));
    connect(&filesystemwatcher_, SIGNAL(exception(const QString&, const QString&)),
            this, SLOT(onException(const QString&, const QString&)));
}

// call backs for signals from file node manager
void 
FileSystemMonitor::onAddDirectoryWatch(const QString &filenode, const QString &path)
{
    QDUMP2(filenode, path);
    filesystemwatcher_.addDirectoryWatcher(path);
}

void 
FileSystemMonitor::onRemoveDirectoryWatch(const QString &filenode, const QString &path)
{
    QDUMP2(filenode, path);
    filesystemwatcher_.endDirectoryWatcher(path);
}

// callbacks for signals from file watcher
void 
FileSystemMonitor::onStatusMessage(const QString &message)
{
    QDUMP(message);
    emit(statusMessage(message));
}

void 
FileSystemMonitor::onFileCreated(const QString &path)
{
    QDUMP(path);
}

void 
FileSystemMonitor::onDirectoryCreated(const QString &path)
{
    QDUMP(path);
}

void 
FileSystemMonitor::onFileDeleted(const QString &path)
{
    QDUMP(path);
}

void 
FileSystemMonitor::onDirectoryDeleted(const QString &path)
{
    // report all directory deletes to file node manager
    QDUMP(path);
    emit(directoryDeleted(path));
}

void 
FileSystemMonitor::onFileChanged(const QString &path)
{
    QDUMP(path);
}

void 
FileSystemMonitor::onDirectoryChanged(const QString &path)
{
    QDUMP(path);
}

void 
FileSystemMonitor::onFileRenamed(const QString &oldpath, const QString &newpath)
{
    QDUMP2(oldpath, newpath);
}

void 
FileSystemMonitor::onDirectoryRenamed(const QString &oldpath, const QString &newpath)
{
    // report all directory renames to file node manager
    QDUMP2(oldpath, newpath);
    emit(directoryRenamed(oldpath, newpath));
}

void 
FileSystemMonitor::onException(const QString &description, const QString &path)
{
    QDUMP2(description, path);
}

