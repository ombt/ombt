#ifndef __DAS_HASH_H
#define __DAS_HASH_H

#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QCryptographicHash>
#include <QByteArray>
#include <QString>
#include <QDateTime>

#include "DASQDebug.h"
#include "DASDbMgr.h"

class DASHash
{
public:
    static void calcMetaHashForPath(const QString &fullpath, 
                                    QCryptographicHash &fullpathhash,
                                    DASDbMgr *dasdbmgr);
    static void calcMetaHashForDir(const QString &fullpath, 
                                    const QString &filenode,
                                    QCryptographicHash &fullpathhash,
                                    DASDbMgr *dasdbmgr);
    static void calcMetaHashForFile(const QString &fullpath, 
                                    const QString &filenode,
                                    const QFileInfo &fileinfo, 
                                    QCryptographicHash &fullpathhash,
                                    DASDbMgr *dasdbmgr);
    static void calcHashForBuffer(const unsigned char *buffer, QByteArray &hash);
};

#endif
