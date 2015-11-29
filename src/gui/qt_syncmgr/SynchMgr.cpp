#include "SynchMgr.h"

// ctors and dtor
SynchMgr::SynchMgr(DBMgr &dbmgr, MsgQueue &msgqueue):
    QObject(), dbmgr_(dbmgr), msgqueue_(msgqueue)
{
    assignSignalsAndSlots();
}

SynchMgr::~SynchMgr()
{
}

// set up connections
void 
SynchMgr::assignSignalsAndSlots()
{
}

