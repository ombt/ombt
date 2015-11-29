#ifndef __TRANSACTION_LOG_H
#define __TRANSACTION_LOG_H

// headers
#include <QObject>
#include <QString>
#include <QStringList>
#include <QReadWriteLock>
#include <QSharedPointer>
#include <QReadLocker>
#include <QWriteLocker>

#include "DASQDebug.h"

class TransactionLog: public QObject
{
    Q_OBJECT

public:
    typedef QHash<QString, QSharedPointer<QStringList> > FileNodeTransactions;
    typedef FileNodeTransactions::iterator FileNodeTransactionsIt;

    virtual ~TransactionLog();

    static TransactionLog *instance();

    void getTransactionLog(const QString &filenode, QString &translogblob);
    void clearTransactionLog(const QString &filenode);

    void writeFileCreatedLogRecord(const QString &filenode, const QString &filenodepath, const QString &relativefilepath);
    void writeDirectoryCreatedLogRecord(const QString &filenode, const QString &filenodepath, const QString &relativefilepath);
    void writeFileDeletedLogRecord(const QString &filenode, const QString &filenodepath, const QString &relativefilepath);
    void writeDirectoryDeletedLogRecord(const QString &filenode, const QString &filenodepath, const QString &relativefilepath);
    void writeFileChangedLogRecord(const QString &filenode, const QString &filenodepath, const QString &relativefilepath);
    void writeDirectoryChangedLogRecord(const QString &filenode, const QString &filenodepath, const QString &relativefilepath);
    void writeFileRenamedLogRecord(const QString &filenode, const QString &filenodepath, const QString &relativefilepath);
    void writeDirectoryRenamedLogRecord(const QString &filenode, const QString &filenodepath, const QString &relativefilepath);

protected:
    explicit TransactionLog(QObject *parent = 0);
    void writeLogRecord(const QString &filenode, const QString &logrecord, bool normalize = false);
    bool normalizeLastRecord(QStringList &filenodelog, const QString &lastrecord);

protected:
    QReadWriteLock lock_;
    FileNodeTransactions transactions_;
};

#endif
