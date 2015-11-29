#ifndef __MDITREEVIEW_H
#define __MDITREEVIEW_H

// tree-view wrapper to work with mdi

// headers
#include <QtGui>

// forward declarations
class AbstractItemModelAdaptor;

// mdi tree-view wrapper
class MdiTreeView: public QTreeView
{
    Q_OBJECT

public:
    // ctor and dtor
    MdiTreeView();
    ~MdiTreeView();

    // actions
    int loadFile(const QString &filename, bool usedummy = false);
    QString userFriendlyCurrentFile();
    QString currentFile() { return(currentfile_); }
    void setReadOnly(bool rd);
    bool getReadOnly() const;

protected:
    // utilities
    void closeEvent(QCloseEvent *event);
    void setCurrentFile(const QString &filename);
    QString strippedName(const QString &fullfilename);

    // data
    AbstractItemModelAdaptor *treemodel_;
    QString currentfile_;
    bool isuntitled_;
};

#endif
