
#include <assert.h>

#include <iostream>
#include <QtGui>

#include "TreeModel.h"

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        std::cerr << "usage: " << argv[0] << " input.file" << std::endl;
        return(2);
    }

    QApplication app(argc, argv);

    TreeModel model(argv[1]);
    assert(model.isOk());

    QTreeView view;
    view.setModel(&model);
    view.setWindowTitle(QObject::tr("Simple Tree Model"));
    view.show();

    return app.exec();
}
