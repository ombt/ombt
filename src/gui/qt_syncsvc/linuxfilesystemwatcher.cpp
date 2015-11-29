#include "linuxfilesystemwatcher.h"

#include "DASQDebug.h"

// linux file watcher based on inotify()
FileWatcher::FileWatcher():
    inotifyfd_(-1)
{
    inotifyfd_ = inotify_init();
}

FileWatcher::~FileWatcher()
{
    if (inotifyfd_ != -1)
    {
        ::close(inotifyfd_);
        inotifyfd_= -1;
    }
}

void
FileWatcher::onDataReceived()
{
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
                 continue;
             }

             // check for queue overflow if descriptor is -1.
             if ((pevent->mask  & IN_Q_OVERFLOW) != 0)
             {
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

         if ((mask  & IN_ISDIR) != 0)
         {
             // we have a directory
             if ((mask & IN_IGNORED) != 0)
             {
                 // nothing to do
                 continue;
             }
             else if ((mask & IN_CREATE) != 0)
             {
                 emit(directoryCreated(fullpath));
             }
             else if ((mask & IN_DELETE) != 0)
             {
                 emit(directoryDeleted(fullpath));
             }
             else if ((mask & IN_DELETE_SELF) != 0)
             {
                 emit(directoryDeleted(fullpath));
             }
             else if ((mask & IN_MOVE_SELF) != 0)
             {
                 emit(directoryDeleted(fullpath));
             }
             else if ((mask & IN_ATTRIB) != 0)
             {
                 emit(directoryChanged(fullpath));
             }
             else if ((mask & IN_MODIFY) != 0)
             {
                 emit(directoryChanged(fullpath));
             }
             else if ((mask & IN_MOVED_FROM) != 0)
             {
                 // save old name for now.
                 cookie2oldpath_[pevent->cookie] = fullpath;
             }
             else if ((mask & IN_MOVED_TO) != 0)
             {
                 C2OP::iterator it = cookie2oldpath_.find(pevent->cookie);
                 C2OP::iterator itend = cookie2oldpath_.end();
                 if (it != itend)
                 {
                     // move within watched subdirectory
                     emit(directoryRenamed(it.value(), fullpath));
                     cookie2oldpath_.erase(it);
                 }
                 else
                 {
                     // move from outside the watched subdirectory
                     emit(directoryCreated(fullpath));
                 }
             }
             else if ((mask & IN_UNMOUNT) != 0)
             {
                 emit(exception("Watched directory unmounted", fullpath));
             }
         }
         else
         {
             // we have a file
             if ((mask & IN_IGNORED) != 0)
             {
                 // nothing to do
                 continue;
             }
             else if ((mask & IN_CREATE) != 0)
             {
                 emit(fileCreated(fullpath));
             }
             else if ((mask & IN_DELETE) != 0)
             {
                 emit(fileDeleted(fullpath));
             }
             else if ((mask & IN_DELETE_SELF) != 0)
             {
                 emit(fileDeleted(fullpath));
             }
             else if ((mask & IN_ATTRIB) != 0)
             {
                 emit(fileChanged(fullpath));
             }
             else if ((mask & IN_MODIFY) != 0)
             {
                 emit(fileChanged(fullpath));
             }
             else if ((mask & IN_MOVED_FROM) != 0)
             {
                 // save old name for now.
                 cookie2oldpath_[pevent->cookie] = fullpath;
             }
             else if ((mask & IN_MOVED_TO) != 0)
             {
                 C2OP::iterator it = cookie2oldpath_.find(pevent->cookie);
                 C2OP::iterator itend = cookie2oldpath_.end();
                 if (it != itend)
                 {
                     // move within watched subdirectory
                     emit(fileRenamed(it.value(), fullpath));
                     cookie2oldpath_.erase(it);
                 }
                 else
                 {
                     // move from outside the watched subdirectory
                     emit(fileCreated(fullpath));
                 }
             }
         }
    }
}

void
FileWatcher::addPath(const QString &path)
{
    if (QDir(path).exists())
    {
        addDirectoryWatcher(path,
            IN_ATTRIB|IN_CREATE|IN_DELETE|IN_DELETE_SELF|
            IN_MODIFY|IN_MOVE_SELF|IN_MOVED_FROM|IN_MOVED_TO);
    }
    else if (QFile(path).exists())
    {
        addFileWatcher(path,
            IN_ATTRIB|IN_CREATE|IN_DELETE|IN_DELETE_SELF|
            IN_MODIFY|IN_MOVE_SELF|IN_MOVED_FROM|IN_MOVED_TO);
    }
}

