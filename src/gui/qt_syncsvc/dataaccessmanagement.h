#ifndef DATAACCESSMANAGEMENT_H
#define DATAACCESSMANAGEMENT_H

#include <QObject>
#include <QString>
#include <QStringList>
#include <QHash>
#include <QMultiHash>
#include <QReadWriteLock>
#include <QDir>
#include <QDebug>
#include <QSharedPointer>

#include "DASQDebug.h"

// Notes:
//  Service names are unique accross all device in HC
//  Filenode names are unique for each DA
//  FileNode => Directory Path ( 1 to 1)
//  FileNode => Services (many to many)

class DataAccessManagement : public QObject
{
    Q_OBJECT

public:
    enum { 
        Default=0
    };

    struct ShadowHash
    {
        ShadowHash():
            _dirty(false), _isdirectory(false) { }
        ShadowHash(bool dirty, bool isdirectory):
            _dirty(dirty), _isdirectory(isdirectory) { }
        bool _dirty;
        bool _isdirectory;
    };

    struct SyncStateData
    {
        SyncStateData():
            dummy_(0) { }
        ~SyncStateData() { }
        int dummy_;
    };

    typedef QHash<QString, ShadowHash> ShadowHashData;
    typedef ShadowHashData::iterator ShadowHashDataIt;
    typedef ShadowHashData::const_iterator ShadowHashDataCit;

    typedef QMultiHash<QString,QString> ServiceToFilenodes;
    typedef ServiceToFilenodes::const_iterator ServiceToFilenodesCit;

    typedef QHash<QString, QString> FilenodeNameToPath;
    typedef FilenodeNameToPath::const_iterator FilenodeNameToPathCit;

    typedef QHash<QString, QString> PathToFilenodeName;
    typedef PathToFilenodeName::const_iterator PathToFilenodeNameCit;

    typedef QHash<QString, SyncStateData> SyncStates;
    typedef SyncStates::const_iterator SyncStatesCit;

public:
    virtual ~DataAccessManagement();

    static DataAccessManagement *instance(unsigned int version = Default);

    void getFileNodePath(const QString& fileNode, QString& path) ;
    void getFileNodeName(QString& fileNode, const QString& path) ;

    void getServiceFileNodes(QStringList& fileNode,const  QString& path) const;
    void getFileNodeServices(const QString& fileNode, QStringList& services) const;

    void addFilenode(const QString& nodeName, const QString& path);
    void removeFilenode(const QString& nodeName, const QString& path);

    bool attachFilenodeToService(const QString& serviceName, const QString& nodeName);
    bool detachFilenodeFromService(const QString& serviceName, const QString& nodeName);

    void readShadowHash(const QString &path, ShadowHash &data, bool &status);
    void insertShadowHash(const QString &path, const ShadowHash &data);
    void updateShadowHash(const QString &path, const ShadowHash &data);
    void removeShadowHash(const QString &path);

    void getShadowHashDirtyFlag(const QString &path, bool &dirtyflag);
    void setShadowHashDirtyFlag(const QString &path, bool dirtyflag);

    bool getFileNodeData(const QString &path, QString &filenode, QString &filenodepath, QString &relativefilepath);

    void readSyncStates(const QString &filenode, SyncStateData &data, bool &status);
    void insertSyncStates(const QString &filenode, const SyncStateData &data);
    void updateSyncStates(const QString &filenode, const SyncStateData &data);
    void removeSyncStates(const QString &filenode);

signals:

public slots:

private:
    explicit DataAccessManagement(QObject *parent = 0);

private:
    QReadWriteLock _lock;

    ServiceToFilenodes _serviceToFileNodes;
    FilenodeNameToPath _nodeNameToPath;
    PathToFilenodeName _pathToNodeName;

    ShadowHashData _shadowHash;
    SyncStates _syncStates;
};

#endif // DATAACCESSMANAGEMENT_H
