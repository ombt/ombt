#include "linuxfilesystemwatcher.h"

#if 0
#include "../UtilityServices/applicationexception.h"
#include "../UtilityServices/logger.h"
#endif

#include "DASQDebug.h"

FileWatcher::FileWatcher():
    inotifyfd_(-1)
{
    QTRACE();
    inotifyfd_ = inotify_init();
    QDUMP(inotifyfd_);
}

FileWatcher::~FileWatcher()
{
    QTRACE();
    if (inotifyfd_ != -1)
    {
        QTRACE();
        ::close(inotifyfd_);
        inotifyfd_= -1;
    }
    QTRACE();
}

void
FileWatcher::onDataReceived()
{
    QTRACE();
    unsigned char buf[10*1024];
    int count = ::read(inotifyfd_, buf, 10*1024);
    if (count <= 0) return;
    inotify_event *pevent = (inotify_event *)buf;
    for (int offset = 0; 
         offset<count;
         offset += offset + sizeof(inotify_event) + pevent->len)
    {
         QMutexLocker lock(&mutex_);
         ID2WP::const_iterator id2wpit = id2watchpoint_.find(pevent->wd);
         if (id2wpit == id2watchpoint_.end())
         {
             // unknown id.
             if (pevent->wd != -1)
             {
                 // qDebug() << "Unknown watch descriptor:" << pevent->wd;
                 continue;
             }
             // check for queue overflow if descriptor is -1.
             if ((pevent->mask & IN_Q_OVERFLOW) != 0)
             {
                 QTRACE();
                 emit(exception("Event Queue Overflow", ""));
             }
             continue;
         }
         QString watchpointpath = id2wpit.value();
         QString fullpath = watchpointpath;
         if (pevent->len > 0)
             fullpath += QString("/") + QString(pevent->name);
         uint32_t mask = pevent->mask;

         qDebug() << "Watch descriptor:" << pevent->wd;
         qDebug() << "Event Path      :" << watchpointpath;
         qDebug() << "Event Mask (hex):" << QString().setNum(mask, 16);
         qDebug() << "Event Mask (bin):" << QString().setNum(mask, 2);
         qDebug() << "Event Cookie    :" << pevent->cookie;
         qDebug() << "Event Length    :" << pevent->len;
         qDebug() << "Full Path       :" << fullpath;

         if ((mask & IN_ISDIR) != 0)
         {
             QTRACE();
             // we have a directory
             if ((mask & IN_IGNORED) != 0)
             {
                 // nothing to do
                 QTRACE();
                 continue;
             }
             else if ((mask & IN_CREATE) != 0)
             {
                 QTRACE();
                 emit(directoryCreated(fullpath));
             }
             else if ((mask & IN_DELETE) != 0)
             {
                 QTRACE();
                 emit(directoryDeleted(fullpath));
             }
             else if ((mask & IN_DELETE_SELF) != 0)
             {
                 QTRACE();
                 emit(directoryDeleted(fullpath));
             }
             else if ((mask & IN_ATTRIB) != 0)
             {
                 QTRACE();
                 emit(directoryChanged(fullpath));
             }
             else if ((mask & IN_MODIFY) != 0)
             {
                 QTRACE();
                 emit(directoryChanged(fullpath));
             }
             else if ((mask & IN_UNMOUNT) != 0)
             {
                 QTRACE();
                 emit(exception("Watched directory unmounted", fullpath));
             }
         }
         else
         {
             // we have a file
             if ((mask & IN_IGNORED) != 0)
             {
                 // nothing to do
                 QTRACE();
                 continue;
             }
             else if ((mask & IN_CREATE) != 0)
             {
                 QTRACE();
                 emit(fileCreated(fullpath));
             }
             else if ((mask & IN_DELETE) != 0)
             {
                 QTRACE();
                 emit(fileDeleted(fullpath));
             }
             else if ((mask & IN_DELETE_SELF) != 0)
             {
                 QTRACE();
                 emit(fileDeleted(fullpath));
             }
             else if ((mask & IN_ATTRIB) != 0)
             {
                 QTRACE();
                 emit(fileChanged(fullpath));
             }
             else if ((mask & IN_MODIFY) != 0)
             {
                 QTRACE();
                 emit(fileChanged(fullpath));
             }
         }
    }
}

