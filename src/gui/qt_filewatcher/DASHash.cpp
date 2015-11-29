
#include "DASHash.h"

void 
DASHash::calcMetaHashForDir(const QString &filenode, 
                            const QString &fullpath, 
                            QCryptographicHash &cumulativehash,
                            DASDbMgr *dasdbmgr)
{
    QCRITDUMP2(filenode, fullpath);

    QDir fpdir(fullpath);

    QStringList subdirlist(fpdir.entryList(QDir::Dirs | 
                                           QDir::NoDotAndDotDot |
                                           QDir::Hidden |
                                           QDir::CaseSensitive, 
                                           QDir::Name));
    foreach(QString subdir, subdirlist)
    {
        QCryptographicHash subdirhash(QCryptographicHash::Sha1);
        QString fpsubdir = 
            QString("%1%2%3").arg(fullpath).arg(QDir::separator()).arg(subdir);
        calcMetaHashForDir(filenode, fpsubdir, subdirhash, dasdbmgr);
        cumulativehash.addData(subdirhash.result());
        if (dasdbmgr)
        {
            dasdbmgr->insertFileMetaHash(fpsubdir, 
               DASDbSchema::FileMetaHashData(
                   filenode, subdirhash.result(), false, true, 
                   DASDbSchema::NotInQueue));
        }
    }

    QStringList filelist(fpdir.entryList(QDir::Files | 
                                         QDir::Hidden |
                                         QDir::CaseSensitive, 
                                         QDir::Name));
    foreach(QString filename, filelist)
    {
        QCryptographicHash filehash(QCryptographicHash::Sha1);
        QString fpfilename = 
            QString("%1%2%3").arg(fullpath).arg(QDir::separator()).arg(filename);
        QFileInfo fpfnfileinfo(fpfilename);
        calcMetaHashForFile(filenode, fpfilename, fpfnfileinfo, filehash, dasdbmgr);
        cumulativehash.addData(filehash.result());
        if (dasdbmgr)
        {
            dasdbmgr->insertFileMetaHash(fpfilename, 
                DASDbSchema::FileMetaHashData(
                    filenode, filehash.result(), false, false, 
                    DASDbSchema::NotInQueue));
        }
    }
}


void 
DASHash::calcMetaHashForFile(const QString &filenode, 
                             const QString &fullpath, 
                             const QFileInfo &fileinfo,
                             QCryptographicHash &filehash,
                             DASDbMgr *dasdbmgr)
{
    QCRITDUMP2(filenode, fullpath);

    QDateTime lastmodtime = fileinfo.lastModified();
    QCRITDUMP(lastmodtime.toString());

    qint64 filesize = fileinfo.size();
    QCRITDUMP(filesize);

    int permissions = int(fileinfo.permissions());
    QCRITDUMP(permissions);

    QString strmetahash(QString("%1%2%3%4").arg(fullpath).arg(lastmodtime.toString()).arg(filesize).arg(permissions));
    QCRITDUMP(strmetahash);

    filehash.addData(strmetahash.toStdString().c_str(), 
                     strmetahash.toStdString().size());
}

void 
DASHash::calcMetaHashForPath(const QString &fullpath, 
                             QCryptographicHash &cumulativehash,
                             DASDbMgr *dasdbmgr)
{
    QCRITDUMP(fullpath);
    QFileInfo fpfileinfo(fullpath);
    if (fpfileinfo.isDir())
    {
        calcMetaHashForDir(fullpath, fullpath, 
                           cumulativehash, dasdbmgr);
        if (dasdbmgr)
        {
            dasdbmgr->insertFileMetaHash(fullpath, 
               DASDbSchema::FileMetaHashData(
                   fullpath, cumulativehash.result(), false, true, 
                   DASDbSchema::NotInQueue));
        }
    }
    else if (fpfileinfo.isFile())
    {
        calcMetaHashForFile(fullpath, fullpath, fpfileinfo, 
                            cumulativehash, dasdbmgr);
    }
}

void 
DASHash::calcHashForBuffer(const unsigned char *buffer, QByteArray &hash)
{
}

