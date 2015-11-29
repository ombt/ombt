#ifndef _DASQDEBUG_H
#define _DASQDEBUG_H

#include <QDebug>

#define USEDASQDEBUG 1

#if USEDASQDEBUG

#define QTRACE() \
    qDebug() << __FILE__ << "[" << __FUNCTION__ << "]'" << __LINE__
#define QDUMP(msg) \
    qDebug() << __FILE__ << "[" << __FUNCTION__ << "]'" << __LINE__ << " " #msg " = " << msg
#define QDUMP2(msg, msg2) \
    qDebug() << __FILE__ << "[" << __FUNCTION__ << "]'" << __LINE__ \
             << " " #msg " = " << msg << " " #msg2 " = " << msg2

#else

#define QTRACE()
#define QDUMP(msg)
#define QDUMP2(msg, msg2)

#endif

#define QCRITTRACE() \
    qDebug() << __FILE__ << "[" << __FUNCTION__ << "]'" << __LINE__
#define QCRITDUMP(msg) \
    qDebug() << __FILE__ << "[" << __FUNCTION__ << "]'" << __LINE__ << " " #msg " = " << msg
#define QCRITDUMP2(msg, msg2) \
    qDebug() << __FILE__ << "[" << __FUNCTION__ << "]'" << __LINE__ \
             << " " #msg " = " << msg << " " #msg2 " = " << msg2
#define QERRORDUMP(errormsg, datatodump) \
    qDebug() << "ERROR at " << __FILE__ << "[" << __FUNCTION__ << "]'" << __LINE__ \
             << errormsg << " " #datatodump " = " << datatodump

#endif