void
FileWatcher::addPath(const QString &path)
{
    QDUMP(path);
    if (QDir(path).exists())
    {
        QTRACE();
        addDirectoryWatcher(path,
            IN_ATTRIB|IN_CREATE|IN_DELETE|IN_DELETE_SELF|
            IN_MODIFY|IN_MOVE_SELF|IN_MOVED_FROM|IN_MOVED_TO);
        QTRACE();
    }
    else if (QFile(path).exists())
    {
        QTRACE();
        addFileWatcher(path,
            IN_ATTRIB|IN_CREATE|IN_DELETE|IN_DELETE_SELF|
            IN_MODIFY|IN_MOVE_SELF|IN_MOVED_FROM|IN_MOVED_TO);
        QTRACE();
    }
    QTRACE();
}

void
FileWatcher::removePath(const QString &path)
{
    QDUMP(path);
    if (QDir(path).exists())
    {
        QTRACE();
        removeDirectoryWatcher(path);
        QTRACE();
    }
    else if (QFile(path).exists())
    {
        QTRACE();
        removeFileWatcher(path);
        QTRACE();
    }
}

void
FileWatcher::addFileWatcher(const QString &path, unsigned long mask)
{
    QTRACE();
    QDUMP(path);
    int id = inotify_add_watch(inotifyfd_, path.toStdString().c_str(), mask);
    QDUMP(id);
    QMutexLocker lock(&mutex_);
    watchpoint2id_[path] = id;
    id2watchpoint_[id] = path;
}

void
FileWatcher::removeFileWatcher(const QString &path)
{
    QTRACE();
    QDUMP(path);
    QMutexLocker lock(&mutex_);
    WP2ID::const_iterator wp2idit = watchpoint2id_.find(path);
    if (wp2idit != watchpoint2id_.end())
    {
        QTRACE();
        inotify_rm_watch(inotifyfd_, wp2idit.value());
        watchpoints_.remove(path);
        watchpoint2id_.remove(path);
        id2watchpoint_.remove(wp2idit.value());
    }
    QTRACE();
}

void
FileWatcher::addDirectoryWatcher(const QString &path, unsigned long mask)
{
    QTRACE();
    QDUMP(path);
    QMutexLocker lock(&mutex_);
    int id = inotify_add_watch(inotifyfd_, path.toStdString().c_str(), mask);
    QDUMP(id);
    watchpoint2id_[path] = id;
    id2watchpoint_[id] = path;
}

void
FileWatcher::removeDirectoryWatcher(const QString &path)
{
    QTRACE();
    QDUMP(path);
    QMutexLocker lock(&mutex_);
    WP2ID::const_iterator wp2idit = watchpoint2id_.find(path);
    if (wp2idit != watchpoint2id_.end())
    {
        QTRACE();
        inotify_rm_watch(inotifyfd_, wp2idit.value());
        watchpoints_.remove(path);
        watchpoint2id_.remove(path);
        id2watchpoint_.remove(wp2idit.value());
    }
    QTRACE();
}

FileWatcherThread::FileWatcherThread(FileWatcher &fw):
    filewatcher_(fw)
{
    QTRACE();
    moveToThread(this);
    QTRACE();
}

FileWatcherThread::~FileWatcherThread()
{
    QTRACE();
}

void
FileWatcherThread::run()
{
    QTRACE();
    QSocketNotifier *psn = new QSocketNotifier(filewatcher_.inotifyfd_, QSocketNotifier::Read, this);
    QObject::connect(psn, SIGNAL(activated(int)), 
                     &filewatcher_, SLOT(onDataReceived()), 
                     Qt::DirectConnection);
    assert(psn->isEnabled());
    QTRACE();
    this->exec();
    QTRACE();
}
FileSystemWatcher::FileSystemWatcher(QObject *):
    _filewatcher(), _filewatcherthread(_filewatcher)
{
    _filewatcherthread.start();

    connect(&_filewatcher, SIGNAL(fileCreated(const QString&)), 
            this, SLOT(onFileCreated(const QString&)));
    connect(&_filewatcher, SIGNAL(directoryCreated(const QString&)), 
            this, SLOT(onDirectoryCreated(const QString&)));
    connect(&_filewatcher, SIGNAL(fileDeleted(const QString&)), 
            this, SLOT(onFileDeleted(const QString&)));
    connect(&_filewatcher, SIGNAL(directoryDeleted(const QString&)), 
            this, SLOT(onDirectoryDeleted(const QString&)));
    connect(&_filewatcher, SIGNAL(fileChanged(const QString&)), 
            this, SLOT(onFileChanged(const QString&)));
    connect(&_filewatcher, SIGNAL(directoryChanged(const QString&)), 
            this, SLOT(onDirectoryChanged(const QString&)));
    connect(&_filewatcher, SIGNAL(fileRenamed(const QString&, const QString&)), 
            this, SLOT(onFileRenamed(const QString&, const QString&)));
    connect(&_filewatcher, SIGNAL(directoryRenamed(const QString&, const QString&)), 
            this, SLOT(onDirectoryRenamed(const QString&, const QString&)));
    connect(&_filewatcher, SIGNAL(exception(const QString&, const QString&)), 
            this, SLOT(onException(const QString&, const QString&)));
}

