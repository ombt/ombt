#ifndef __MDI_MAIN_WINDOW_H
#define __MDI_MAIN_WINDOW_H

// headers
#include <string>
#include <QApplication>
#include <QMainWindow>
#include <QMdiArea>
#include <QAction>
#include <QMenu>
#include <QMenuBar>
#include <QStatusBar>
#include <QToolBar>
#include <QMessageBox>
#include <QFileDialog>
#include <QMdiSubWindow>

#include "RouterStorageMgr.h"
#include "FileMgr.h"

// single document main window
class MdiMainWindow: public QMainWindow
{
    Q_OBJECT

public:
    MdiMainWindow();
    ~MdiMainWindow();

private slots:
    void updateAll();
    void updateWindowMenu();
    void about();
    void newFileWatcher();

protected:
    void createActions();
    void createButtons();
    void createMenus();
    void createToolBars();
    void createStatusBar();

protected:
    QMdiArea *mdiarea_;

    QAction *newaction_;
    QAction *exitaction_;
    QAction *closeactwindowaction_;
    QAction *closeallwindowsaction_;
    QAction *tilewindowsaction_;
    QAction *cascadewindowsaction_;
    QAction *nextwindowaction_;
    QAction *previouswindowaction_;
    QAction *separatoraction_;
    QAction *aboutaction_;

    QMenu *filemenu_;
    QMenu *windowmenu_;
    QMenu *helpmenu_;

    QToolBar *filetoolbar_;
    QToolBar *buttontoolbar_;

    FileMgr filemgr_;
    RouterStorageMgr storagemgr_;
};

#endif
