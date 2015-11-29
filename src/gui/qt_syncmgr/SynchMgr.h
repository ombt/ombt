#ifndef __SYNCH_MGR_H
#define __SYNCH_MGR_H

#include <QObject>
#include <QString>

#include "DBMgr.h"
#include "MsgQueue.h"

class SynchMgr: public QObject
{
    Q_OBJECT

signals:
    void statusMessage(const QString &message);

public:
    explicit SynchMgr(DBMgr &dbmgr, MsgQueue &msgqueue);
    virtual ~SynchMgr();

public slots:

private:
    void assignSignalsAndSlots();

protected:
    DBMgr &dbmgr_;
    MsgQueue &msgqueue_;
};

#endif
