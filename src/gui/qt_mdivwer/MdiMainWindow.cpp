// SDi window demo

// headers
#include <assert.h>
#include <iostream>

#include "MdiMainWindow.h"
#include "MdiTreeView.h"

// ctors and dtor
MdiMainWindow::MdiMainWindow():
    ok_(false), 
    usedummymodel_(false), 
    mdiarea_(NULL),
    windowmapper_(NULL),
    usedummymodelbutton_(NULL),
    openaction_(NULL),
    exitaction_(NULL),
    closeactwindowaction_(NULL),
    closeallwindowsaction_(NULL),
    tilewindowsaction_(NULL),
    cascadeaction_(NULL),
    nextwindowaction_(NULL),
    previouswindowaction_(NULL),
    separatoraction_(NULL),
    aboutaction_(NULL),
    closeallaction_(NULL),
    expandallaction_(NULL),
    enablereadonlyaction_(NULL),
    disablereadonlyaction_(NULL),
    insertrowaction_(NULL),
    removerowaction_(NULL),
    insertcolumnaction_(NULL),
    removecolumnaction_(NULL),
    insertchildaction_(NULL),
    filemenu_(NULL),
    windowmenu_(NULL),
    helpmenu_(NULL),
    filetoolbar_(NULL)
{
    // create central area where windows are created
    mdiarea_ = new QMdiArea;
    assert(mdiarea_ != NULL);

    // scrollbar policy
    mdiarea_->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    mdiarea_->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);

    // add mdi area to the current window
    setCentralWidget(mdiarea_);

    // we need to update the windows menu as subwindows appear
    // and disappear.
    connect(mdiarea_, SIGNAL(subWindowActivated(QMdiSubWindow *)),
            this, SLOT(updateMenus()));

    // collect signals and forward signals to a sink
    windowmapper_ = new QSignalMapper(this);
    connect(windowmapper_, SIGNAL(mapped(QWidget *)), 
            this, SLOT(setActiveSubWindow(QWidget *)));

    // create and crate all the widgets, gadgets and gidgets
    createButtons();
    createActions();
    createMenus();
    createToolBars();
    createStatusBar();
    updateMenus();

    // set default current location, etc.
    readSettings();

    // label window
    // setWindowIcon(QPixmap(":/images/middlefinger.png"));
    setWindowTitle(QString("MDI Tree Model Viewer"));

    // MAC stuff
    setUnifiedTitleAndToolBarOnMac(true);

    // sane object -- insane programmer !!!
    ok_ = true;
}

MdiMainWindow::~MdiMainWindow()
{
    // do nothing
}

// create the widgets, etc.
void
MdiMainWindow::createButtons()
{
    usedummymodelbutton_ = new QCheckBox(tr("Dummy Model"), this);
    connect(usedummymodelbutton_, SIGNAL(stateChanged(int)), this, SLOT(useDummyModel(int)));
}

