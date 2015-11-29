// headers
#include "transactionlog.h"

TransactionLog::TransactionLog(QObject *parent): 
    QObject(parent), lock_()
{
    QTRACE();
}

TransactionLog::~TransactionLog()
{
    QTRACE();
}

TransactionLog *
TransactionLog::instance()
{
    static QSharedPointer<TransactionLog> theInstance(NULL);
    if (theInstance == NULL)
    {
        QTRACE();
        theInstance = QSharedPointer<TransactionLog>(new TransactionLog);
    }
    return(theInstance.data());
}

void 
TransactionLog::getTransactionLog(const QString &filenode, QString &transactionlog)
{
    QReadLocker locker(&lock_);
    TransactionLog::FileNodeTransactionsIt it = transactions_.find(filenode);
    if (it != transactions_.end())
    {
        transactionlog = (*it)->join("\n");
    }
    else
    {
        transactionlog = "";
    }
}

void 
TransactionLog::clearTransactionLog(const QString &filenode)
{
    QWriteLocker locker(&lock_);
    transactions_.remove(filenode);
}

// true = insert new record, false = do not insert new record
bool 
TransactionLog::normalizeLastRecord(QStringList &filenodelog, const QString &lastrecord)
{
    // simple test for now
    if (filenodelog.isEmpty()) 
        return(false);
    else if (filenodelog.last() == lastrecord)
        return true;
    else
        return false;
}

void 
TransactionLog::writeLogRecord(const QString &filenode, const QString &logrecord, bool normalize)
{
    TransactionLog::FileNodeTransactionsIt it = transactions_.find(filenode);
    if (it != transactions_.end())
    {
        if (normalize)
        {
            if ( ! normalizeLastRecord(**it, logrecord))
            {
                (*it)->push_back(logrecord);
            }
        }
        else
        {
            (*it)->push_back(logrecord);
        }
    }
    else
    {
        QSharedPointer<QStringList> translog(new QStringList);
        translog->push_back(logrecord);
        transactions_[filenode] = translog;
    }
}

void 
TransactionLog::writeFileCreatedLogRecord(const QString &filenode, const QString &filenodepath, const QString &relativefilepath)
{
    QWriteLocker locker(&lock_);
    writeLogRecord(filenode, QString("FileCreated:%1:%2:%3").arg(filenode).arg(filenodepath).arg(relativefilepath));
}

void
TransactionLog::writeDirectoryCreatedLogRecord(const QString &filenode, const QString &filenodepath, const QString &relativefilepath)
{
    QWriteLocker locker(&lock_);
    writeLogRecord(filenode, QString("DirectoryCreated:%1:%2:%3").arg(filenode).arg(filenodepath).arg(relativefilepath));
}

void
TransactionLog::writeFileDeletedLogRecord(const QString &filenode, const QString &filenodepath, const QString &relativefilepath)
{
    QWriteLocker locker(&lock_);
    writeLogRecord(filenode, QString("FileDeleted:%1:%2:%3").arg(filenode).arg(filenodepath).arg(relativefilepath));
}

void
TransactionLog::writeDirectoryDeletedLogRecord(const QString &filenode, const QString &filenodepath, const QString &relativefilepath)
{
    QWriteLocker locker(&lock_);
    writeLogRecord(filenode, QString("DirectoryDeleted:%1:%2:%3").arg(filenode).arg(filenodepath).arg(relativefilepath));
}

void
TransactionLog::writeFileChangedLogRecord(const QString &filenode, const QString &filenodepath, const QString &relativefilepath)
{
    QWriteLocker locker(&lock_);
    writeLogRecord(filenode, QString("FileChanged:%1:%2:%3").arg(filenode).arg(filenodepath).arg(relativefilepath), true);
}

void
TransactionLog::writeDirectoryChangedLogRecord(const QString &filenode, const QString &filenodepath, const QString &relativefilepath)
{
    QWriteLocker locker(&lock_);
    writeLogRecord(filenode, QString("DirectoryChanged:%1:%2:%3").arg(filenode).arg(filenodepath).arg(relativefilepath));
}

void
TransactionLog::writeFileRenamedLogRecord(const QString &filenode, const QString &filenodepath, const QString &relativefilepath)
{
    QWriteLocker locker(&lock_);
    writeLogRecord(filenode, QString("FileRenamed:%1:%2:%3").arg(filenode).arg(filenodepath).arg(relativefilepath));
}

void
TransactionLog::writeDirectoryRenamedLogRecord(const QString &filenode, const QString &filenodepath, const QString &relativefilepath)
{
    QWriteLocker locker(&lock_);
    writeLogRecord(filenode, QString("DirectoryRenamed:%1:%2:%3").arg(filenode).arg(filenodepath).arg(relativefilepath));
}
