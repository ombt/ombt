//
// Copyright (C) 2010, OMBT LLC and Mike A. Rumore
// All rights reserved.
// Contact: Mike A. Rumore, (mike.a.rumore@gmail.com)
//
#ifndef __BINARY_TREE2_H
#define __BINARY_TREE2_H
// binary search tree class definition using recursion

// headers
#include "adt/AbsTree.h"
#include "adt/AbsIterator.h"
#include "adt/Queue_List.h"
#include "adt/Stack_List.h"

namespace ombt {

// forward declarations
template <class DataType> class BinaryTreeNode;
template <class DataType> class BinaryTree;
template <class DataType> class BinaryTreeIterator_PreOrder;
template <class DataType> class BinaryTreeIterator_InOrder;
template <class DataType> class BinaryTreeIterator_PostOrder;
template <class DataType> class BinaryTreeIterator_LevelOrder;

// tree node class
template <class DataType> class BinaryTreeNode
{
public:
#ifdef USEOSTREAM
	// output
	friend std::ostream &operator<<(std::ostream &os, 
		const BinaryTreeNode<DataType> &btn) {
		os << btn.data;
		return(os);
	};
#endif

protected:
        // friend classes 
        friend class BinaryTree<DataType>;
	friend class BinaryTreeIterator_PreOrder<DataType>;
	friend class BinaryTreeIterator_InOrder<DataType>;
	friend class BinaryTreeIterator_PostOrder<DataType>;
	friend class BinaryTreeIterator_LevelOrder<DataType>;

        // constructors and destructor
        BinaryTreeNode(const DataType &);
        BinaryTreeNode(const BinaryTreeNode &);
        ~BinaryTreeNode();

	// assignment
        BinaryTreeNode &operator=(const BinaryTreeNode &);

        // internal data
        DataType data;
        BinaryTreeNode<DataType> *left;
        BinaryTreeNode<DataType> *right;
};

// binary search tree class
template <class DataType> class BinaryTree:
	public AbstractTree<DataType>
{
public:
	// friend classes 
	friend class BinaryTreeIterator_PreOrder<DataType>;
	friend class BinaryTreeIterator_InOrder<DataType>;
	friend class BinaryTreeIterator_PostOrder<DataType>;
	friend class BinaryTreeIterator_LevelOrder<DataType>;

        // constructors and destructor
        BinaryTree();
        BinaryTree(const BinaryTree &);
        ~BinaryTree();

        // assignment
        BinaryTree &operator=(const BinaryTree &);

        // binary tree operations
        int insert(const DataType &);
        int update(const DataType &);
        int remove(DataType &);
        int retrieve(DataType &) const;
        int includes(const DataType &) const;
	int isEmpty() const;
        void clear();

	// recursive tree traversal functions
	int preOrder(int (*)(DataType &));
	int inOrder(int (*)(DataType &));
	int postOrder(int (*)(DataType &));
	int levelOrder(int (*)(DataType &));

        // miscellaneous
        std::ostream &dump(std::ostream &) const;

protected:
	// utility functions
        BinaryTreeNode<DataType> *copy() const;
        BinaryTreeNode<DataType> *
		copy(const BinaryTreeNode<DataType> *) const;
        void clear(BinaryTreeNode<DataType> *&);
        int insert(BinaryTreeNode<DataType> *&, const DataType &);
        int remove(BinaryTreeNode<DataType> *&, DataType &);
        int removeRightMost(BinaryTreeNode<DataType> *&, DataType &);
        int retrieve(const BinaryTreeNode<DataType> *, DataType &) const;
        int includes(const BinaryTreeNode<DataType> *, const DataType &) const;
        int preOrder(BinaryTreeNode<DataType> *, int (*)(DataType &));
        int inOrder(BinaryTreeNode<DataType> *, int (*)(DataType &));
        int postOrder(BinaryTreeNode<DataType> *, int (*)(DataType &));
        void dump(BinaryTreeNode<DataType> *, std::ostream &) const;

protected:
        // internal data
        BinaryTreeNode<DataType> *root;
#ifdef MUTABLE_ITERATOR
	List<AbstractIterator<DataType> * > iterList;
#endif
};

// pre-order tree iterator
template <class DataType> class BinaryTreeIterator_PreOrder:
	public AbstractIterator<DataType>
{
public:
        // constructors and destructor
#ifdef MUTABLE_ITERATOR
        BinaryTreeIterator_PreOrder(
		BinaryTree<DataType> &);
#else
        BinaryTreeIterator_PreOrder(
		const BinaryTree<DataType> &);
#endif
        BinaryTreeIterator_PreOrder(
		const BinaryTreeIterator_PreOrder<DataType> &);
        ~BinaryTreeIterator_PreOrder();

	// reset interator to start
	void reset();

	// check if at end of list
	int done() const;

        // return data 
        DataType operator()();

	// advance iterator to next link
	int operator++(int);

private:
	// not allowed
        BinaryTreeIterator_PreOrder();
        BinaryTreeIterator_PreOrder &operator=(const 
		BinaryTreeIterator_PreOrder &);

protected:
        // internal data
#ifdef MUTABLE_ITERATOR
	BinaryTree<DataType> *tree;
#else
	const BinaryTree<DataType> *tree;
#endif
	Stack_List<BinaryTreeNode<DataType> * > stack;
};

// in-order tree iterator
template <class DataType> class BinaryTreeIterator_InOrder:
	public BinaryTreeIterator_PreOrder<DataType>
{
public:
        // constructors and destructor
#ifdef MUTABLE_ITERATOR
        BinaryTreeIterator_InOrder(
		BinaryTree<DataType> &);
#else
        BinaryTreeIterator_InOrder(
		const BinaryTree<DataType> &);
#endif
        BinaryTreeIterator_InOrder(
		const BinaryTreeIterator_InOrder<DataType> &);
        ~BinaryTreeIterator_InOrder();

	// reset interator to start
	void reset();

	// advance iterator to next link
	int operator++(int);

private:
	// not allowed
        BinaryTreeIterator_InOrder();
        BinaryTreeIterator_InOrder &operator=(const 
		BinaryTreeIterator_InOrder &);
};

// post-order tree iterator
template <class DataType> class BinaryTreeIterator_PostOrder:
	public BinaryTreeIterator_PreOrder<DataType>
{
public:
        // constructors and destructor
#ifdef MUTABLE_ITERATOR
        BinaryTreeIterator_PostOrder(
		BinaryTree<DataType> &);
#else
        BinaryTreeIterator_PostOrder(
		const BinaryTree<DataType> &);
#endif
        BinaryTreeIterator_PostOrder(
		const BinaryTreeIterator_PostOrder<DataType> &);
        ~BinaryTreeIterator_PostOrder();

	// reset interator to start
	void reset();

	// advance iterator to next link
	int operator++(int);

private:
	// not allowed
        BinaryTreeIterator_PostOrder();
        BinaryTreeIterator_PostOrder &operator=(const 
		BinaryTreeIterator_PostOrder &);

protected:
        // internal data
	Stack_List<int> vstack;
};

// level-order tree iterator
template <class DataType> class BinaryTreeIterator_LevelOrder:
	public AbstractIterator<DataType>
{
public:
        // constructors and destructor
#ifdef MUTABLE_ITERATOR
        BinaryTreeIterator_LevelOrder(
		BinaryTree<DataType> &);
#else
        BinaryTreeIterator_LevelOrder(
		const BinaryTree<DataType> &);
#endif
        BinaryTreeIterator_LevelOrder(
		const BinaryTreeIterator_LevelOrder<DataType> &);
        ~BinaryTreeIterator_LevelOrder();

	// reset interator to start
	void reset();

	// check if at end of list
	int done() const;

        // return data 
        DataType operator()();

	// advance iterator to next link
	int operator++(int);

private:
	// not allowed
        BinaryTreeIterator_LevelOrder();
        BinaryTreeIterator_LevelOrder &operator=(const 
		BinaryTreeIterator_LevelOrder &);

protected:
        // internal data
#ifdef MUTABLE_ITERATOR
	BinaryTree<DataType> *tree;
#else
	const BinaryTree<DataType> *tree;
#endif
	Queue_List<BinaryTreeNode<DataType> * > queue;
};

#include "adt/BinaryTree2.i"

}

#endif
