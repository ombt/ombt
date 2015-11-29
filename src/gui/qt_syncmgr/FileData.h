#ifndef __FILE_DATA_H
#define __FILE_DATA_H

#include <QString>
#include <QStringList>
#include <QList>
#include <QVector>
#include <QDir>

#include "DASQDebug.h"

class FileData
{
public:
    struct Data
    {
        Data(): isdir_(false), isdirty_(false), dummy_(0) { }
        bool isdir_;
        bool isdirty_;
        int dummy_;
    };

    struct Node
    {
        // ctor
        Node(): 
            name_(), 
            parent_(NULL), child_(NULL), sibling_(NULL),
            data_() { }

        // data
        QString name_;
        Node *parent_;
        Node *child_;
        Node *sibling_;
        Data data_;
    };

private:
    enum OperationStatus
    {
        Searching,
        Found,
        NotFound,
        Inserted,
        NotInserted,
        Updated,
        NotUpdated,
        Removed,
        NotRemoved
    };


public:
    // ctor and dtor
    explicit FileData();
    ~FileData();

    // access
    void clear();
    void retrieve(const QString &path, Data &data, bool &retrieved);
    void insert(const QString &path, const Data &data, bool &inserted);
    void update(const QString &path, const Data &data, bool &updated);
    void remove(const QString &path, bool &removed);

    // output data
    void print() const;
    void print(const Node * const &node) const;

private:
    // utilities
    void clear(Node *&node);
    void retrieve(Node *&node, int idx, int maxidx, const QVector<QString> &pathvec, Data &data, OperationStatus &status);
    void insert(Node *&node, int idx, int maxidx, const QVector<QString> &pathvec, const Data &data, OperationStatus &status);
    void update(Node *&node, int idx, int maxidx, const QVector<QString> &pathvec, const Data &data, OperationStatus &status);
    void remove(Node *&node, int idx, int maxidx, const QVector<QString> &pathvec, OperationStatus &status);
    void addNewNodes(Node *&node, int idx, int maxidx, const QVector<QString> &pathvec, const Data &data, OperationStatus &status);
    void print(QStringList &path, const Node * const &node, bool output) const;

private:
    Node *root_;
};

#endif
