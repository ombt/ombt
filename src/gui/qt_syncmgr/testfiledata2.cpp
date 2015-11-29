
#include <stdio.h>

#include <QDebug>
#include <QFile>
#include <QTextStream>
#include <QFileInfo>

#include "FileData.h"

int
main(int, char **)
{
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
    }

    table.print();

    return(0);
}
