#include <QApplication>

#include "SdiMainWindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

#if 0
    SdiMainWindow *mainwin = new SdiMainWindow(argv[1]);
#else
    SdiMainWindow *mainwin = new SdiMainWindow();
#endif

    mainwin->show();

    return app.exec();
}
