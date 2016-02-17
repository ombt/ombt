#ifndef __BINARYTREE_H
#define __BINARYTREE_H
// binary tree class definition

// headers
#include <stdlib.h>
#include <iostream.h>

// local headers
#include "returns.h"
#include "debug.h"
#include "data.h"

// forward declarations
class BinaryTreeNode;
typedef BinaryTreeNode *BinaryTree;

// definitions
#define ENDOFTREE ((BinaryTree)0)

// binary tree class
class BinaryTreeNode {
public:
        // constructors and destructor
        BinaryTreeNode(DataItem *, BinaryTree = ENDOFTREE, 
                   BinaryTree = ENDOFTREE);
        BinaryTreeNode(const BinaryTreeNode &);
        virtual ~BinaryTreeNode();

        // assignment and logical operators
        virtual BinaryTreeNode &operator=(const BinaryTreeNode &);
        virtual int operator==(const BinaryTreeNode &) const;
        virtual int operator!=(const BinaryTreeNode &) const;
        virtual int operator<(const BinaryTreeNode &) const;
        virtual int operator>(const BinaryTreeNode &) const;
        virtual int operator<=(const BinaryTreeNode &) const;
        virtual int operator>=(const BinaryTreeNode &) const;

        // tree operations
        friend int isEmpty(const BinaryTree &);
        friend int makeNull(BinaryTree &);
        friend int insert(BinaryTree &, DataItem *);
        friend int remove(BinaryTree &, DataItem *);
        friend int retrieve(const BinaryTree, DataItem *&);
        friend BinaryTree leftTree(const BinaryTree);
        friend BinaryTree rightTree(const BinaryTree);
        friend DataItem *removeMin(BinaryTree &);

        // tree traversal functions
        friend int preOrder(BinaryTree, int (*)(DataItem *));
        friend int inOrder(BinaryTree, int (*)(DataItem *));
        friend int postOrder(BinaryTree, int (*)(DataItem *));

        // print tree
        virtual void dump(ostream & = cout) const;
        friend ostream &operator<<(ostream &, const BinaryTreeNode &);
        friend ostream &operator<<(ostream &, const BinaryTree);

private:
        // internal use only
        BinaryTreeNode();

protected:
        // data
        DataItem *data;
        BinaryTree left;
        BinaryTree right;
};


#endif
