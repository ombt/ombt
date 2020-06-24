// functions for binary tree class

// headers
#include <stdlib.h>
#include <iostream.h>
#include <assert.h>

// local headers
#include "binary.h"

// binary tree constructors and destructor
BinaryTreeNode::BinaryTreeNode():
        data(0), left(ENDOFTREE), right(ENDOFTREE)
{
        // do nothing
}

BinaryTreeNode::BinaryTreeNode(DataItem *d, BinaryTree lbt, BinaryTree rbt):
        data(d), left(lbt), right(rbt)
{
        assert(data != (DataItem *)0);
}

BinaryTreeNode::BinaryTreeNode(const BinaryTreeNode &btn):
        data(btn.data), left(ENDOFTREE), right(ENDOFTREE)
{
        assert(data != (DataItem *)0);
}

BinaryTreeNode::~BinaryTreeNode()
{
        data = (DataItem *)0;
        left = ENDOFTREE;
        right = ENDOFTREE;
}

// assignment and equivalence
BinaryTreeNode &
BinaryTreeNode::operator=(const BinaryTreeNode &bt)
{
        if (this != &bt)
        {
                data = bt.data;
                left = ENDOFTREE;
                right = ENDOFTREE;
        }
        assert(data != (DataItem *)0);
        return(*this);                                                              
}

int
BinaryTreeNode::operator==(const BinaryTreeNode &bt) const
{
        return(*data == *bt.data);
}

int
BinaryTreeNode::operator!=(const BinaryTreeNode &bt) const
{
        return(*data != *bt.data);
}

int
BinaryTreeNode::operator<(const BinaryTreeNode &bt) const
{
        return(*data < *bt.data);
}

int
BinaryTreeNode::operator>(const BinaryTreeNode &bt) const
{
        return(*data > *bt.data);
}

int
BinaryTreeNode::operator<=(const BinaryTreeNode &bt) const
{
        return(*data <= *bt.data);
}

int
BinaryTreeNode::operator>=(const BinaryTreeNode &bt) const
{
        return(*data >= *bt.data);
}

// tree operations
int 
isEmpty(const BinaryTree &bt)
{
        if (bt == ENDOFTREE)
                return(1);
        else
                return(0);
}

int
makeNull(BinaryTree &bt)
{
        if (bt != ENDOFTREE);
        {
                makeNull(bt->left);
                makeNull(bt->right);
                delete bt;
                bt = ENDOFTREE;
        }
        return(OK);
}

int
insert(BinaryTree &bt, DataItem *d)
{
        if (bt == ENDOFTREE)
        {
                bt = new BinaryTreeNode(d);
                assert(bt != (BinaryTreeNode *)0);
        }
        else if (*d < *bt->data)
        {
                return(insert(bt->left, d));
        }
        else if (*d > *bt->data)
        {
                return(insert(bt->right, d));
        }
        else
        {
                // already in tree, overwrite
                bt->data = d;
        }
        assert(bt->data != (DataItem *)0);
        return(OK);
}

int
remove(BinaryTree &bt, DataItem *d)
{
        if (bt == ENDOFTREE)
        {
                return(NOMATCH);
        }
        else if (*d < *bt->data)
        {
                return(remove(bt->left, d));
        }
        else if (*d > *bt->data)
        {
                return(remove(bt->right, d));
        }
        else if (bt->left == ENDOFTREE && bt->right == ENDOFTREE)
        {
                delete bt;
                bt = ENDOFTREE;
                return(OK);
        }
        else if (bt->left == ENDOFTREE)
        {
                BinaryTree save = bt->right;
                delete bt;
                bt = save;
                return(OK);
        }
        else if (bt->right == ENDOFTREE)
        {
                BinaryTree save = bt->left;
                delete bt;
                bt = save;
                return(OK);
        }
        else
        {
                bt->data = removeMin(bt->right);
                assert(bt->data != (DataItem *)0);
                return(OK);
        }
}

DataItem *
removeMin(BinaryTree &bt)
{
        if (bt->left == ENDOFTREE)
        {
                DataItem *saveData = bt->data;
                BinaryTree saveTree = bt->right;
                delete bt;
                bt = saveTree;
                return(saveData);
        }
        else
        {
                return(removeMin(bt->left));
        }
}

int
retrieve(const BinaryTree bt, DataItem *&d)
{
        if (bt != ENDOFTREE)
        {
                d = bt->data;
                return(OK);
        }
        else
                return(NOTOK);
}

BinaryTree 
leftTree(const BinaryTree bt)
{
        if (bt != ENDOFTREE)
                return(bt->left);
        else
                return(ENDOFTREE);
}

BinaryTree  
rightTree(const BinaryTree bt)
{
        if (bt != ENDOFTREE)
                return(bt->right);
        else
                return(ENDOFTREE);
}

// tree traversal
int
preOrder(BinaryTree bt, int (*work)(DataItem *))
{
        int status;
        if (bt == ENDOFTREE) return(OK);
        if ((status = (*work)(bt->data)) != OK) return(status);
        if ((status = preOrder(bt->left, work)) != OK) return(status);
        if ((status = preOrder(bt->right, work)) != OK) return(status);
        return(OK);
}

int
inOrder(BinaryTree bt, int (*work)(DataItem *))
{
        int status;
        if (bt == ENDOFTREE) return(OK);
        if ((status = inOrder(bt->left, work)) != OK) return(status);
        if ((status = (*work)(bt->data)) != OK) return(status);
        if ((status = inOrder(bt->right, work)) != OK) return(status);
        return(OK);
}

int
postOrder(BinaryTree bt, int (*work)(DataItem *))
{
        int status;
        if (bt == ENDOFTREE) return(OK);
        if ((status = postOrder(bt->left, work)) != OK) return(status);
        if ((status = postOrder(bt->right, work)) != OK) return(status);
        if ((status = (*work)(bt->data)) != OK) return(status);
        return(OK);
}

// print tree
void 
BinaryTreeNode::dump(ostream &os) const
{
        if (left != ENDOFTREE)
                left->dump(os);
        if (data != (DataItem *)0)
                os << *data << " ";
        else
                os << "NO DATA !!!";
        if (right != ENDOFTREE)
                right->dump(os);
        return;
}

ostream &
operator<<(ostream &os, const BinaryTreeNode &bt)
{
        os << "{ ";
        bt.dump(os);
        os << " }";
        return(os);
}

ostream &
operator<<(ostream &os, const BinaryTree bt)
{
        os << "{ ";
        if (bt != ENDOFTREE) bt->dump(os);
        os << " }";
        return(os);
}

