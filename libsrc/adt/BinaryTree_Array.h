//
// Copyright (C) 2010, OMBT LLC and Mike A. Rumore
// All rights reserved.
// Contact: Mike A. Rumore, (mike.a.rumore@gmail.com)
//
#ifndef __BINARY_TREE_ARRAY_H
#define __BINARY_TREE_ARRAY_H
// binary search tree class definition

// headers
#include <adt/AbsTree.h>
#include <adt/AbsIterator.h>
#include <adt/Queue_List.h>
#include <adt/Stack_List.h>

namespace ombt {

// forward declarations
template <class DataType> class BinaryTreeNode_Array;
template <class DataType> class BinaryTree_Array;
template <class DataType> class BinaryTree_Array_Iterator_PreOrder;
template <class DataType> class BinaryTree_Array_Iterator_InOrder;
template <class DataType> class BinaryTree_Array_Iterator_PostOrder;
template <class DataType> class BinaryTree_Array_Iterator_LevelOrder;

// tree node class
template <class DataType> class BinaryTreeNode_Array
{
public:
#ifdef USEOSTREAM
	// output
	friend std::ostream &operator<<(std::ostream &os, 
		const BinaryTreeNode_Array<DataType> &btn) {
		os << btn.data;
		return(os);
	};
#endif

protected:
        // friend classes 
        friend class BinaryTree_Array<DataType>;
	friend class BinaryTree_Array_Iterator_PreOrder<DataType>;
	friend class BinaryTree_Array_Iterator_InOrder<DataType>;
	friend class BinaryTree_Array_Iterator_PostOrder<DataType>;
	friend class BinaryTree_Array_Iterator_LevelOrder<DataType>;

        // constructors and destructor
        BinaryTreeNode_Array();
        BinaryTreeNode_Array(const DataType &);
        BinaryTreeNode_Array(const BinaryTreeNode_Array &);
        ~BinaryTreeNode_Array();

	// assignment
        BinaryTreeNode_Array &operator=(const BinaryTreeNode_Array &);

        // internal data
	int left, right;
        DataType data;
};

// binary search tree class
template <class DataType> class BinaryTree_Array:
	public AbstractTree<DataType>
{
public:
	// friend classes 
	friend class BinaryTree_Array_Iterator_PreOrder<DataType>;
	friend class BinaryTree_Array_Iterator_InOrder<DataType>;
	friend class BinaryTree_Array_Iterator_PostOrder<DataType>;
	friend class BinaryTree_Array_Iterator_LevelOrder<DataType>;

        // constructors and destructor
        BinaryTree_Array(int);
        BinaryTree_Array(const BinaryTree_Array &);
        ~BinaryTree_Array();

        // assignment
        BinaryTree_Array &operator=(const BinaryTree_Array &);

        // binary tree operations
        int insert(const DataType &);
        int update(const DataType &);
        int remove(DataType &);
        int retrieve(DataType &) const;
        int includes(const DataType &) const;
	int isEmpty() const;
	int isFull() const;
        void clear();

	// recursive tree traversal functions
	int preOrder(int (*)(DataType &));
	int inOrder(int (*)(DataType &));
	int postOrder(int (*)(DataType &));
	int levelOrder(int (*)(DataType &));

        // miscellaneous
        std::ostream &dump(std::ostream &) const;

private:
	// not provided
	BinaryTree_Array();

protected:
	// utility functions
        int newNode(const DataType &);
	void deleteNode(int);

protected:
        // internal data
	int arraySize, root, available;
        BinaryTreeNode_Array<DataType> *array;
};

// pre-order tree iterator
template <class DataType> class BinaryTree_Array_Iterator_PreOrder:
	public AbstractIterator<DataType>
{
public:
        // constructors and destructor
        BinaryTree_Array_Iterator_PreOrder(
		const BinaryTree_Array<DataType> &);
        BinaryTree_Array_Iterator_PreOrder(
		const BinaryTree_Array_Iterator_PreOrder<DataType> &);
        ~BinaryTree_Array_Iterator_PreOrder();

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
        BinaryTree_Array_Iterator_PreOrder();
        BinaryTree_Array_Iterator_PreOrder &operator=(const 
		BinaryTree_Array_Iterator_PreOrder &);

protected:
        // internal data
	const BinaryTree_Array<DataType> *tree;
	Stack_List<int> stack;
};

// in-order tree iterator
template <class DataType> class BinaryTree_Array_Iterator_InOrder:
	public BinaryTree_Array_Iterator_PreOrder<DataType>
{
public:
        // constructors and destructor
        BinaryTree_Array_Iterator_InOrder(
		const BinaryTree_Array<DataType> &);
        BinaryTree_Array_Iterator_InOrder(
		const BinaryTree_Array_Iterator_InOrder<DataType> &);
        ~BinaryTree_Array_Iterator_InOrder();

	// reset interator to start
	void reset();

	// advance iterator to next link
	int operator++(int);

private:
	// not allowed
        BinaryTree_Array_Iterator_InOrder();
        BinaryTree_Array_Iterator_InOrder &operator=(const 
		BinaryTree_Array_Iterator_InOrder &);
};

// post-order tree iterator
template <class DataType> class BinaryTree_Array_Iterator_PostOrder:
	public BinaryTree_Array_Iterator_PreOrder<DataType>
{
public:
        // constructors and destructor
        BinaryTree_Array_Iterator_PostOrder(
		const BinaryTree_Array<DataType> &);
        BinaryTree_Array_Iterator_PostOrder(
		const BinaryTree_Array_Iterator_PostOrder<DataType> &);
        ~BinaryTree_Array_Iterator_PostOrder();

	// reset interator to start
	void reset();

	// advance iterator to next link
	int operator++(int);

private:
	// not allowed
        BinaryTree_Array_Iterator_PostOrder();
        BinaryTree_Array_Iterator_PostOrder &operator=(const 
		BinaryTree_Array_Iterator_PostOrder &);

protected:
        // internal data
	Stack_List<int> vstack;
};

// level-order tree iterator
template <class DataType> class BinaryTree_Array_Iterator_LevelOrder:
	public AbstractIterator<DataType>
{
public:
        // constructors and destructor
        BinaryTree_Array_Iterator_LevelOrder(
		const BinaryTree_Array<DataType> &);
        BinaryTree_Array_Iterator_LevelOrder(
		const BinaryTree_Array_Iterator_LevelOrder<DataType> &);
        ~BinaryTree_Array_Iterator_LevelOrder();

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
        BinaryTree_Array_Iterator_LevelOrder();
        BinaryTree_Array_Iterator_LevelOrder &operator=(const 
		BinaryTree_Array_Iterator_LevelOrder &);

protected:
        // internal data
	const BinaryTree_Array<DataType> *tree;
	Queue_List<int> queue;
};

#include <adt/BinaryTree_Array.cpp>

}

#endif