void
MdiMainWindow::createActions()
{
    openaction_ = new QAction(tr("&Open File"), this);
    openaction_->setShortcuts(QKeySequence::Open);
    openaction_->setStatusTip(tr("Open an existing file"));
    connect(openaction_, SIGNAL(triggered()), this, SLOT(openFile()));

    exitaction_ = new QAction(tr("E&xit"), this);
    exitaction_->setShortcut(tr("Ctrl+Q"));
    exitaction_->setStatusTip(tr("Exit the application"));
    connect(exitaction_, SIGNAL(triggered()), qApp, SLOT(closeAllWindows()));

    closeactwindowaction_ = new QAction(tr("Cl&ose"), this);
    closeactwindowaction_->setShortcut(tr("Ctrl+F4"));
    closeactwindowaction_->setStatusTip(tr("Close the active window"));
    connect(closeactwindowaction_, SIGNAL(triggered()),
            mdiarea_, SLOT(closeActiveSubWindow()));

    closeallwindowsaction_ = new QAction(tr("Close &All"), this);
    closeallwindowsaction_->setStatusTip(tr("Close all the windows"));
    connect(closeallwindowsaction_, SIGNAL(triggered()),
            mdiarea_, SLOT(closeAllSubWindows()));

    tilewindowsaction_ = new QAction(tr("&Tile"), this);
    tilewindowsaction_->setStatusTip(tr("Tile the windows"));
    connect(tilewindowsaction_, SIGNAL(triggered()), 
            mdiarea_, SLOT(tileSubWindows()));

    cascadeaction_ = new QAction(tr("&Cascade"), this);
    cascadeaction_->setStatusTip(tr("Cascade the windows"));
    connect(cascadeaction_, SIGNAL(triggered()), 
            mdiarea_, SLOT(cascadeSubWindows()));

    nextwindowaction_ = new QAction(tr("Ne&xt"), this);
    nextwindowaction_->setShortcuts(QKeySequence::NextChild);
    nextwindowaction_->setStatusTip(tr("Move the focus to the next window"));
    connect(nextwindowaction_, SIGNAL(triggered()),
            mdiarea_, SLOT(activateNextSubWindow()));

    previouswindowaction_ = new QAction(tr("Pre&vious"), this);
    previouswindowaction_->setShortcuts(QKeySequence::PreviousChild);
    previouswindowaction_->setStatusTip(tr("Move the focus to the previous "
                                     "window"));
    connect(previouswindowaction_, SIGNAL(triggered()),
            mdiarea_, SLOT(activatePreviousSubWindow()));

    separatoraction_ = new QAction(this);
    separatoraction_->setSeparator(true);

    aboutaction_ = new QAction(tr("&About"), this);
    aboutaction_->setStatusTip(tr("Show the application's About box"));
    connect(aboutaction_, SIGNAL(triggered()), this, SLOT(about()));

    closeallaction_ = new QAction(tr("Collapse All Rows"), this);
    closeallaction_->setStatusTip(tr("Collapse all open rows"));
    connect(closeallaction_, SIGNAL(triggered()), this, SLOT(closeAll()));

    expandallaction_ = new QAction(tr("Expand All Rows"), this);
    expandallaction_->setStatusTip(tr("Expand all rows"));
    connect(expandallaction_, SIGNAL(triggered()), this, SLOT(expandAll()));

    enablereadonlyaction_ = new QAction(tr("Read-Only"), this);
    enablereadonlyaction_->setStatusTip(tr("View is read-only"));
    connect(enablereadonlyaction_, SIGNAL(triggered()), this, SLOT(enableReadOnly()));

    disablereadonlyaction_ = new QAction(tr("Read && Update"), this);
    disablereadonlyaction_->setStatusTip(tr("View is editable"));
    connect(disablereadonlyaction_, SIGNAL(triggered()), this, SLOT(disableReadOnly()));

    insertrowaction_ = new QAction(tr("Insert Row"), this);
    insertrowaction_->setStatusTip(tr("Insert a new row"));
    connect(insertrowaction_, SIGNAL(triggered()), this, SLOT(insertRow()));

    removerowaction_ = new QAction(tr("Remove Row"), this);
    removerowaction_->setStatusTip(tr("Remove a row"));
    connect(removerowaction_, SIGNAL(triggered()), this, SLOT(removeRow()));

    insertcolumnaction_ = new QAction(tr("Insert Column"), this);
    insertcolumnaction_->setStatusTip(tr("Insert a new column"));
    connect(insertcolumnaction_, SIGNAL(triggered()), this, SLOT(insertColumn()));

    removecolumnaction_ = new QAction(tr("Remove Column"), this);
    removecolumnaction_->setStatusTip(tr("Remove a column"));
    connect(removecolumnaction_, SIGNAL(triggered()), this, SLOT(removeColumn()));

    insertchildaction_ = new QAction(tr("Insert Child"), this);
    insertchildaction_->setStatusTip(tr("Insert a new child"));
    connect(insertchildaction_, SIGNAL(triggered()), this, SLOT(insertChild()));
}

