#ifndef __DUMMYTREEMODEL_H
#define __DUMMYTREEMODEL_H

// dummy tree model based tree node

// headers
#include <vector>
#include <QModelIndex>
#include <QVariant>

#include "AbstractItemModelAdaptor.h"

// simple tree model class based on Qt's generic class
class DummyTreeModel: public AbstractItemModelAdaptor
{
    // Qt magic macro -- Q_OBJECT macro is mandatory for any object that 
    // implements signals, slots or properties. 
    Q_OBJECT

public:
    // ctors and dtor
    DummyTreeModel(const std::string &inputfile, QObject *parent = NULL);
    ~DummyTreeModel();

    // support for read-only item model public interface
    Qt::ItemFlags flags(const QModelIndex &index) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const;
    QModelIndex index(int row, int column,
                      const QModelIndex &parent = QModelIndex()) const;
    QModelIndex parent(const QModelIndex &index) const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;

#if 0
    // support for insert/delete/update of model public interface
    bool setData(const QModelIndex &index, const QVariant &value,
                 int role = Qt::EditRole);
    bool setHeaderData(int section, Qt::Orientation orientation,
                       const QVariant &value, int role = Qt::EditRole);
    bool insertColumns(int position, int columns,
                       const QModelIndex &parent = QModelIndex());
    bool removeColumns(int position, int columns,
                       const QModelIndex &parent = QModelIndex());
    bool insertRows(int position, int rows,
                    const QModelIndex &parent = QModelIndex());
    bool removeRows(int position, int rows,
                    const QModelIndex &parent = QModelIndex());
#endif

protected:
    bool hasChildren(const QModelIndex &parent) const;

private:
    void setupModelData(const std::string &identifier);

    int numkeys_;
    int numlevels_;
    int numsublevels_;
    int numfields_;
    int numgroups_;

    int numrows_;
    int numcols_;
};

#endif
