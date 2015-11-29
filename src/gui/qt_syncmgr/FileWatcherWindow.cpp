// file watcher window

// headers
#include "FileWatcherWindow.h"

static const int EightySpaces = 80;

// ctors and dtor
FileWatcherWindow::FileWatcherWindow():
    mainboxlayout_(NULL), 
    buttonboxlayout_(NULL), 
    onetextline_(NULL), 
    watchbutton_(NULL),  
    messages_(NULL),
    counter_(0),
    watcheddirs_(),
    das_()
{
    // set basic attributes
    setAttribute(Qt::WA_DeleteOnClose);

    // layouts
    mainboxlayout_ = new QVBoxLayout;
    buttonboxlayout_ = new QHBoxLayout;
    watchedlistbuttonboxlayout_ = new QHBoxLayout;

    // create text line and watch/unwatch buttons
    onetextline_ = new QLineEdit;
    QFontMetrics fontmetrics(onetextline_->font());
    onetextline_->setMinimumWidth(EightySpaces*fontmetrics.width(" "));

    cleartextbutton_ = new QPushButton("Clear");
    filebrowserbutton_ = new QPushButton("Browse");
    watchbutton_ = new QPushButton("Watch");
    onetextlinelabel_ = new QLabel("Enter Directory", this);

    buttonboxlayout_->addWidget(onetextlinelabel_);
    buttonboxlayout_->addWidget(onetextline_);
    buttonboxlayout_->addWidget(cleartextbutton_);
    buttonboxlayout_->addWidget(filebrowserbutton_);
    buttonboxlayout_->addWidget(watchbutton_);
    mainboxlayout_->addLayout(buttonboxlayout_, true);

    // connect callbacks to widgets
    connect(cleartextbutton_, SIGNAL(clicked()), this, SLOT(onClearTextButtonClicked()));
    connect(filebrowserbutton_, SIGNAL(clicked()), this, SLOT(onFileBrowserButtonClicked()));
    connect(watchbutton_, SIGNAL(clicked()), this, SLOT(onWatchButtonClicked()));

    // connect das callbacks to window signals
    connect(this, SIGNAL(addDirectoryWatch(const QString &, const QString &)), 
           &das_, SLOT(onAddDirectoryWatch(const QString &, const QString &)));
    connect(this, SIGNAL(removeDirectoryWatch(const QString &, const QString &)), 
           &das_, SLOT(onRemoveDirectoryWatch(const QString &, const QString &)));

    // connect window call backs to das signals
    connect(&das_, SIGNAL(statusMessage(const QString &)), 
            this, SLOT(onStatusMessage(const QString &)));

    // list of watched directories
    watchedlist_ = new QListWidget;
    mainboxlayout_->addWidget(watchedlist_);
    watchedlistlabel_ = new QLabel("Choose list item", this);
    watchedlistunwatchbutton_ = new QPushButton("Un-Watch");
    watchedlistspacer_ = new QSpacerItem(1, 1, QSizePolicy::Expanding);
    watchedlistbuttonboxlayout_->addWidget(watchedlistlabel_);
    watchedlistbuttonboxlayout_->addWidget(watchedlistunwatchbutton_);
    watchedlistbuttonboxlayout_->addSpacerItem(watchedlistspacer_);
    mainboxlayout_->addLayout(watchedlistbuttonboxlayout_);

    // call-backs for watched list buttons
    connect(watchedlistunwatchbutton_, SIGNAL(clicked()), 
            this, SLOT(onWatchedListUnWatchButtonClicked()));

    // create text area for status msgs
    messages_ = new QTextEdit;
    messages_->setReadOnly(true);
    mainboxlayout_->addWidget(messages_);

    // display on sub window
    delete layout();
    setLayout(mainboxlayout_);

    // label window
    setWindowTitle(QString("DAS Viewer"));
}

FileWatcherWindow::~FileWatcherWindow()
{
}

// callbacks
void
FileWatcherWindow::closeEvent(QCloseEvent *event)
{
    if (event) event->accept();
}

void
FileWatcherWindow::onClearTextButtonClicked()
{
    onetextline_->clear();
}

void
FileWatcherWindow::onFileBrowserButtonClicked()
{
    QFileDialog dirdialog(this);
    dirdialog.setFileMode(QFileDialog::Directory);
    dirdialog.setOption(QFileDialog::ShowDirsOnly, true);
    dirdialog.setViewMode(QFileDialog::Detail);
    if (!dirdialog.exec()) return;

    QStringList directorynames;
    directorynames = dirdialog.selectedFiles();
    if (directorynames.isEmpty()) return;

    QStringList::Iterator sit = directorynames.begin();
    QStringList::Iterator sitend = directorynames.end();
    if (sit == sitend) return;

    messages_->append(QString("Choosing directory ") + *sit);
    onetextline_->setText(*sit);
}

void
FileWatcherWindow::onWatchButtonClicked()
{
    QString watcheddir = onetextline_->text();
    if (watcheddir == "")
    {
        messages_->append(QString("Watched directory is null-lengthed"));
        return;
    }
    QString filenode = QString("FILENODE%1").arg(++counter_);
    messages_->append(QString("%1: Adding a watch to %2").arg(filenode).arg(watcheddir));
    if (watcheddirs_.find(watcheddir) != watcheddirs_.end())
    {
        messages_->append(QString("Directory %1 ALREADY found in Watched list").arg(watcheddir));
        return;
    }
    watcheddirs_.insert(watcheddir, filenode);
    watchedlist_->addItem(QString("%1:%2").arg(filenode).arg(watcheddir));

    emit(addDirectoryWatch(filenode, watcheddir));
}

void
FileWatcherWindow::onWatchedListUnWatchButtonClicked()
{
    QList<QListWidgetItem *> selecteditems = watchedlist_->selectedItems();
    if (selecteditems.isEmpty())
    {
        messages_->append(QString("No items selected from watched list"));
        return;
    }
    QList<QListWidgetItem *>::iterator sit = selecteditems.begin();
    QList<QListWidgetItem *>::iterator sitend = selecteditems.end();
    for ( ; sit != sitend; ++sit)
    {
        QString watcheddir = (*sit)->text();
        if (watcheddir == "")
        {
            messages_->append(QString("Watched directory is null-lengthed. Skip and continue."));
            continue;
        }
        messages_->append(QString("Removing watch from ") + watcheddir);
        QList<QListWidgetItem *> watcheditems = 
            watchedlist_->findItems(watcheddir, Qt::MatchExactly);
        if (watcheditems.isEmpty())
        {
            messages_->append(QString("Watched directory NOT found in Watched list"));
            return;
        }
        QList<QListWidgetItem*>::Iterator wit = watcheditems.begin();
        QList<QListWidgetItem*>::Iterator witend = watcheditems.end();
        for ( ; wit != witend; ++wit)
        {
            int row = watchedlist_->row(*wit);
            watchedlist_->takeItem(row);
        }
    
        QStringList list = watcheddir.split(":");
        QString filenode = list.front();
        list.pop_front();
        watcheddir = list.front();
        list.pop_front();

        watcheddirs_.remove(watcheddir);

        emit(removeDirectoryWatch(filenode, watcheddir));
    }
}

void
FileWatcherWindow::onStatusMessage(const QString &message)
{
    QCRITDUMP(message);
    messages_->append(message);
}

