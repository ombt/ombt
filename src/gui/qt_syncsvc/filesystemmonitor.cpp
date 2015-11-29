#include "filesystemmonitor.h"
#include "DASQDebug.h"

//#include "../UtilityServices/logger.h"

FileSystemMonitor::FileSystemMonitor(QObject *parent ) :
        QObject(parent)
{
    assignSignalsAndSlots();
}

FileSystemMonitor::~FileSystemMonitor()
{}

void FileSystemMonitor::assignSignalsAndSlots()
{
    connect(&_fileSystemWatcher, SIGNAL(fileCreated(const QString&)),this, SLOT(onFileCreated(const QString&)));
    connect(&_fileSystemWatcher, SIGNAL(directoryCreated(const QString&)),this, SLOT(onDirectoryCreated(const QString&)));
    connect(&_fileSystemWatcher, SIGNAL(fileDeleted(const QString&)),this, SLOT(onFileDeleted(const QString&)));
    connect(&_fileSystemWatcher, SIGNAL(directoryDeleted(const QString&)),this, SLOT(onDirectoryDeleted(const QString&)));
    connect(&_fileSystemWatcher, SIGNAL(fileChanged(const QString&)),this, SLOT(onFileChanged(const QString&)));
    connect(&_fileSystemWatcher, SIGNAL(directoryChanged(const QString&)),this, SLOT(onDirectoryChanged(const QString&)));
    connect(&_fileSystemWatcher, SIGNAL(fileRenamed(const QString&, const QString&)),this, SLOT(onFileRenamed(const QString&, const QString&)));
    connect(&_fileSystemWatcher, SIGNAL(directoryRenamed(const QString&, const QString&)),this, SLOT(onDirectoryRenamed(const QString&, const QString&)));
}

void FileSystemMonitor::onRemoveDirectoryWatch(const QString & path)
{
    _fileSystemWatcher.endDirectoryWatcher(path);
}

void FileSystemMonitor::onAddDirectoryWatch(const QString & path)
{
    _fileSystemWatcher.addDirectoryWatcher(path);
}

void FileSystemMonitor::onFileCreated(const QString & path)
{
    emit(fileCreated( path));
}

void FileSystemMonitor::onDirectoryCreated(const QString & path)
{
    emit(directoryCreated( path));
}

void FileSystemMonitor::onFileDeleted(const QString & path)
{
    emit(fileDeleted( path));
}

void FileSystemMonitor::onDirectoryDeleted(const QString & path)
{
    emit(directoryDeleted( path));
}

void FileSystemMonitor::onFileChanged(const QString& path)
{
    emit(fileChanged( path));
}

void FileSystemMonitor::onDirectoryChanged(const QString& path)
{
    emit(directoryChanged( path));
}

void FileSystemMonitor::onFileRenamed(const QString & oldPath,const QString & newPath)
{
    emit(fileRenamed( oldPath, newPath));
}

void FileSystemMonitor::onDirectoryRenamed(const QString & oldPath, const QString & newPath)
{
    emit(directoryRenamed( oldPath, newPath));
}
