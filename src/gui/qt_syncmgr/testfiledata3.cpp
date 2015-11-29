
#include <stdio.h>

#include <QDebug>
#include <QFile>
#include <QTextStream>
#include <QFileInfo>
#include <QStringList>

#include "FileData.h"

int
main(int, char **)
{
    QStringList allpaths;
    FileData table;

    QTextStream stream(stdin);

    while (true)
    {
        QString path = stream.readLine(); 
        if (path.isNull()) break;

        QFileInfo pathfi(path);

        FileData::Data data;
        data.isdir_ = pathfi.isDir();
        path = pathfi.absoluteFilePath();

        bool flag;
        table.insert(path, data, flag);
        allpaths.push_back(path);
    }

    qDebug() << "after reading and inserting all paths";
    table.print();

    QStringList::iterator it = allpaths.begin();
    QStringList::iterator itend = allpaths.end();
    for ( ; it != itend; ++it)
    {
        bool flag;
        qDebug() << "deleting path" << *it;
        table.remove(*it, flag);
        qDebug() << "deleted flag is" << flag;
    }

    qDebug() << "after deleting all paths";
    table.print();

    return(0);
}
