#include "filesystemwatcher.h"
#if 0
#include "../UtilityServices/applicationexception.h"
#include "../UtilityServices/logger.h"
#endif
#include <QDebug>
#include <QDir>

FileSystemWatcher::FileSystemWatcher(QObject *)
{
    connect(&_watcher, 
            SIGNAL(directoryChanged(const QString&)), 
            this, 
            SLOT(onWatcherDirectoryChanged(const QString&)));
}

FileSystemWatcher::~FileSystemWatcher(){}

void FileSystemWatcher::addDirectoryWatch(const QString & path)
{
    try
    {
        QDir  dir(path);
        _watcher.addPath(dir.absolutePath());
    }
    catch(const std::exception& ex )
    {
        qCritical() << ex.what();
    }
}

void FileSystemWatcher::endDirectoryWatch(const QString & path)
{
    try
    {
        if (! isValidPath(path))
            return;
        _watcher.removePath(path);
    }
    catch(const std::exception& ex )
    {
        qCritical() << ex.what();
    }
}

void FileSystemWatcher::beginRecursiveDirectoryWatch(const QString & path)
{
    if (! isValidPath(path))
        return;

    addDirectoryWatch(path);
    QDir dir(path);

    QStringList list(dir.entryList(QDir::AllDirs | 
                                   QDir::NoDotAndDotDot |
                                   QDir::CaseSensitive));
    foreach(QString item, list)
    {
        QString subDir = 
            QString("%1%2%3").arg(path).arg(QDir::separator()).arg(item);
        beginRecursiveDirectoryWatch(subDir);
    }
}

void FileSystemWatcher::endRecursiveDirectoryWatch(const QString & path)
{
    if (! isValidPath(path))
        return;
    endDirectoryWatch(path);
    QDir dir(path);

    QStringList list(dir.entryList(QDir::AllDirs | 
                                   QDir::NoDotAndDotDot |
                                   QDir::CaseSensitive));
    foreach(QString item, list)
    {
        QString subDir = 
            QString("%1%2%3").arg(path).arg(QDir::separator()).arg(item);
        endRecursiveDirectoryWatch(subDir);
    }
}

void FileSystemWatcher::onWatcherDirectoryChanged(const QString & path)
{
    try
    {
        if (! isValidPath(path))
            return;

        emit(directoryChanged(path));

        this->endRecursiveDirectoryWatch(path);
        this->beginRecursiveDirectoryWatch(path);
    }
    catch(const std::exception& ex )
    {
        qCritical() << ex.what();
    }
}

bool FileSystemWatcher::isDirectoryWatched(const QString & path)
{
    if (! isValidPath(path))
        return false;
    QStringList list = _watcher.directories();
    return list.contains(path);
}

void FileSystemWatcher::dumpLists( )
{
    QStringList dirList = _watcher.directories();
    qDebug() << "****    Begin directories watched  list.  Item count is " << dirList.count();
    foreach(QString item, dirList)
    {
        qDebug() << item;
    }
    qDebug() << "****     End list     ****";
    qDebug() << "" ;

    QStringList fileList = _watcher.files();
    qDebug() << "****    Begin files watched  list.  Item count is " << fileList.count();
    foreach(QString item, fileList)
    {
        qDebug() << item;
    }
    qDebug() << "****     End list     ****";
    qDebug() << "" ;
}

