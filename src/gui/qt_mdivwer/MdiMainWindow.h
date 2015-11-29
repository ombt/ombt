#ifndef __MDI_MAIN_WINDOW_H
#define __MDI_MAIN_WINDOW_H

// headers
#include <string>
#include <QMainWindow>
#include <QModelIndex>

// forward declarations
class MdiTreeView;
class QTreeView;
class QAction;
class QMenu;
class QMdiArea;
class QMdiSubWindow;
class QSignalMapper;
class QCheckBox;

// single document main window
class MdiMainWindow: public QMainWindow
{
    Q_OBJECT

public:
    MdiMainWindow();
    ~MdiMainWindow();

protected:
    void closeEvent(QCloseEvent *event);

private slots:
    void openFile();
    MdiTreeView *createMdiChild();
    void setActiveSubWindow(QWidget *window);
    void updateWindowMenu();
    void updateMenus();
    void about();
    void expandAll();
    void closeAll();
    void enableReadOnly();
    void disableReadOnly();
    void insertRow();
    void removeRow();
    bool insertColumn(const QModelIndex &parent = QModelIndex());
    bool removeColumn(const QModelIndex &parent = QModelIndex());
    void insertChild();
    void updateActions();
    void useDummyModel(int state);

protected:
    void createActions();
    void createButtons();
    void createMenus();
    void createToolBars();
    void createStatusBar();
    void readSettings();
    void writeSettings();
    MdiTreeView *activeMdiChild();
    QMdiSubWindow *findMdiChild(const QString &childname);

protected:
    bool ok_;
    bool usedummymodel_;

    QMdiArea *mdiarea_;
    QSignalMapper *windowmapper_;

    QCheckBox *usedummymodelbutton_;

    QAction *openaction_;
    QAction *exitaction_;
    QAction *closeactwindowaction_;
    QAction *closeallwindowsaction_;
    QAction *tilewindowsaction_;
    QAction *cascadeaction_;
    QAction *nextwindowaction_;
    QAction *previouswindowaction_;
    QAction *separatoraction_;
    QAction *aboutaction_;
    QAction *closeallaction_;
    QAction *expandallaction_;
    QAction *enablereadonlyaction_;
    QAction *disablereadonlyaction_;
    QAction *insertrowaction_;
    QAction *removerowaction_;
    QAction *insertcolumnaction_;
    QAction *removecolumnaction_;
    QAction *insertchildaction_;
    QAction *usedummymodelaction_;

    QMenu *filemenu_;
    QMenu *windowmenu_;
    QMenu *helpmenu_;

    QToolBar *filetoolbar_;
    QToolBar *buttontoolbar_;
};

#endif
