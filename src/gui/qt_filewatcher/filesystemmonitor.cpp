#include "DASQDebug.h"
#include "filesystemmonitor.h"

#include <QtGlobal>
#include <QDebug>

FileSystemMonitor::FileSystemMonitor(DASDbMgr &dasDbMgr, DASSynchMgr &synchMgr, QObject *parent ) :
        QObject(parent), _dasDbMgr(dasDbMgr), _synchMgr(synchMgr)
{
    assignSignalsAndSlots();
}

FileSystemMonitor::~FileSystemMonitor()
{}

void FileSystemMonitor::assignSignalsAndSlots()
{
    connect(&_fileSystemWatcher, SIGNAL(fileCreated(const QString&)),
             this, SLOT(onFileCreated(const QString&)));
    connect(&_fileSystemWatcher, SIGNAL(directoryCreated(const QString&)),
             this, SLOT(onDirectoryCreated(const QString&)));
    connect(&_fileSystemWatcher, SIGNAL(fileDeleted(const QString&)),
             this, SLOT(onFileDeleted(const QString&)));
    connect(&_fileSystemWatcher, SIGNAL(directoryDeleted(const QString&)),
             this, SLOT(onDirectoryDeleted(const QString&)));
    connect(&_fileSystemWatcher, SIGNAL(fileChanged(const QString&)),
             this, SLOT(onFileChanged(const QString&)));
    connect(&_fileSystemWatcher, SIGNAL(directoryChanged(const QString&)),
             this, SLOT(onDirectoryChanged(const QString&)));
    connect(&_fileSystemWatcher, SIGNAL(fileRenamed(const QString&, const QString&)),
             this, SLOT(onFileRenamed(const QString&, const QString&)));
    connect(&_fileSystemWatcher, SIGNAL(directoryRenamed(const QString&, const QString&)),
             this, SLOT(onDirectoryRenamed(const QString&, const QString&)));
    connect(&_fileSystemWatcher, SIGNAL(exception(const QString&, const QString&)),
             this, SLOT(onException(const QString&, const QString&)));
}

void FileSystemMonitor::onRemoveDirectoryWatch(const QString &filenode, const QString & path)
{
    QDUMP2(filenode, path);
    _synchMgr.enqueue(QString("%1;%2;%3").arg(DASSynchMgr::RemoveDirectoryWatch).arg(filenode).arg(path));
    _fileSystemWatcher.endDirectoryWatcher(path);
}

void FileSystemMonitor::onAddDirectoryWatch(const QString &filenode, const QString & path)
{
    QDUMP2(filenode, path);
    _synchMgr.enqueue(QString("%1;%2;%3").arg(DASSynchMgr::AddDirectoryWatch).arg(filenode).arg(path));
    _fileSystemWatcher.addDirectoryWatcher(path);
}

void FileSystemMonitor::onFileCreated(const QString & path)
{
    QDUMP(path);
    _synchMgr.enqueue(QString("%1;%2").arg(DASSynchMgr::FileCreated).arg(path));
    markPathAsDirty(path);
    emit(fileCreated(path));
}

void FileSystemMonitor::onDirectoryCreated(const QString & path)
{
    QDUMP(path);
    _synchMgr.enqueue(QString("%1;%2").arg(DASSynchMgr::DirectoryCreated).arg(path));
    markPathAsDirty(path);
    emit(directoryCreated(path));
}

void FileSystemMonitor::onFileDeleted(const QString & path)
{
    QDUMP(path);
    _synchMgr.enqueue(QString("%1;%2").arg(DASSynchMgr::FileDeleted).arg(path));
    markPathAsDirty(path);
    emit(fileDeleted(path));
}

void FileSystemMonitor::onDirectoryDeleted(const QString & path)
{
    QDUMP(path);
    _synchMgr.enqueue(QString("%1;%2").arg(DASSynchMgr::DirectoryDeleted).arg(path));
    markPathAsDirty(path);
    emit(directoryDeleted(path));
}

void FileSystemMonitor::onFileChanged(const QString& path)
{
    QDUMP(path);
    _synchMgr.enqueue(QString("%1;%2").arg(DASSynchMgr::FileChanged).arg(path));
    markPathAsDirty(path);
    emit(fileChanged(path));
}

void FileSystemMonitor::onDirectoryChanged(const QString& path)
{
    QDUMP(path);
    _synchMgr.enqueue(QString("%1;%2").arg(DASSynchMgr::DirectoryChanged).arg(path));
    markPathAsDirty(path);
    emit(directoryChanged(path));
}

void FileSystemMonitor::onFileRenamed(const QString & oldPath,const QString & newPath)
{
    QDUMP2(oldPath, newPath);
    _synchMgr.enqueue(QString("%1;%2").arg(DASSynchMgr::FileRenamed).arg(oldPath).arg(newPath));
    markPathAsDirty(oldPath);
    emit(fileRenamed(oldPath, newPath));
}

void FileSystemMonitor::onDirectoryRenamed(const QString & oldPath, const QString & newPath)
{
    QDUMP2(oldPath, newPath);
    _synchMgr.enqueue(QString("%1;%2").arg(DASSynchMgr::DirectoryRenamed).arg(oldPath).arg(newPath));
    markPathAsDirty(oldPath);
    emit(directoryRenamed(oldPath, newPath));
}

void FileSystemMonitor::onException(const QString & description, const QString & path)
{
    QDUMP2(description, path);
    _synchMgr.enqueue(QString("%1;%2").arg(DASSynchMgr::Exception).arg(description).arg(path));
    emit(exception(description, path));
}

// internal utilities
void FileSystemMonitor::markPathAsDirty(const QString &path)
{
    QCRITDUMP(path);

    DASDbTransaction transaction(_dasDbMgr, true);

    bool found;
    DASDbSchema::FileMetaHashData mdata;
    _dasDbMgr.readFileMetaHashNoLock(path, mdata, found);
    if (!found)
    {
        QCRITTRACE();
        return;
    }
    QCRITTRACE();

    QStringList pathparts = path.split(QDir::separator(), 
                                       QString::KeepEmptyParts, 
                                       Qt::CaseSensitive);
    QString filenode = mdata.filenode_;
    do {
        QCRITDUMP(filenode);
        pathparts.pop_back();
        QCRITDUMP(pathparts.join(QDir::separator()));
    } while (filenode != pathparts.join(QDir::separator()));
    QCRITTRACE();
}
