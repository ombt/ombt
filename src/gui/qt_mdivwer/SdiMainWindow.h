#ifndef __SDI_MAIN_WINDOW_H
#define __SDI_MAIN_WINDOW_H

// headers
#include <string>
#include <QMainWindow>
#include <QtGui>

// forward declarations
class TreeModel;
class QTreeView;

// single document main window
class SdiMainWindow: public QMainWindow
{
    Q_OBJECT

public:
#if 0
    SdiMainWindow(const std::string &filename);
#else
    SdiMainWindow();
#endif
    ~SdiMainWindow();

private slots:
    void openFile();
    void closeAll();
    void expandAll();

protected:
    // utilities
    void createActions();
    void createMenus();
    void createToolBars();
    void createStatusBar();

protected:
    // data
    bool ok_;

    TreeModel *treemodel_;
    QTreeView *treeview_;

    QAction *openaction_;
    QAction *exitaction_;
    QAction *expandallaction_;
    QAction *closeallaction_;

    QMenu *filemenu_;
};

#endif
