#include <QDebug>
#include "FileData.h"

FileData table;

void
insert(const QString &path, const FileData::Data &data)
{
    bool inserted;
    table.insert(path, data, inserted);
    if (inserted)
        qDebug() << "tuple inserted for" << path;
    else
        qDebug() << "tuple NOT inserted for" << path;
    table.print();
}

void
retrieve(const QString &path, FileData::Data &data)
{
    bool found;
    table.retrieve(path, data, found);
    if (found)
    {
        qDebug() << "tuple found for" << path;
        qDebug() << "dummy is" << data.dummy_;
    }
    else
    {
        qDebug() << "tuple NOT found for " << path;
    }
    table.print();
}

int
main(int argc, char **argv)
{
    bool found;

    QString path1("/a/b/c/d");
    FileData::Data data1;
    data1.dummy_ = 10;
    insert(path1, data1);

    QString path3("/a/b/d/d/e/f");
    FileData::Data data3;
    data3.dummy_ = -20;
    insert(path3, data3);

    QString path2("/a/b/d/d");
    FileData::Data data2;
    data2.dummy_ = -10;
    insert(path2, data2);

    FileData::Data rdata;
    retrieve(path1, rdata);
    retrieve(path2, rdata);
    retrieve(path3, rdata);

    table.clear();
    table.print();

    return(0);
}
