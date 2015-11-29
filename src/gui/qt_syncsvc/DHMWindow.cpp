// DHM window

// headers
#include "DHMWindow.h"

// ctors and dtor
DHMWindow::DHMWindow():
    mainboxlayout_(NULL), 
    daslistlabel_(NULL), 
    daslist_(NULL), 
    dasset_(),
    dasfnset_()
{
    // set basic attributes
    setAttribute(Qt::WA_DeleteOnClose);

    // layouts
    mainboxlayout_ = new QVBoxLayout;

    // das list 
    daslistlabel_ = new QLabel("DAS List", this);
    QFontMetrics fontmetrics(daslistlabel_->font());
    mainboxlayout_->addWidget(daslistlabel_);
    daslist_ = new QListWidget;
    daslist_->setMinimumSize(
        40*fontmetrics.width("M"), 5*fontmetrics.height());
    mainboxlayout_->addWidget(daslist_);

    // das file node list 
    dasfnlistlabel_ = new QLabel("DAS File Node List", this);
    mainboxlayout_->addWidget(dasfnlistlabel_);
    dasfnlist_ = new QListWidget;
    dasfnlist_->setMinimumSize(
        40*fontmetrics.width("M"), 5*fontmetrics.height());
    mainboxlayout_->addWidget(dasfnlist_);

    // status messages
    messages_ = new QTextEdit;
    messages_->setReadOnly(true);
    messages_->setMinimumSize(
        40*fontmetrics.width("M"), 5*fontmetrics.height());
    mainboxlayout_->addWidget(messages_);

    // display on sub window
    delete layout();
    setLayout(mainboxlayout_);

    // label window
    setWindowTitle(QString("DHM Viewer"));
}

DHMWindow::~DHMWindow()
{
}

// callbacks
void
DHMWindow::closeEvent(QCloseEvent *event)
{
    if (event) event->accept();
}

void
DHMWindow::onAddDAS(const QString &das)
{
    QDUMP(das);
    if (dasset_.find(das) != dasset_.end())
    {
        messages_->append(QString("%1 found in DHM list").arg(das));
        return;
    }
    daslist_->addItem(das);
    dasset_.insert(das);
    messages_->append(QString("%1 inserted into DHM list").arg(das));
}

void
DHMWindow::onRemoveDAS(const QString &das)
{
    QDUMP(das);
    if (dasset_.find(das) == dasset_.end())
    {
        messages_->append(QString("%1 NOT found in DHM list").arg(das));
        return;
    }
    QList<QListWidgetItem*> removelist = daslist_->findItems(das, Qt::MatchExactly);
    QList<QListWidgetItem*>::Iterator rit = removelist.begin();
    QList<QListWidgetItem*>::Iterator ritend = removelist.end();
    for ( ; rit != ritend; ++rit)
    {
        int row = daslist_->row(*rit);
        daslist_->takeItem(row);
        dasset_.remove(das);
    }
    messages_->append(QString("%1 removed from DHM list").arg(das));
}

void
DHMWindow::onAddFileNodeToDAS(const QString &das, const QString &filenode)
{
    QDUMP2(das, filenode);
    QString dasfn(QString("%1:%2").arg(das).arg(filenode));
    if (dasfnset_.find(dasfn) != dasfnset_.end())
    {
        messages_->append(QString("%1 found in DHM list").arg(dasfn));
        return;
    }
    dasfnlist_->addItem(dasfn);
    dasfnset_.insert(dasfn);
    messages_->append(QString("%1 inserted into DHM list").arg(dasfn));
}

void
DHMWindow::onRemoveFileNodeToDAS(const QString &das, const QString &filenode)
{
    QDUMP2(das, filenode);
    QString dasfn(QString("%1:%2").arg(das).arg(filenode));
    if (dasfnset_.find(dasfn) == dasfnset_.end())
    {
        messages_->append(QString("%1 NOT found in DHM list").arg(dasfn));
        return;
    }
    QList<QListWidgetItem*> removelist = dasfnlist_->findItems(dasfn, Qt::MatchExactly);
    QList<QListWidgetItem*>::Iterator rit = removelist.begin();
    QList<QListWidgetItem*>::Iterator ritend = removelist.end();
    for ( ; rit != ritend; ++rit)
    {
        int row = dasfnlist_->row(*rit);
        dasfnlist_->takeItem(row);
        dasfnset_.remove(dasfn);
    }
    messages_->append(QString("%1 removed from DHM list").arg(dasfn));
}

void
DHMWindow::onStatusMessage(const QString &message)
{
    QDUMP(message);
}

