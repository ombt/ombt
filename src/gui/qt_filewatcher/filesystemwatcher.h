#ifndef FILESYSTEMWATCHER_H
#define FILESYSTEMWATCHER_H

#include  <QFileSystemWatcher>
#include  <QDir>

class FileSystemWatcher : public QObject
{
    Q_OBJECT

signals:
    void fileCreated(const QString & path);
    void directoryCreated(const QString & path);
    void fileDeleted(const QString & path);
    void directoryDeleted(const QString & path);
    void fileChanged(const QString & path);
    void directoryChanged(const QString & path);
    void fileRenamed(const QString & oldPath,const QString & newPath);
    void directoryRenamed(const QString & oldPath, const QString & newPath);

public:
    explicit FileSystemWatcher(QObject *parent = 0);
    virtual ~FileSystemWatcher();

    void addDirectoryWatch(const QString & path);
    void endDirectoryWatch(const QString & path);
    void beginRecursiveDirectoryWatch(const QString & path);
    void endRecursiveDirectoryWatch(const QString & path);
    int fileWatchCount(){return _watcher.files().count();}
    int directoryWatchCount(){return _watcher.directories().count();}
    void dumpLists();

public:
    static bool isValidPath(const QString & path) {
        return QDir(path).exists();
    }

private slots:
    void onWatcherDirectoryChanged(const QString & path);

private:
    bool isDirectoryWatched(const QString & path);

private:
    QFileSystemWatcher _watcher;
};

#endif // FILESYSTEMWATCHER_H