FileSystemWatcher::~FileSystemWatcher()
{
    _filewatcherthread.quit();
    _filewatcherthread.wait();
}

void FileSystemWatcher::addDirectoryWatcher(const QString &path)
{
    try
    {
        QDUMP(path);
        if (! isValidPath(path))
        {
            QTRACE();
            return;
        }

        this->beginRecursiveDirectoryWatch(path);
    }
    catch(const std::exception& ex )
    {
        qCritical() << ex.what();
    }
}

void FileSystemWatcher::endDirectoryWatcher(const QString &path)
{
    try
    {
        QDUMP(path);
        if (! isValidPath(path))
        {
            QTRACE();
            return;
        }

        this->endRecursiveDirectoryWatch(path);
    }
    catch(const std::exception& ex )
    {
        qCritical() << ex.what();
    }
}

void FileSystemWatcher::addDirectoryWatch(const QString & path)
{
    try
    {
        QDUMP(path);
        QDir dir(path);
        _filewatcher.addPath(path);
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
        QDUMP(path);
        if (! isValidPath(path))
        {
            QTRACE();
            return;
        }
        _filewatcher.removePath(path);
    }
    catch(const std::exception& ex )
    {
        qCritical() << ex.what();
    }
}

void FileSystemWatcher::beginRecursiveDirectoryWatch(const QString & path)
{
    QDUMP(path);
    if (! isValidPath(path))
    {
        QTRACE();
        return;
    }

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
    QDUMP(path);
    if (! isValidPath(path))
    {
        QTRACE();
        return;
    }
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
        QDUMP(path);
        if (! isValidPath(path))
        {
            QTRACE();
            return;
        }

        emit(directoryChanged(path));

        this->endRecursiveDirectoryWatch(path);
        this->beginRecursiveDirectoryWatch(path);
    }
    catch(const std::exception& ex )
    {
        qCritical() << ex.what();
    }
}

void FileSystemWatcher::onFileCreated(const QString &path)
{
    QDUMP(path);
    emit(fileCreated(path));
}

void FileSystemWatcher::onDirectoryCreated(const QString &path)
{
    try
    {
        QDUMP(path);
        if (! isValidPath(path))
        {
            QTRACE();
            return;
        }

        emit(directoryCreated(path));

        this->endRecursiveDirectoryWatch(path);
        this->beginRecursiveDirectoryWatch(path);
    }
    catch(const std::exception& ex )
    {
        qCritical() << ex.what();
    }
}

void FileSystemWatcher::onFileDeleted(const QString &path)
{
    QDUMP(path);
    emit(fileDeleted(path));
}

void FileSystemWatcher::onDirectoryDeleted(const QString &path)
{
    try
    {
        QDUMP(path);
        if (! isValidPath(path))
        {
            QTRACE();
            return;
        }

        emit(directoryDeleted(path));

        this->endRecursiveDirectoryWatch(path);
        this->beginRecursiveDirectoryWatch(path);
    }
    catch(const std::exception& ex )
    {
        qCritical() << ex.what();
    }
}

void FileSystemWatcher::onFileChanged(const QString &path)
{
    QDUMP(path);
    emit(fileChanged(path));
}

void FileSystemWatcher::onDirectoryChanged(const QString &path)
{
    try
    {
        QDUMP(path);
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

void FileSystemWatcher::onFileRenamed(const QString &oldpath, const QString &newpath)
{
    QDUMP2(oldpath, newpath);
    emit(fileRenamed(oldpath, newpath));
}

void FileSystemWatcher::onDirectoryRenamed(const QString &oldpath, const QString &newpath)
{
    QDUMP2(oldpath, newpath);
    emit(directoryRenamed(oldpath, newpath));
}

void FileSystemWatcher::onException(const QString & description, const QString &path)
{
    QDUMP2(description, path);
    emit(exception(description, path));
}

