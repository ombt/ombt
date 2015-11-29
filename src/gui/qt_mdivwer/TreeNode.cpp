// tree node in a general tree

// headers
#include "TreeNode.h"
#include <QVector>

// ctors and dtor
TreeNode::TreeNode():
    children_(), data_(), parent_(NULL)
{
    // do nothing
}

TreeNode::TreeNode(const QVector<QVariant> &data, TreeNode *parent):
    children_(), data_(data), parent_(parent)
{
    // do nothing
}

TreeNode::~TreeNode()
{
    // use Qt recursive method
    qDeleteAll(children_.begin(), children_.end());
}

// utilities
void
TreeNode::appendChild(TreeNode *node)
{
    children_.append(node);
}

// functions provided to Qt
TreeNode *
TreeNode::child(int row)
{
    // NOTE: if row is out-of-bounds, then a default-constructed 
    // value is returned.
    return(children_.value(row));
}

int
TreeNode::childCount() const
{
    return(children_.count());
}

// how many data items does this node contain?
int
TreeNode::columnCount() const
{
    return(data_.count());
}

// data for requested column
QVariant
TreeNode::data(int column) const
{
    return(data_.value(column));
}

// set data for a column
bool
TreeNode::setData(int column, const QVariant &value)
{
    if (column < 0 || column >= data_.size()) return(false);
    if (value.isNull() || !value.isValid()) return(false);
    if (value.toString().isEmpty()) return(false);

    data_[column] = value;
    return(true);
}

// insert a new row or remove a row
bool
TreeNode::insertChildren(int position, int count, int columns)
{
    if (position < 0 || position > children_.size()) return(false);

    for (int row = 0; row < count; ++row)
    {
        QVector<QVariant> data(columns);
        TreeNode *node = new TreeNode(data, this);
        children_.insert(position, node);
    }

    return true;
}

bool
TreeNode::removeChildren(int position, int count)
{
    if (position < 0 || (position + count) > children_.size()) return(false);

    for (int row = 0; row < count; ++row)
    {
        delete children_.takeAt(position);
    }

    return true;
}

// insert and remove columns
bool
TreeNode::insertColumns(int position, int columns)
{
    if (position < 0 || position > children_.size()) return false;

    for (int column = 0; column < columns; ++column)
    {
        data_.insert(position, QVariant());
    }

    foreach (TreeNode *child, children_)
    {
        child->insertColumns(position, columns);
    }

    return(true);
}

bool
TreeNode::removeColumns(int position, int columns)
{
    if (position < 0 || (position + columns) > data_.size()) return(false);

    for (int column = 0; column < columns; ++column)
    {
        data_.remove(position);
    }

    foreach (TreeNode *child, children_)
    {
        child->removeColumns(position, columns);
    }

    return(true);
}

// return node parent pointer
TreeNode *
TreeNode::parent()
{
    return(parent_);
}

// get node row -- need to look at parent to get row.
int
TreeNode::row() const
{
    if (parent_)
        return(parent_->children_.indexOf(const_cast<TreeNode *>(this)));
    else
        return(NULL); // root node had no parent
}


