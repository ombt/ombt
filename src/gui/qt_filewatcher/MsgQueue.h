#ifndef __MSG_QUEUE_H
#define __MSG_QUEUE_H

// headers
#include <QObject>
#include <QList>
#include <QString>
#include <QMutex>
#include <QWaitCondition>

#include "DASQDebug.h"

class MsgQueue: public QObject
{
    Q_OBJECT

public:
    explicit MsgQueue();
    virtual ~MsgQueue();

    void enqueue(const QString &msg);
    bool dequeue(QString &msg);
    bool head(QString &msg);
    void done();

protected:
    bool done_;
    QMutex mutex_;
    QWaitCondition condvar_;
    QList<QString> msgqueue_;
};

#endif
