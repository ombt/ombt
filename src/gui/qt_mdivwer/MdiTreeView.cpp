// tree-view wrapper for mdi

// headers
#include <assert.h>

#include "MdiTreeView.h"
#include "TreeModel.h"
#include "DummyTreeModel.h"

// not many functions to do
MdiTreeView::MdiTreeView():
    treemodel_(NULL), currentfile_(), isuntitled_(false)
{
    setAttribute(Qt::WA_DeleteOnClose);
}

MdiTreeView::~MdiTreeView()
{
    if (treemodel_ != NULL)
    {
        delete treemodel_;
        treemodel_ = NULL;
    }
}

// actions
int
MdiTreeView::loadFile(const QString &filename, bool usedummy)
{
    if (filename.isEmpty()) return(1);

    AbstractItemModelAdaptor *newtreemodel = NULL;
    if (usedummy)
        newtreemodel = new DummyTreeModel(filename.toStdString());
    else
        newtreemodel = new TreeModel(filename.toStdString());
    assert(newtreemodel != NULL);

    if ( ! newtreemodel->isOk())
    {
        delete newtreemodel;
        return(1);
    }


    hide();
    setModel(newtreemodel);
    show();

    if (treemodel_ != NULL) delete treemodel_;

    treemodel_ = newtreemodel;
    setCurrentFile(filename);
    return(0);
}

QString
MdiTreeView::userFriendlyCurrentFile()
{
    return strippedName(currentfile_);
}

void
MdiTreeView::closeEvent(QCloseEvent *event)
{
    event->accept();
}

void
MdiTreeView::setCurrentFile(const QString &fileName)
{
    currentfile_ = QFileInfo(fileName).canonicalFilePath();
    isuntitled_ = false;
    setWindowTitle(userFriendlyCurrentFile() + "[*]");
}

QString
MdiTreeView::strippedName(const QString &fullFileName)
{
    return QFileInfo(fullFileName).fileName();
}

// set/get readonly flag
void 
MdiTreeView::setReadOnly(bool rd) {
    if (treemodel_ != NULL) {
        treemodel_->setReadOnly(rd);
    }
}

bool 
MdiTreeView::getReadOnly() const {
   if (treemodel_ != NULL)
      return(treemodel_->getReadOnly());
   else
      return(false);
}

