
#include "testfilewatcherthread.h"

FileWatcherThread::FileWatcherThread():
    inotifyfd_(-1)
{
    QTRACE();
    inotifyfd_ = inotify_init();
    QDUMP(inotifyfd_);
    moveToThread(this);
    QTRACE();
}

FileWatcherThread::~FileWatcherThread()
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
FileWatcherThread::run()
{
    QTRACE();
    QSocketNotifier *psn = new QSocketNotifier(inotifyfd_, QSocketNotifier::Read, this);
    QObject::connect(psn, SIGNAL(activated(int)), 
                     this, SLOT(onDataReceived()), 
                     Qt::DirectConnection);
    assert(psn->isEnabled());
    QTRACE();
    this->exec();
    QTRACE();
}

void
FileWatcherThread::onDataReceived()
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
         QString watchpointpath;
         ID2WP::const_iterator id2wpit = id2watchpoint_.find(pevent->wd);
         if (id2wpit != id2watchpoint_.end())
             watchpointpath = id2wpit.value();
         else
             watchpointpath = "";
         qDebug() << "Watch descriptor:" << pevent->wd;
         qDebug() << "Event Path      :" << watchpointpath;
         qDebug() << "Event Mask      :" << pevent->mask;
         qDebug() << "Event Cookie    :" << pevent->cookie;
         qDebug() << "Event Length    :" << pevent->len;
         if (pevent->len > 0)
             qDebug() << "File Name       :" <<  pevent->name;
    }
}

void
FileWatcherThread::addFileWatcher(const QString &path, unsigned long mask)
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
FileWatcherThread::removeFileWatcher(const QString &path)
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
FileWatcherThread::addDirectoryWatcher(const QString &path, unsigned long mask)
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
FileWatcherThread::removeDirectoryWatcher(const QString &path)
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

static void
usage(const QString &cmd)
{
    qDebug() << "usage: " << cmd << " seconds dir1 [dir2...]";
}

int
main(int argc, char **argv)
{
    QCoreApplication app(argc, argv);

    if (argc <= 2)
    {
        usage(argv[0]);
        return(2);
    }

    int arg = 0;
    int seconds = ::atoi(argv[++arg]);

    FileWatcherThread fwthread;
    fwthread.start();

    for (int iarg = ++arg; iarg<argc; ++iarg)
    {
        QString qpath(argv[iarg]);
        if (QDir(qpath).exists())
        {
            fwthread.addDirectoryWatcher(qpath, 
                IN_ATTRIB|IN_CREATE|IN_DELETE|IN_DELETE_SELF|
                IN_MODIFY|IN_MOVE_SELF|IN_MOVED_FROM|IN_MOVED_TO);
        }
        else if (QFile(qpath).exists())
        {
            fwthread.addFileWatcher(qpath,
                IN_ATTRIB|IN_CREATE|IN_DELETE|IN_DELETE_SELF|
                IN_MODIFY|IN_MOVE_SELF|IN_MOVED_FROM|IN_MOVED_TO);
        }
    }

    ::sleep(seconds);

    for (int iarg = arg; iarg<argc; ++iarg)
    {
        QString qpath(argv[iarg]);
        if (QDir(qpath).exists())
        {
            fwthread.removeDirectoryWatcher(qpath);
        }
        else if (QFile(qpath).exists())
        {
            fwthread.removeFileWatcher(qpath);
        }
    }

    fwthread.quit();
    fwthread.wait();

    return(0);
}
