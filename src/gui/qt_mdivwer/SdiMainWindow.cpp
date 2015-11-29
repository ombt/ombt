// SDi window demo

// headers
#include <assert.h>

#include "SdiMainWindow.h"
#include "TreeModel.h"

// ctors and dtor
#if 0
SdiMainWindow::SdiMainWindow(const std::string &filename):
#else
SdiMainWindow::SdiMainWindow():
#endif
    ok_(false), treemodel_(NULL), treeview_(NULL),
    openaction_(NULL), exitaction_(NULL), filemenu_(NULL)
{
    setAttribute(Qt::WA_DeleteOnClose);
    setWindowTitle(QString("SDI Simple Tree Model Viewer"));

#if 0
    treemodel_ = new TreeModel(filename);
    assert(treemodel_ != NULL);
    assert(treemodel_->isOk());
#endif

    treeview_ = new QTreeView();
    assert(treeview_ != NULL);
    // treeview_->verticalHeader().setVisible(true);

#if 0
    treeview_->setModel(treemodel_);
#endif
    setCentralWidget(treeview_);

    createActions();
    createMenus();
    createToolBars();
    createStatusBar();

    ok_ = true;
}

SdiMainWindow::~SdiMainWindow()
{
    // do nothing
}

// make window widgets
void
SdiMainWindow::createActions()
{
    openaction_ = new QAction(tr("&Open"), this);
    openaction_->setShortcut(QKeySequence::Open);
    openaction_->setStatusTip(tr("Open an existing file"));
    connect(openaction_, SIGNAL(triggered()), this, SLOT(openFile()));

    expandallaction_ = new QAction(tr("&Expand All"), this);
    expandallaction_->setStatusTip(tr("Expand all rows"));
    connect(expandallaction_, SIGNAL(triggered()), this, SLOT(expandAll()));

    closeallaction_ = new QAction(tr("&Close All"), this);
    closeallaction_->setStatusTip(tr("Close all rows"));
    connect(closeallaction_, SIGNAL(triggered()), this, SLOT(closeAll()));

    exitaction_ = new QAction(tr("E&xit"), this);
    exitaction_->setShortcut(tr("Ctrl+Q"));
    exitaction_->setStatusTip(tr("Exit the application"));
    connect(exitaction_, SIGNAL(triggered()), qApp, SLOT(closeAllWindows()));
}

void
SdiMainWindow::createMenus()
{
    filemenu_ = menuBar()->addMenu(tr("&File"));
    filemenu_->addAction(openaction_);
    filemenu_->addAction(expandallaction_);
    filemenu_->addAction(closeallaction_);
    filemenu_->addAction(exitaction_);
}

void
SdiMainWindow::createToolBars()
{
    // nothing for now
}

void
SdiMainWindow::createStatusBar()
{
    // nothing for now
}

// dialog to get a file name
void
SdiMainWindow::expandAll()
{
    treeview_->expandAll();
    treeview_->show();
}

void
SdiMainWindow::closeAll()
{
    treeview_->collapseAll();
    treeview_->show();
}

void
SdiMainWindow::openFile()
{
    QString filename = QFileDialog::getOpenFileName(this);
    if (filename.isEmpty()) return;

    TreeModel *newtreemodel = new TreeModel(filename.toStdString());
    assert(newtreemodel != NULL);

    if ( ! newtreemodel->isOk())
    {
        delete newtreemodel;
        return;
    }

    treeview_->hide();
    treeview_->setModel(newtreemodel);
    treeview_->show();

    if (treemodel_ != NULL) delete treemodel_;

    treemodel_ = newtreemodel;
}
