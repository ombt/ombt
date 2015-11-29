
#include <stdio.h>

#include <QDebug>
#include <QFile>
#include <QTextStream>
#include <QFileInfo>
#include <QStringList>

#include "FileData.h"
#include "List.h"
#include "Tuple.h"

typedef Tuple<QString, FileData::Data> FileDataTuple;

int
main(int, char **)
{
    List<FileDataTuple> list;

    QStringList allpaths;

    QTextStream stream(stdin);

    while (true)
    {
        QString path = stream.readLine(); 
        if (path.isNull()) break;

        QFileInfo pathfi(path);

        FileData::Data data;
        data.isdir_ = pathfi.isDir();
        path = pathfi.absoluteFilePath();

        list.insertOrdered(FileDataTuple(path, data));
        allpaths.push_back(path);
    }

    qDebug() << "count after reading and inserting all paths" << list.getCount();

    for (ListIterator<FileDataTuple> lit(list);
         !lit.done(); lit++)
    {
        qDebug() << "tuple key" << lit().key;
    }

    FileData::Data dummy;
    QStringList::iterator it = allpaths.begin();
    QStringList::iterator itend = allpaths.end();
    for ( ; it != itend; ++it)
    {
        FileDataTuple key(*it, dummy);
        list.removeOrdered(key);
    }

    qDebug() << "count after deleting all paths" << list.getCount();

    for (ListIterator<FileDataTuple> lit(list);
         !lit.done(); lit++)
    {
        qDebug() << "tuple key" << lit().key;
    }


    return(0);
}
