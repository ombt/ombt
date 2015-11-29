
#include "MdiMainWindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    MdiMainWindow mainwin;
    mainwin.show();

    return app.exec();
}