void
MdiMainWindow::createMenus()
{
    filemenu_ = menuBar()->addMenu(tr("&File"));
    filemenu_->addAction(openaction_);
    filemenu_->addSeparator();
    filemenu_->addAction(exitaction_);

    windowmenu_ = menuBar()->addMenu(tr("&Window"));
    updateWindowMenu();
    connect(windowmenu_, SIGNAL(aboutToShow()), 
            this, SLOT(updateWindowMenu()));

    menuBar()->addSeparator();

    helpmenu_ = menuBar()->addMenu(tr("&Help"));
    helpmenu_->addAction(aboutaction_);
}

void
MdiMainWindow::createToolBars()
{
    filetoolbar_ = addToolBar(tr("File"));
    filetoolbar_->addAction(openaction_);
    buttontoolbar_ = addToolBar(tr("Dummy Model"));
    buttontoolbar_->addWidget(usedummymodelbutton_);
}

void
MdiMainWindow::createStatusBar()
{
    statusBar()->showMessage(tr("Ready"));
}

void
MdiMainWindow::readSettings()
{
    QSettings settings("Prime Analytics", "MDI Tree Viewer");
    QPoint pos = settings.value("pos", QPoint(200, 200)).toPoint();
    QSize size = settings.value("size", QSize(400, 400)).toSize();

    move(pos);
    resize(size);
}

void
MdiMainWindow::writeSettings()
{
    QSettings settings("Prime Analytics", "MDI Tree Viewer");
    settings.setValue("pos", pos());
    settings.setValue("size", size());
}

// mdi window maintenance 
MdiTreeView *
MdiMainWindow::activeMdiChild()
{
    QMdiSubWindow *activeSubWindow = mdiarea_->activeSubWindow();
    if (activeSubWindow)
        return(qobject_cast<MdiTreeView *>(activeSubWindow->widget()));
    else
        return(NULL);
}

QMdiSubWindow *
MdiMainWindow::findMdiChild(const QString &filename)
{
    QString canonicalFilePath = QFileInfo(filename).canonicalFilePath();

    // Qt for-loop macro -- do it the Qt way.
    foreach (QMdiSubWindow *window, mdiarea_->subWindowList())
    {
        MdiTreeView *mdiChild = qobject_cast<MdiTreeView *>(window->widget());
        if (mdiChild->currentFile() == canonicalFilePath) return(window);
    }

    // not found
    return(NULL);
}

void
MdiMainWindow::setActiveSubWindow(QWidget *window)
{
    if (!window) return;
    mdiarea_->setActiveSubWindow(qobject_cast<QMdiSubWindow *>(window));
}

// slot callbacks
void
MdiMainWindow::useDummyModel(int state)
{
    usedummymodel_ = (state == Qt::Checked);
    if (usedummymodel_)
    {
        std::cout << "USING DUMMY MODEL" << std::endl;
    }
    else
    {
        std::cout << "NOT USING DUMMY MODEL" << std::endl;
    }
}

void
MdiMainWindow::closeEvent(QCloseEvent *event)
{
    mdiarea_->closeAllSubWindows();
    if (activeMdiChild())
    {
        event->ignore();
    }
    else
    {
        writeSettings();
        event->accept();
    }
}

void
MdiMainWindow::closeAll()
{
    MdiTreeView *active_child = activeMdiChild();
    if (active_child != NULL)
    {
        active_child->collapseAll();
        active_child->show();
    }
}

void
MdiMainWindow::expandAll()
{
    MdiTreeView *active_child = activeMdiChild();
    if (active_child != NULL)
    {
        active_child->expandAll();
        active_child->show();
    }
}

void
MdiMainWindow::enableReadOnly()
{
    MdiTreeView *active_child = activeMdiChild();
    if (active_child != NULL)
    {
        active_child->setReadOnly(true);
        updateMenus();
        active_child->show();
    }
}

void
MdiMainWindow::disableReadOnly()
{
    MdiTreeView *active_child = activeMdiChild();
    if (active_child != NULL)
    {
        active_child->setReadOnly(false);
        updateMenus();
        active_child->show();
    }
}

