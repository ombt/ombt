#ifndef __DHM_WINDOW_H
#define __DHM_WINDOW_H

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
#include <QSet>

#include "DASQDebug.h"
#include "DAS.h"

// DHM sub window
class DHMWindow: public QMdiSubWindow
{
    Q_OBJECT

public:
    DHMWindow();
    ~DHMWindow();

signals:

private slots:
    void onAddDAS(const QString &das);
    void onRemoveDAS(const QString &das);
    void onAddFileNodeToDAS(const QString &das, const QString &filenode);
    void onRemoveFileNodeToDAS(const QString &das, const QString &filenode);
    void onStatusMessage(const QString &path);

protected:
    void closeEvent(QCloseEvent *event);

protected:
    QVBoxLayout *mainboxlayout_;
    QLabel *daslistlabel_;
    QListWidget *daslist_;
    QLabel *dasfnlistlabel_;
    QListWidget *dasfnlist_;
    QTextEdit *messages_;

    QSet<QString> dasset_;
    QSet<QString> dasfnset_;
};

#endif
