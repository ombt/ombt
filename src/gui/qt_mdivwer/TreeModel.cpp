// general tree model

// system headers
#include <assert.h>
#include <iostream>
#include <fstream>
#include <string>

// Qt headers
#include <QtGui>

// local headers
#include "TreeNode.h"
#include "TreeModel.h"

// ctors and dtor
TreeModel::TreeModel(const std::string &inputfile, QObject *parent):
    AbstractItemModelAdaptor(parent), root_(NULL)
{
    setupModelData(inputfile);
}

TreeModel::~TreeModel()
{
    delete root_;
    ok_ = false;
}

// read in file and populate the tree model
void
TreeModel::setupModelData(const std::string &inputfile)
{
    // assume a problem
    ok_ = false;

    // read in file
    std::vector<std::string> tokens;
    std::ifstream infile;
    infile.open(inputfile.c_str());
    if (!infile) return;

    // read in column heading line -- first line in file
    char buf[10*BUFSIZ+1];
    infile.getline(buf, 10*BUFSIZ);
    if (infile.eof() || infile.fail()) return;
    buf[10*BUFSIZ] = '\0';

    int keylevel = 0;
    tokenize(buf, tokens, keylevel);
    if (tokens.empty()) return;

    // this how the example did it ... sort of, kind of, maybe.
    QVector<QVariant> rootdata;
    std::vector<std::string>::const_iterator tit = tokens.begin();
    std::vector<std::string>::const_iterator tend = tokens.end();
    for ( ; tit != tend; ++tit)
    {
        rootdata << tit->c_str();
    }
    root_ = new TreeNode(rootdata);

    // stacks to track where we are in the tree.
    QList<TreeNode * > parents;
    QList<int> keylevels;
    parents << root_;
    keylevels << 0;

    // read in the rest of the data and construct tree model
    for (infile.getline(buf, 10*BUFSIZ);
        !infile.eof() && !infile.fail();
         infile.getline(buf, 10*BUFSIZ))
    {
        // break string into junks we can handle
        buf[10*BUFSIZ] = '\0';
        tokenize(buf, tokens, keylevel);
        if (tokens.empty()) continue;

        // add new data to tree model
        QVector<QVariant> columndata;
        std::vector<std::string>::const_iterator tit = tokens.begin();
        std::vector<std::string>::const_iterator tend = tokens.end();
        for ( ; tit != tend; ++tit)
        {
            columndata << tit->c_str();
        }

        if (keylevel > keylevels.last())
        {
            if (parents.last()->childCount() > 0)
            {
                parents << parents.last()->child(parents.last()->childCount()-1);
                keylevels << keylevel;
            }
        }
        else
        {
            while (keylevel < keylevels.last() && parents.count() > 0)
            {
                parents.pop_back();
                keylevels.pop_back();
            }
        }

        // append child to current node
        parents.last()->appendChild(new TreeNode(columndata, parents.last()));
    }

    infile.close();

    ok_ = true;
}

// update functions
TreeNode *
TreeModel::getItem(const QModelIndex &index) const
{
    if (index.isValid())
    {
        TreeNode *item = static_cast<TreeNode *>(index.internalPointer());
        if (item) return(item);
    }
    return(root_);
}

int
TreeModel::columnCount(const QModelIndex & ) const
{
    // assume a fix number of columns
    return(root_->columnCount());
}

QVariant
TreeModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || role != Qt::DisplayRole) return(QVariant());
    TreeNode *item = static_cast<TreeNode*>(index.internalPointer());
    return(item->data(index.column()));
}

Qt::ItemFlags
TreeModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return(0);
    else if (readonly_)
        return(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
    else
        return(Qt::ItemIsEditable | Qt::ItemIsEnabled | Qt::ItemIsSelectable);
}

QVariant
TreeModel::headerData(int section, Qt::Orientation orientation,
                      int role) const
{
    if (role != Qt::DisplayRole) return(QVariant());

    switch (orientation)
    {
    case Qt::Horizontal:
        // std::cout << "Column Header - " << section << std::endl;
        return(root_->data(section));
#if 0
    case Qt::Vertical:
        // std::cout << "Row Header - " << section << std::endl;
        return(QString("Row %1").arg(section));
    default:
        return(QVariant());
#else
    default:
        // std::cout << "Row Header - " << section << std::endl;
        return(QString("Row %1").arg(section));
#endif
    }
}

bool
TreeModel::setData(const QModelIndex &index, const QVariant &value,
                        int role)
{
    if (role != Qt::EditRole) return false;

    TreeNode *item = getItem(index);
    bool result = item->setData(index.column(), value);

    if (result)
        emit dataChanged(index, index);

    return result;
}

QModelIndex
TreeModel::index(int row, int column, const QModelIndex &parent) const
{
    if (parent.isValid() && parent.column() != 0) return(QModelIndex());

    TreeNode *parentItem = getItem(parent);
    TreeNode *childItem = parentItem->child(row);
    if (childItem)
        return(createIndex(row, column, childItem));
    else
        return(QModelIndex());
}

QModelIndex
TreeModel::parent(const QModelIndex &index) const
{
    if (!index.isValid()) return(QModelIndex());

    TreeNode *childItem = getItem(index);
    TreeNode *parentItem = childItem->parent();

    if (parentItem == root_) return(QModelIndex());

    return(createIndex(parentItem->row(), 0, parentItem));
}

int
TreeModel::rowCount(const QModelIndex &parent) const
{
    return(getItem(parent)->childCount());
}

// insert new rows or columns
bool
TreeModel::insertColumns(int position, int columns, const QModelIndex &parent)
{
    bool success;

    beginInsertColumns(parent, position, position + columns - 1);
    success = root_->insertColumns(position, columns);
    endInsertColumns();

    return(success);
}

bool
TreeModel::insertRows(int position, int rows, const QModelIndex &parent)
{
    TreeNode *parentItem = getItem(parent);
    bool success;

    beginInsertRows(parent, position, position + rows - 1);
    success = parentItem->insertChildren(position, rows, root_->columnCount());
    endInsertRows();

    return(success);
}

bool
TreeModel::setHeaderData(int section, Qt::Orientation orientation,
                         const QVariant &value, int role)
{
    if (role != Qt::EditRole || orientation != Qt::Horizontal)
        return(false);

    bool result = root_->setData(section, value);

    if (result)
        emit headerDataChanged(orientation, section, section);

    return(result);
}

bool
TreeModel::removeColumns(int position, int columns, const QModelIndex &parent)
{
    bool success;

    beginRemoveColumns(parent, position, position + columns - 1);
    success = root_->removeColumns(position, columns);
    endRemoveColumns();

    if (root_->columnCount() == 0) removeRows(0, rowCount());

    return(success);
}

bool
TreeModel::removeRows(int position, int rows, const QModelIndex &parent)
{
    TreeNode *parentItem = getItem(parent);
    bool success = true;

    beginRemoveRows(parent, position, position + rows - 1);
    success = parentItem->removeChildren(position, rows);
    endRemoveRows();

    return(success);
}