void
FileWatcher::removePath(const QString &path)
{
    if (QDir(path).exists())
    {
        removeDirectoryWatcher(path);
    }
    else if (QFile(path).exists())
    {
        removeFileWatcher(path);
    }
}

void
FileWatcher::addFileWatcher(const QString &path, unsigned long mask)
{
    int id = inotify_add_watch(inotifyfd_, path.toStdString().c_str(), mask);
    QMutexLocker lock(&mutex_);
    watchpoint2id_[path] = id;
    id2watchpoint_[id] = path;
}

void
FileWatcher::removeFileWatcher(const QString &path)
{
    QMutexLocker lock(&mutex_);
    WP2ID::const_iterator wp2idit = watchpoint2id_.find(path);
    if (wp2idit != watchpoint2id_.end())
    {
        inotify_rm_watch(inotifyfd_, wp2idit.value());
        watchpoints_.remove(path);
        watchpoint2id_.remove(path);
        id2watchpoint_.remove(wp2idit.value());
    }
}

void
FileWatcher::addDirectoryWatcher(const QString &path, unsigned long mask)
{
    QMutexLocker lock(&mutex_);
    int id = inotify_add_watch(inotifyfd_, path.toStdString().c_str(), mask);
    watchpoint2id_[path] = id;
    id2watchpoint_[id] = path;
}

void
FileWatcher::removeDirectoryWatcher(const QString &path)
{
    QMutexLocker lock(&mutex_);
    WP2ID::const_iterator wp2idit = watchpoint2id_.find(path);
    if (wp2idit != watchpoint2id_.end())
    {
        inotify_rm_watch(inotifyfd_, wp2idit.value());
        watchpoints_.remove(path);
        watchpoint2id_.remove(path);
        id2watchpoint_.remove(wp2idit.value());
    }
}

FileWatcherThread::FileWatcherThread(FileWatcher &fw):
    filewatcher_(fw)
{
    moveToThread(this);
}

FileWatcherThread::~FileWatcherThread()
{
}

void
FileWatcherThread::run()
{
    QSocketNotifier *psn = new QSocketNotifier(filewatcher_.inotifyfd_, QSocketNotifier::Read, this);
    QObject::connect(psn, SIGNAL(activated(int)), 
                     &filewatcher_, SLOT(onDataReceived()), 
                     Qt::DirectConnection);
    assert(psn->isEnabled());
    this->exec();
}

// ctors and dtor
FileSystemWatcher::FileSystemWatcher(QObject *parent):
    QObject(parent), dataMgr_(NULL),
    filewatcher_(), filewatcherthread_(filewatcher_)
{
    dataMgr_ = DataAccessManagement::instance();

    filewatcherthread_.start();

    assignSignalsAndSlots();
}

void
FileSystemWatcher::assignSignalsAndSlots()
{
    // catch signals from native file watcher
    connect(&filewatcher_, SIGNAL(fileCreated(const QString&)), 
            this, SLOT(onFileCreated(const QString&)));
    connect(&filewatcher_, SIGNAL(fileDeleted(const QString&)), 
            this, SLOT(onFileDeleted(const QString&)));

    connect(&filewatcher_, SIGNAL(directoryCreated(const QString&)), 
            this, SLOT(onDirectoryCreated(const QString&)));
    connect(&filewatcher_, SIGNAL(directoryDeleted(const QString&)), 
            this, SLOT(onDirectoryDeleted(const QString&)));

    connect(&filewatcher_, SIGNAL(fileChanged(const QString&)), 
            this, SLOT(onFileChanged(const QString&)));
    connect(&filewatcher_, SIGNAL(fileRenamed(const QString&, const QString&)), 
            this, SLOT(onFileRenamed(const QString&, const QString&)));

    connect(&filewatcher_, SIGNAL(directoryChanged(const QString&)), 
            this, SLOT(onDirectoryChanged(const QString&)));
    connect(&filewatcher_, SIGNAL(directoryRenamed(const QString&, const QString&)), 
            this, SLOT(onDirectoryRenamed(const QString&, const QString&)));

    connect(&filewatcher_, SIGNAL(exception(const QString&, const QString&)), 
            this, SLOT(onException(const QString&, const QString&)));
}

