// SDi window demo

// headers
#include "MdiMainWindow.h"

// ctors and dtor
MdiMainWindow::MdiMainWindow():
    mdiarea_(NULL)
{
    // create central area where windows are created
    mdiarea_ = new QMdiArea;
    assert(mdiarea_ != NULL);

    // scrollbar policy
    mdiarea_->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    mdiarea_->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);

    // add mdi area to the current window
    setCentralWidget(mdiarea_);

    // create windows and widgets
    createButtons();
    createActions();
    createMenus();
    createToolBars();
    createStatusBar();
    updateAll();

    // label window
    setWindowTitle(QString("DAS Emulation Viewer"));
}

MdiMainWindow::~MdiMainWindow()
{
    // do nothing
}

// create windows and widgets
void
MdiMainWindow::createButtons()
{
}

void
MdiMainWindow::createActions()
{
    newaction_ = new QAction(tr("&New DAS"), this);
    newaction_->setShortcuts(QKeySequence::New);
    newaction_->setStatusTip(tr("Create a new DAS"));
    connect(newaction_, SIGNAL(triggered()), this, SLOT(newFileWatcher()));

    exitaction_ = new QAction(tr("E&xit"), this);
    exitaction_->setShortcut(tr("Ctrl+Q"));
    exitaction_->setStatusTip(tr("Exit the application"));
    connect(exitaction_, SIGNAL(triggered()), qApp, SLOT(closeAllWindows()));

    closeactwindowaction_ = new QAction(tr("Cl&ose"), this);
    closeactwindowaction_->setShortcut(tr("Ctrl+F4"));
    closeactwindowaction_->setStatusTip(tr("Close the active window"));
    connect(closeactwindowaction_, SIGNAL(triggered()), mdiarea_, SLOT(closeActiveSubWindow()));

    closeallwindowsaction_ = new QAction(tr("Close &All"), this);
    closeallwindowsaction_->setStatusTip(tr("Close all the windows"));
    connect(closeallwindowsaction_, SIGNAL(triggered()), mdiarea_, SLOT(closeAllSubWindows()));

    tilewindowsaction_ = new QAction(tr("&Tile"), this);
    tilewindowsaction_->setStatusTip(tr("Tile the windows"));
    connect(tilewindowsaction_, SIGNAL(triggered()), mdiarea_, SLOT(tileSubWindows()));

    cascadewindowsaction_ = new QAction(tr("&Cascade"), this);
    cascadewindowsaction_->setStatusTip(tr("Cascade the windows"));
    connect(cascadewindowsaction_, SIGNAL(triggered()), mdiarea_, SLOT(cascadeSubWindows()));

    nextwindowaction_ = new QAction(tr("Ne&xt"), this);
    nextwindowaction_->setShortcuts(QKeySequence::NextChild);
    nextwindowaction_->setStatusTip(tr("Move the focus to the next window"));
    connect(nextwindowaction_, SIGNAL(triggered()), mdiarea_, SLOT(activateNextSubWindow()));

    previouswindowaction_ = new QAction(tr("Pre&vious"), this);
    previouswindowaction_->setShortcuts(QKeySequence::PreviousChild);
    previouswindowaction_->setStatusTip(tr("Move the focus to the previous " "window"));
    connect(previouswindowaction_, SIGNAL(triggered()),
            mdiarea_, SLOT(activatePreviousSubWindow()));

    separatoraction_ = new QAction(this);
    separatoraction_->setSeparator(true);

    aboutaction_ = new QAction(tr("&About"), this);
    aboutaction_->setStatusTip(tr("Show the application's About box"));
    connect(aboutaction_, SIGNAL(triggered()), this, SLOT(about()));
}

void
MdiMainWindow::createMenus()
{
    filemenu_ = menuBar()->addMenu(tr("&File"));
    filemenu_->addAction(newaction_);
    filemenu_->addSeparator();
    filemenu_->addAction(exitaction_);

    windowmenu_ = menuBar()->addMenu(tr("&Window"));
    updateWindowMenu();
    connect(windowmenu_, SIGNAL(aboutToShow()), this, SLOT(updateWindowMenu()));

    menuBar()->addSeparator();

    helpmenu_ = menuBar()->addMenu(tr("&Help"));
    helpmenu_->addAction(aboutaction_);
}

void
MdiMainWindow::createToolBars()
{
    filetoolbar_ = addToolBar(tr("File"));
    filetoolbar_->addAction(newaction_);
}

void
MdiMainWindow::createStatusBar()
{
    statusBar()->showMessage(tr("Ready"));
}

// update menus, etc. when subwindows change
void
MdiMainWindow::updateAll()
{
}

// slots
void
MdiMainWindow::updateWindowMenu()
{
     windowmenu_->clear();

     windowmenu_->addAction(closeactwindowaction_);
     windowmenu_->addAction(closeallwindowsaction_);

     windowmenu_->addSeparator();

     windowmenu_->addAction(tilewindowsaction_);
     windowmenu_->addAction(cascadewindowsaction_);

     windowmenu_->addSeparator();

     windowmenu_->addAction(nextwindowaction_);
     windowmenu_->addAction(previouswindowaction_);
}

void
MdiMainWindow::about()
{
    QMessageBox::about(this,
        tr("About File Watcher"),
        tr("File Watcher and Sync. Algo. Simulator"));
}

void
MdiMainWindow::newFileWatcher()
{
    FileWatcherWindow *subwindow = new FileWatcherWindow();
    mdiarea_->addSubWindow(subwindow);
    subwindow->show();
    statusBar()->showMessage(tr("File Watcher started"), 2000);
}