void
MdiMainWindow::insertRow()
{
    MdiTreeView *view = activeMdiChild();
    if (view == NULL) return;
    if (view->getReadOnly()) return;

    QModelIndex index = view->selectionModel()->currentIndex();
    QAbstractItemModel *model = view->model();

    if (!model->insertRow(index.row()+1, index.parent())) return;

    updateActions();

    for (int column = 0; 
         column < model->columnCount(index.parent()); 
       ++column)
    {
        QModelIndex child = model->index(index.row()+1, column, index.parent());
        model->setData(child, QVariant("[No data]"), Qt::EditRole);
    }
}

void
MdiMainWindow::removeRow()
{
    MdiTreeView *view = activeMdiChild();
    if (view == NULL) return;

    QModelIndex index = view->selectionModel()->currentIndex();
    QAbstractItemModel *model = view->model();
    if (model->removeRow(index.row(), index.parent())) updateActions();
}

bool
MdiMainWindow::insertColumn(const QModelIndex &parent)
{
    MdiTreeView *view = activeMdiChild();
    if (view == NULL) return(false);
    if (view->getReadOnly()) return(false);

    QAbstractItemModel *model = view->model();
    int column = view->selectionModel()->currentIndex().column();

    // Insert a column in the parent item.
    bool changed = model->insertColumn(column + 1, parent);
    if (changed)
        model->setHeaderData(column + 1, Qt::Horizontal, 
                             QVariant("[No header]"), Qt::EditRole);

    updateActions();

    return(changed);
}

bool
MdiMainWindow::removeColumn(const QModelIndex &parent)
{
    MdiTreeView *view = activeMdiChild();
    if (view == NULL) return(false);
    if (view->getReadOnly()) return(false);

    QAbstractItemModel *model = view->model();
    int column = view->selectionModel()->currentIndex().column();

    // Insert columns in each child of the parent item.
    bool changed = model->removeColumn(column, parent);

    if (!parent.isValid() && changed) updateActions();

    return(changed);
}

void
MdiMainWindow::insertChild()
{
    MdiTreeView *view = activeMdiChild();
    if (view == NULL) return;
    if (view->getReadOnly()) return;

    QModelIndex index = view->selectionModel()->currentIndex();
    QAbstractItemModel *model = view->model();

    if (model->columnCount(index) == 0)
    {
        if (!model->insertColumn(0, index)) return;
    }

    if (!model->insertRow(0, index)) return;

    for (int column = 0; column < model->columnCount(index); ++column)
    {
        QModelIndex child = model->index(0, column, index);
        model->setData(child, QVariant("[No data]"), Qt::EditRole);
        if (!model->headerData(column, Qt::Horizontal).isValid())
            model->setHeaderData(column, 
                                 Qt::Horizontal, 
                                 QVariant("[No header]"),
                                 Qt::EditRole);
    }

    view->selectionModel()->setCurrentIndex(model->index(0, 0, index),
                                            QItemSelectionModel::ClearAndSelect);
    updateActions();
}

void
MdiMainWindow::openFile()
{
    QString filename = QFileDialog::getOpenFileName(this);
    if (!filename.isEmpty())
    {
        QMdiSubWindow *existing = findMdiChild(filename);
        if (existing)
        {
            mdiarea_->setActiveSubWindow(existing);
            return;
        }

        MdiTreeView *child = createMdiChild();
        if (child->loadFile(filename, usedummymodel_) == 0)
        {
            // we have a model at this point. if the model
            // changed, then we want the ui to update.
            connect(child->selectionModel(),
            SIGNAL(selectionChanged(const QItemSelection &,
                                    const QItemSelection &)),
                   this, SLOT(updateActions()));
            statusBar()->showMessage(tr("Data loaded"), 2000);
            child->show();
        }
        else
        {
            child->close();
        }
    }
}

void
MdiMainWindow::about()
{
    QMessageBox::about(this, 
        tr("About MDI Tree Model Viewer"),
        tr("Tree model viewer and prototype."));
}

