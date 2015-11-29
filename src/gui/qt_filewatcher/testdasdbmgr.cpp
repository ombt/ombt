
#include <QCoreApplication>
#include <QDebug>

#include "DASQDebug.h"
#include "DASDbMgr.h"

static void
usage(const QString &cmd)
{
    qDebug() << "usage: " << cmd << " ";
}

int
main(int argc, char **argv)
{
    QCoreApplication app(argc, argv);

    if (argc != 1)
    {
        usage(argv[0]);
        return(2);
    }

    DASDbMgr dasdbm;

    QString das1("DAS1");
    QSet<QString> das1files;
    das1files.insert("/tmp/xxx");
    das1files.insert("/tmp/yyy");
    das1files.insert("/tmp/zzz");
    das1files.insert("/home/ombt/xxx");
    das1files.insert("/home/ombt/yyy");
    das1files.insert("/home/ombt/zzz");

    bool found;
    QSet<QString> data;
    dasdbm.readFileNodeToFiles(das1, data, found);
    qDebug() << "Count of empty FileNode2Files is" << data.count();

    dasdbm.insertFileNodeToFiles(das1, das1files);
    data.clear();
    dasdbm.readFileNodeToFiles(das1, data, found);
    qDebug() << "Count of FileNode2Files after 1 insert is" << data.count();

    dasdbm.deleteFileNodeToFiles(das1);
    data.clear();
    dasdbm.readFileNodeToFiles(das1, data, found);
    qDebug() << "Count of FileNode2Files after 1 delete is" << data.count();

    return(0);
}
