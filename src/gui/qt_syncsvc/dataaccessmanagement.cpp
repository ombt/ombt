
#include "dataaccessmanagement.h"

DataAccessManagement::DataAccessManagement(QObject *parent) :
    QObject(parent)
{
    QTRACE();
}

DataAccessManagement::~DataAccessManagement()
{
    QTRACE();
}

DataAccessManagement *
DataAccessManagement::instance(unsigned int version)
{
    static QSharedPointer<DataAccessManagement> theInstance(NULL);
    if (theInstance == NULL)
    {
        QTRACE();
        theInstance = QSharedPointer<DataAccessManagement>(new DataAccessManagement);
    }
    return(theInstance.data());
}

void 
DataAccessManagement::getFileNodePath(const QString& fileNode, QString& path)
{
    try
    {
        QReadLocker locker(&_lock);
        path = QString(_nodeNameToPath[fileNode]);
    }
    catch(const std::exception& ex )
    {
        qCritical() <<  ex.what();
    }
}

void 
DataAccessManagement::getFileNodeName(QString& fileNode, const QString& path)
{

    try
    {
        fileNode.clear();
        QReadLocker locker(&_lock);
        const QString nativePath = QDir::fromNativeSeparators(path);
        FilenodeNameToPathCit cit = _nodeNameToPath.begin();
        while ( cit != _nodeNameToPath.constEnd()  &&  fileNode.isEmpty())
        {
            if ( cit.value() == nativePath )
                fileNode = QString(cit.key());
        }
    }
    catch(const std::exception& ex )
    {
        qCritical() <<  ex.what();
    }
}

void 
DataAccessManagement::addFilenode( const QString& nodeName, const QString& path)
{
    try
    {
        QWriteLocker locker(&_lock);
        QString nativePath = QDir::fromNativeSeparators(path);
        _nodeNameToPath[nodeName] = nativePath;
        _pathToNodeName[nativePath] = nodeName;
    }
    catch(const std::exception& ex )
    {
        qCritical() <<  ex.what();
    }
}

void 
DataAccessManagement::removeFilenode(const QString& nodeName,const QString& path)
{
    try
    {
        QWriteLocker locker(&_lock);
        _nodeNameToPath.remove(nodeName);
        QString nativePath = QDir::fromNativeSeparators(path);
        _pathToNodeName.remove(nativePath);
    }
    catch(const std::exception& ex )
    {
        qCritical() <<  ex.what();
    }
}

bool  
DataAccessManagement::attachFilenodeToService(const QString& serviceName, const QString& nodeName)
{
    bool anyFilenodes = false;
    try
    {
        QWriteLocker locker(&_lock);
        anyFilenodes = _serviceToFileNodes.contains(serviceName, nodeName);
        _serviceToFileNodes.insert(serviceName, nodeName);
    }
    catch(const std::exception& ex )
    {
        qCritical() <<  ex.what();
    }
    return anyFilenodes;
}

bool   
DataAccessManagement::detachFilenodeFromService(const QString& serviceName, const QString& nodeName)
{
    bool anyFilenodes = false;
    try
    {
        QWriteLocker locker(&_lock);
        _serviceToFileNodes.remove(serviceName, nodeName);
        anyFilenodes = _serviceToFileNodes.contains(serviceName, nodeName);
    }
    catch(const std::exception& ex )
    {
        qCritical() <<  ex.what();
    }
    return anyFilenodes;
}

void 
DataAccessManagement::readShadowHash(const QString &path, ShadowHash &data, bool &status)
{
    status = false;
    try
    {
        QReadLocker locker(&_lock);
        QString nativePath = QDir::fromNativeSeparators(path);
        ShadowHashDataCit cit = _shadowHash.find(nativePath);
        ShadowHashDataCit citend = _shadowHash.end();
        if (cit != citend)
        {
            data = *cit;
            status = true;
        }
    }
    catch(const std::exception& ex )
    {
        qCritical() <<  ex.what();
    }
}

void 
DataAccessManagement::insertShadowHash(const QString &path, const ShadowHash &data)
{
    try
    {
        QWriteLocker locker(&_lock);
        QString nativePath = QDir::fromNativeSeparators(path);
        _shadowHash[nativePath] = data;
    }
    catch(const std::exception& ex )
    {
        qCritical() <<  ex.what();
    }
}

