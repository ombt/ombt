// general tree model

// system headers
#include <assert.h>
#include <iostream>
#include <fstream>
#include <string>

// Qt headers
#include <QtGui>

// local headers
#include "DummyTreeModel.h"

DummyTreeModel::DummyTreeModel(const std::string &identifier, QObject *parent):
    AbstractItemModelAdaptor(parent)
{
    setupModelData(identifier);
}

DummyTreeModel::~DummyTreeModel()
{
    ok_ = false;
}

void
DummyTreeModel::setupModelData(const std::string &identifier)
{
    ok_ = true;

    int keylevel;
    std::vector<std::string> tokens;
    size_t last_slash = identifier.find_last_of("/");
    tokenize(identifier.substr(last_slash+1), tokens, keylevel, "_.");

    numkeys_ = ::atoi(tokens[0].c_str());
    numlevels_ = ::atoi(tokens[1].c_str());
    numsublevels_ = ::atoi(tokens[2].c_str());
    numfields_ = ::atoi(tokens[3].c_str());
    numgroups_ = ::atoi(tokens[4].c_str());

    // keys as rows and fields as columns
    numrows_ = numkeys_;
    numcols_ = numfields_;
}

int
DummyTreeModel::rowCount(const QModelIndex & /* parent */) const
{
    return(numrows_);
}
int
DummyTreeModel::columnCount(const QModelIndex & ) const
{
    return(numcols_);
}

QVariant
DummyTreeModel::data(const QModelIndex &index, int role) const
{
    switch (role)
    {
    case Qt::DisplayRole:
        return((index.row()+1)*(index.column()+1));
    case Qt::ToolTipRole:
        return(QString("%1 x %2").arg(index.row()+1).arg(index.column()+1));
    default:
        return(QVariant());
    }
}

QVariant
DummyTreeModel::headerData(int section, Qt::Orientation orientation,
                      int role) const
{
    if (role != Qt::DisplayRole) return(QVariant());

    switch (orientation)
    {
    case Qt::Horizontal:
        return(QString("Col %1").arg(section));
    default:
        return(QString("Row %1").arg(section));
    }
}

Qt::ItemFlags
DummyTreeModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return(0);
    else if (readonly_)
        return(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
    else
        return(Qt::ItemIsEditable | Qt::ItemIsEnabled | Qt::ItemIsSelectable);
}

QModelIndex
DummyTreeModel::index(int row, int column, const QModelIndex &parent) const
{
    if (hasIndex(row, column, parent))
        return(createIndex(row, column, 0));
    else
        return(QModelIndex());
}

QModelIndex
DummyTreeModel::parent(const QModelIndex & /* index */) const
{
    return(QModelIndex());
}

bool
DummyTreeModel::hasChildren(const QModelIndex &parent) const
{
    if (parent.model() == this || !parent.isValid())
        return rowCount(parent) > 0 && columnCount(parent) > 0;
    else
        return false;
}

#if 0
// insert new rows or columns
bool
DummyTreeModel::insertColumns(int position, int columns, const QModelIndex &parent)
{
    beginInsertColumns(parent, position, position + columns - 1);

    endInsertColumns();
    return(true);
}
 
bool
DummyTreeModel::insertRows(int position, int rows, const QModelIndex &parent)
{
    beginInsertRows(parent, position, position + rows - 1);

    endInsertRows();

    return(true);
}

bool
DummyTreeModel::setHeaderData(int section, Qt::Orientation orientation,
                         const QVariant & /* value */, int role)
{
    if (role != Qt::EditRole || orientation != Qt::Horizontal)
        return(false);

    emit headerDataChanged(orientation, section, section);

    return(true);
}


bool
DummyTreeModel::removeColumns(int position, int columns, const QModelIndex &parent)
{
    beginRemoveColumns(parent, position, position + columns - 1);
    endRemoveColumns();
    return(true);
}

bool
DummyTreeModel::removeRows(int position, int rows, const QModelIndex &parent)
{

    beginRemoveRows(parent, position, position + rows - 1);
    endRemoveRows();
    return(true);
}

#endif