FileSystemWatcher::~FileSystemWatcher()
{
    filewatcherthread_.quit();
    filewatcherthread_.wait();
}

void FileSystemWatcher::addDirectoryWatcher(const QString &path)
{
    if (! isValidPath(path))
    {
        return;
    }
    this->beginRecursiveDirectoryWatch(path);
}

void FileSystemWatcher::endDirectoryWatcher(const QString &path)
{
    if (! isValidPath(path))
    {
        return;
    }
    this->endRecursiveDirectoryWatch(path);
}

void FileSystemWatcher::addDirectoryWatch(const QString  & path)
{
    QDir dir(path);
    dataMgr_->updateShadowHash(path, DataAccessManagement::ShadowHash(true, true));
    filewatcher_.addPath(path);
}

void FileSystemWatcher::endDirectoryWatch(const QString  & path)
{
    if (! isValidPath(path))
    {
        return;
    }
    dataMgr_->updateShadowHash(path, DataAccessManagement::ShadowHash(true, true));
    filewatcher_.removePath(path);
}

void FileSystemWatcher::beginRecursiveDirectoryWatch(const QString  & path)
{
    if (! isValidPath(path))
    {
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

void FileSystemWatcher::endRecursiveDirectoryWatch(const QString  & path)
{
    if (! isValidPath(path))
    {
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

void FileSystemWatcher::onWatcherDirectoryChanged(const QString &path)
{

    if (! isValidPath(path))
    {
        return;
    }

    emit(directoryChanged(path));

    this->endRecursiveDirectoryWatch(path);
    this->beginRecursiveDirectoryWatch(path);
}

void FileSystemWatcher::onFileCreated(const QString &path)
{
    dataMgr_->updateShadowHash(path, DataAccessManagement::ShadowHash(true, false));
    emit(fileCreated(path));
}

void FileSystemWatcher::onDirectoryCreated(const QString &path)
{
    if (! isValidPath(path))
    {
        return;
    }

    dataMgr_->updateShadowHash(path, DataAccessManagement::ShadowHash(true, true));
    emit(directoryCreated(path));

    this->endRecursiveDirectoryWatch(path);
    this->beginRecursiveDirectoryWatch(path);
}

void FileSystemWatcher::onFileDeleted(const QString &path)
{
    dataMgr_->updateShadowHash(path, DataAccessManagement::ShadowHash(true, false));
    emit(fileDeleted(path));
}

void FileSystemWatcher::onDirectoryDeleted(const QString &path)
{
    if (! isValidPath(path))
    {
        return;
    }

    dataMgr_->updateShadowHash(path, DataAccessManagement::ShadowHash(true, true));
    emit(directoryDeleted(path));

    this->endRecursiveDirectoryWatch(path);
    this->beginRecursiveDirectoryWatch(path);
}

void FileSystemWatcher::onFileChanged(const QString &path)
{
    dataMgr_->updateShadowHash(path, DataAccessManagement::ShadowHash(true, false));
    emit(fileChanged(path));
}

void FileSystemWatcher::onDirectoryChanged(const QString &path)
{
    if (! isValidPath(path))
        return;

    dataMgr_->updateShadowHash(path, DataAccessManagement::ShadowHash(true, true));
    emit(directoryChanged(path));

    this->endRecursiveDirectoryWatch(path);
    this->beginRecursiveDirectoryWatch(path);
}

void FileSystemWatcher::onFileRenamed(const QString &oldpath, const QString &newpath)
{
    dataMgr_->updateShadowHash(oldpath, DataAccessManagement::ShadowHash(true, false));
    dataMgr_->updateShadowHash(newpath, DataAccessManagement::ShadowHash(true, false));
    emit(fileRenamed(oldpath, newpath));
}

void FileSystemWatcher::onDirectoryRenamed(const QString &oldpath, const QString &newpath)
{
    dataMgr_->updateShadowHash(oldpath, DataAccessManagement::ShadowHash(true, true));
    dataMgr_->updateShadowHash(newpath, DataAccessManagement::ShadowHash(true, true));
    emit(directoryRenamed(oldpath, newpath));
}

void FileSystemWatcher::onException(const QString &description, const QString &path)
{
    emit(exception(description, path));
}

