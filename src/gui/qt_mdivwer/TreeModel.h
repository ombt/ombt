#ifndef __TREEMODEL_H
#define __TREEMODEL_H

// Qt tree model based tree node

// headers
#include <vector>
#include <QModelIndex>
#include <QVariant>

#include "AbstractItemModelAdaptor.h"

// forward declarations
class TreeNode;

// simple tree model class based on Qt's generic class
class TreeModel: public AbstractItemModelAdaptor
{
    // Qt magic macro -- Q_OBJECT macro is mandatory for any object that 
    // implements signals, slots or properties. 
    Q_OBJECT

public:
    // ctors and dtor
    // TreeModel(const QStringList &headers, const QString &data, QObject *parent = 0);
    TreeModel(const std::string &inputfile, QObject *parent = NULL);
    ~TreeModel();

    // support for read-only item model public interface
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const;
    QModelIndex index(int row, int column,
                      const QModelIndex &parent = QModelIndex()) const;
    QModelIndex parent(const QModelIndex &index) const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;

    // support for insert/delete/update of model public interface
    Qt::ItemFlags flags(const QModelIndex &index) const;
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

private:
    // build tree model from data in the file.
    void setupModelData(const std::string &inputfile);
    TreeNode *getItem(const QModelIndex &index) const;

    // data
    TreeNode *root_;
};

#endif
