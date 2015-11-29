// file watcher window

// headers
#include <assert.h>
#include <iostream>

#include "DASQDebug.h"
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
    filemgr_(),
    dasdbmgr_(),
    synchmgr_(dasdbmgr_),
    synchmgrthread_(synchmgr_),
    monitor_(dasdbmgr_, synchmgr_)
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

    // connect file monitor callbacks to window signals
    connect(this, SIGNAL(addDirectoryWatch(const QString &, const QString &)), 
           &monitor_, SLOT(onAddDirectoryWatch(const QString &, const QString &)));
    connect(this, SIGNAL(removeDirectoryWatch(const QString &, const QString &)), 
           &monitor_, SLOT(onRemoveDirectoryWatch(const QString &, const QString &)));

    // connect window call backs to file monitor signals
    connect(&monitor_, SIGNAL(fileCreated(const QString &)), 
            this, SLOT(onFileCreated(const QString &)));
    connect(&monitor_, SIGNAL(directoryCreated(const QString &)), 
            this, SLOT(onDirectoryCreated(const QString &)));
    connect(&monitor_, SIGNAL(fileDeleted(const QString &)), 
            this, SLOT(onFileDeleted(const QString &)));
    connect(&monitor_, SIGNAL(directoryDeleted(const QString &)), 
            this, SLOT(onDirectoryDeleted(const QString &)));
    connect(&monitor_, SIGNAL(fileChanged(const QString &)), 
            this, SLOT(onFileChanged(const QString &)));
    connect(&monitor_, SIGNAL(directoryChanged(const QString &)), 
            this, SLOT(onDirectoryChanged(const QString &)));
    connect(&monitor_, SIGNAL(fileRenamed(const QString &, const QString &)), 
            this, SLOT(onFileRenamed(const QString &, const QString &)));
    connect(&monitor_, SIGNAL(directoryRenamed(const QString &, const QString &)), 
            this, SLOT(onDirectoryRenamed(const QString &, const QString &)));
    connect(&monitor_, SIGNAL(exception(const QString &, const QString &)), 
            this, SLOT(onException(const QString &, const QString &)));

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
    setWindowTitle(QString("File Watcher Viewer"));

    // start up the DAS synch mgr thread
    synchmgrthread_.start();
}

FileWatcherWindow::~FileWatcherWindow()
{
    synchmgr_.getMsgQueue().done();
    synchmgrthread_.wait();
}

// callbacks
void
FileWatcherWindow::closeEvent(QCloseEvent *event)
{
    QTRACE();
    if (event) event->accept();
}

void
FileWatcherWindow::onClearTextButtonClicked()
{
    QTRACE();
    onetextline_->clear();
}

void
FileWatcherWindow::onFileBrowserButtonClicked()
{
    QTRACE();

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
    QTRACE();
    QString watcheddir = onetextline_->text();
    if (watcheddir == "")
    {
        messages_->append(QString("Watched directory is null-lengthed"));
        return;
    }
    QString filenode = QString("FILENODE%1").arg(++counter_);
    messages_->append(QString("%1: Adding a watch to %2").arg(filenode).arg(watcheddir));
    if (!watchedlist_->findItems(watcheddir, Qt::MatchExactly).isEmpty())
    {
        messages_->append(QString("Watched directory ALREADY found in Watched list"));
        return;
    }
    watchedlist_->addItem(QString("%1:%2").arg(filenode).arg(watcheddir));

    emit(addDirectoryWatch(filenode, watcheddir));
}

void
FileWatcherWindow::onWatchedListUnWatchButtonClicked()
{
    QTRACE();
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

        emit(removeDirectoryWatch(filenode, watcheddir));
    }
}

void
FileWatcherWindow::onFileCreated(const QString &path)
{
    messages_->append(QString("File created: ") + path);
}

void
FileWatcherWindow::onDirectoryCreated(const QString &path)
{
    messages_->append(QString("Directory created: ") + path);
}

void
FileWatcherWindow::onFileDeleted(const QString &path)
{
    messages_->append(QString("File deleted: ") + path);
}

void
FileWatcherWindow::onDirectoryDeleted(const QString &path)
{
    messages_->append(QString("Directory deleted: ") + path);
}

void
FileWatcherWindow::onFileChanged(const QString &path)
{
    messages_->append(QString("File changed: ") + path);
}

void
FileWatcherWindow::onDirectoryChanged(const QString &path)
{
    messages_->append(QString("Directory changed: ") + path);
}

void
FileWatcherWindow::onFileRenamed(const QString &oldpath, const QString &newpath)
{
    messages_->append(QString("File renamed: ") + oldpath + " ==>> " + newpath);
}

void
FileWatcherWindow::onDirectoryRenamed(const QString &oldpath, const QString &newpath)
{
    messages_->append(QString("Directory renamed: ") + oldpath + " ==>> " + newpath);
}

void
FileWatcherWindow::onException(const QString &description, const QString &path)
{
    messages_->append(QString("EXCEPTION: ") + description + " Path: " + path);
}