// enable/disable menu items depending if a subwindow exists.
void
MdiMainWindow::updateMenus()
{
    bool hasMdiChild = (activeMdiChild() != 0);
    bool readonly = (hasMdiChild) ? activeMdiChild()->getReadOnly() : false;

    closeactwindowaction_->setEnabled(hasMdiChild);
    closeallwindowsaction_->setEnabled(hasMdiChild);
    tilewindowsaction_->setEnabled(hasMdiChild);
    cascadeaction_->setEnabled(hasMdiChild);
    nextwindowaction_->setEnabled(hasMdiChild);
    previouswindowaction_->setEnabled(hasMdiChild);
    separatoraction_->setVisible(hasMdiChild);
    closeallaction_->setVisible(hasMdiChild);
    expandallaction_->setVisible(hasMdiChild);
    enablereadonlyaction_->setVisible(hasMdiChild);
    disablereadonlyaction_->setVisible(hasMdiChild);
    insertrowaction_->setVisible(hasMdiChild && !readonly);
    removerowaction_->setVisible(hasMdiChild && !readonly);
    insertcolumnaction_->setVisible(hasMdiChild && !readonly);
    removecolumnaction_->setVisible(hasMdiChild && !readonly);
    insertchildaction_->setVisible(hasMdiChild && !readonly);
}

void
MdiMainWindow::updateWindowMenu()
{
    windowmenu_->clear();
    windowmenu_->addAction(closeactwindowaction_);
    windowmenu_->addAction(closeallwindowsaction_);
    windowmenu_->addSeparator();
    windowmenu_->addAction(closeallaction_);
    windowmenu_->addAction(expandallaction_);
    windowmenu_->addAction(enablereadonlyaction_);
    windowmenu_->addAction(disablereadonlyaction_);
    windowmenu_->addAction(insertrowaction_);
    windowmenu_->addAction(removerowaction_);
    windowmenu_->addAction(insertcolumnaction_);
    windowmenu_->addAction(removecolumnaction_);
    windowmenu_->addAction(insertchildaction_);

    windowmenu_->addSeparator();
    windowmenu_->addAction(tilewindowsaction_);
    windowmenu_->addAction(cascadeaction_);
    windowmenu_->addSeparator();
    windowmenu_->addAction(nextwindowaction_);
    windowmenu_->addAction(previouswindowaction_);
    windowmenu_->addAction(separatoraction_);

    QList<QMdiSubWindow *> windows = mdiarea_->subWindowList();
    separatoraction_->setVisible(!windows.isEmpty());

    for (int i = 0; i < windows.size(); ++i)
    {
        MdiTreeView *child = qobject_cast<MdiTreeView *>(windows.at(i)->widget());

        QString text;
        if (i < 9)
        {
            text = tr("&%1 %2").arg(i + 1)
                               .arg(child->userFriendlyCurrentFile());
        }
        else
        {
            text = tr("%1 %2").arg(i + 1)
                              .arg(child->userFriendlyCurrentFile());
        }
        QAction *action  = windowmenu_->addAction(text);
        action->setCheckable(true);
        action->setChecked(child == activeMdiChild());
        connect(action, SIGNAL(triggered()), windowmapper_, SLOT(map()));
        windowmapper_->setMapping(action, windows.at(i));
    }
}

MdiTreeView *
MdiMainWindow::createMdiChild()
{
    MdiTreeView *child = new MdiTreeView;
    mdiarea_->addSubWindow(child);
    return child;
}

// update view after any changes
void
MdiMainWindow::updateActions()
{
    MdiTreeView *view = activeMdiChild();
    if (view == NULL) return;

    bool hasSelection = !view->selectionModel()->selection().isEmpty();
    removerowaction_->setEnabled(hasSelection);
    removecolumnaction_->setEnabled(hasSelection);

    bool hasCurrent = view->selectionModel()->currentIndex().isValid();
    insertrowaction_->setEnabled(hasCurrent);
    insertcolumnaction_->setEnabled(hasCurrent);

    if (hasCurrent)
    {
        view->closePersistentEditor(view->selectionModel()->currentIndex());

        int row = view->selectionModel()->currentIndex().row();
        int column = view->selectionModel()->currentIndex().column();
        if (view->selectionModel()->currentIndex().parent().isValid())
            statusBar()->showMessage(tr("Position: (%1,%2)").arg(row).arg(column));
        else
            statusBar()->showMessage(tr("Position: (%1,%2) in top level").arg(row).arg(column));
    }
}

