#include <QApplication>

#include "MdiMainWindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    MdiMainWindow *mainwin = new MdiMainWindow();

    mainwin->show();

    return app.exec();
}
