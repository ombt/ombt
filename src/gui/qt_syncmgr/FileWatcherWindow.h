#ifndef __FILE_WATCHER_WINDOW_H
#define __FILE_WATCHER_WINDOW_H

// headers
#include <string>
#include <QMdiSubWindow>
#include <QStringList>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QTextEdit>
#include <QLabel>
#include <QFileDialog>
#include <QFontMetrics>
#include <QListWidget>
#include <QCloseEvent>
#include <QSpacerItem>
#include <QHash>

#include "DASQDebug.h"
#include "DAS.h"

// file watcher sub window
class FileWatcherWindow: public QMdiSubWindow
{
    Q_OBJECT

public:
    FileWatcherWindow();
    ~FileWatcherWindow();

signals:
    void addDirectoryWatch(const QString &filenode, const QString &path);
    void removeDirectoryWatch(const QString &filenode, const QString &path);

private slots:
    void onClearTextButtonClicked();
    void onFileBrowserButtonClicked();
    void onWatchButtonClicked();
    void onWatchedListUnWatchButtonClicked();
    void onStatusMessage(const QString &path);

protected:
    void closeEvent(QCloseEvent *event);

protected:
    QVBoxLayout *mainboxlayout_;
    QHBoxLayout *buttonboxlayout_;
    QLineEdit *onetextline_;
    QLabel *onetextlinelabel_;
    QPushButton *cleartextbutton_;
    QPushButton *filebrowserbutton_;
    QPushButton *watchbutton_;
    QListWidget *watchedlist_;
    QHBoxLayout *watchedlistbuttonboxlayout_;
    QLabel *watchedlistlabel_;
    QPushButton *watchedlistunwatchbutton_;
    QSpacerItem *watchedlistspacer_;
    QTextEdit *messages_;

    int counter_;
    QHash<QString, QString> watcheddirs_;
    DAS das_;
};

#endif
