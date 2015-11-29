#ifndef __TREENODE_H
#define __TREENODE_H
// a read-only tree node for a general tree model

// headers
#include <stddef.h>
#include <QList>
#include <QVector>
#include <QVariant>

// a node in the data tree
class TreeNode {
public:
    // ctor and dtor - minimal set
    TreeNode();
    TreeNode(const QVector<QVariant> &data, TreeNode *parent = NULL);
    ~TreeNode();

    // utilities
    void appendChild(TreeNode *child);

    // functions for read-only Qt tree model
    TreeNode *child(int row);
    int childCount() const;
    int columnCount() const;
    QVariant data(int column) const;
    TreeNode *parent();
    int row() const;

    // additional functions for insert/delete Qt tree model
    bool insertChildren(int position, int count, int columns);
    bool insertColumns(int position, int columns);
    bool removeChildren(int position, int count);
    bool removeColumns(int position, int columns);
    bool setData(int column, const QVariant &value);

private:
    // block these
    TreeNode(const TreeNode &src);
    TreeNode &operator=(const TreeNode &rhs);

protected:
    // pointers to children, parent and the node's data
    QList<TreeNode * > children_;
    QVector<QVariant> data_;
    TreeNode *parent_;
};

#endif