void 
DataAccessManagement::updateShadowHash(const QString &path, const ShadowHash &data)
{
    try
    {
        QWriteLocker locker(&_lock);
        QString nativePath = QDir::fromNativeSeparators(path);
        _shadowHash[nativePath] = data;
    }
    catch(const std::exception& ex )
    {
        qCritical() <<  ex.what();
    }
}

void 
DataAccessManagement::removeShadowHash(const QString &path)
{
    try
    {
        QWriteLocker locker(&_lock);
        QString nativePath = QDir::fromNativeSeparators(path);
        _shadowHash.remove(nativePath);
    }
    catch(const std::exception& ex )
    {
        qCritical() <<  ex.what();
    }
}

void 
DataAccessManagement::getShadowHashDirtyFlag(const QString &path, bool &dirtyflag)
{
    dirtyflag = false;
    try
    {
        QReadLocker locker(&_lock);
        QString nativePath = QDir::fromNativeSeparators(path);
        ShadowHashDataCit cit = _shadowHash.find(nativePath);
        ShadowHashDataCit citend = _shadowHash.end();
        if (cit != citend)
        {
            dirtyflag = cit->_dirty;
        }
    }
    catch(const std::exception& ex )
    {
        qCritical() <<  ex.what();
    }
}

void 
DataAccessManagement::setShadowHashDirtyFlag(const QString &path, bool dirtyflag)
{
    try
    {
        QReadLocker locker(&_lock);
        QString nativePath = QDir::fromNativeSeparators(path);
        ShadowHashDataIt it = _shadowHash.find(nativePath);
        ShadowHashDataIt itend = _shadowHash.end();
        if (it != itend)
        {
            it->_dirty = dirtyflag;
        }
    }
    catch(const std::exception& ex )
    {
        qCritical() <<  ex.what();
    }
}

bool 
DataAccessManagement::getFileNodeData(const QString &path, QString &filenode, QString &filenodepath, QString &relativefilepath)
{
    try
    {
        QReadLocker locker(&_lock);

        QString nativePath = QDir::fromNativeSeparators(path);
        QStringList pathparts = nativePath.split(QDir::separator());

        QString searchpath = "";
        PathToFilenodeNameCit pfitend = _pathToNodeName.constEnd();
        for (pathparts.removeFirst(); !pathparts.empty(); )
        {
            QString pathpart = pathparts.takeFirst();
            searchpath += QString("%1%2").arg(QDir::separator()).arg(pathpart);
            PathToFilenodeNameCit pfit = _pathToNodeName.constFind(searchpath);
            if (pfit != pfitend)
            {
                filenode = *pfit;
                filenodepath = searchpath;
                relativefilepath = pathparts.join(QDir::separator());
                QDUMP(filenode);
                QDUMP(filenodepath);
                QDUMP(relativefilepath);
                return true;
            }
        }
        return false;
    }
    catch(const std::exception& ex )
    {
        qCritical() <<  ex.what();
    }
}

void 
DataAccessManagement::readSyncStates(const QString &path, SyncStateData &data, bool &status)
{
    status = false;
    try
    {
        QReadLocker locker(&_lock);
        QString nativePath = QDir::fromNativeSeparators(path);
        SyncStatesCit cit = _syncStates.find(nativePath);
        SyncStatesCit citend = _syncStates.end();
        if (cit != citend)
        {
            data = *cit;
            status = true;
        }
    }
    catch(const std::exception& ex )
    {
        qCritical() <<  ex.what();
    }
}

void 
DataAccessManagement::insertSyncStates(const QString &path, const SyncStateData &data)
{
    try
    {
        QWriteLocker locker(&_lock);
        QString nativePath = QDir::fromNativeSeparators(path);
        _syncStates[nativePath] = data;
    }
    catch(const std::exception& ex )
    {
        qCritical() <<  ex.what();
    }
}

void 
DataAccessManagement::updateSyncStates(const QString &path, const SyncStateData &data)
{
    try
    {
        QWriteLocker locker(&_lock);
        QString nativePath = QDir::fromNativeSeparators(path);
        _syncStates[nativePath] = data;
    }
    catch(const std::exception& ex )
    {
        qCritical() <<  ex.what();
    }
}

void 
DataAccessManagement::removeSyncStates(const QString &path)
{
    try
    {
        QWriteLocker locker(&_lock);
        QString nativePath = QDir::fromNativeSeparators(path);
        _syncStates.remove(nativePath);
    }
    catch(const std::exception& ex )
    {
        qCritical() <<  ex.what();
    }
}

