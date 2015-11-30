#ifndef __BINARY_TREE_AVL_H
#define __BINARY_TREE_AVL_H
// binary AVL search tree class definition using recursion

// headers
#include "absTree.h"
#include "absIterator.h"
#include "queue_List.h"
#include "stack_List.h"

// forward declarations
template <class DataType> class BinaryTreeNode_AVL;
template <class DataType> class BinaryTree_AVL;
template <class DataType> class BinaryTree_AVL_Iterator_PreOrder;
template <class DataType> class BinaryTree_AVL_Iterator_InOrder;
template <class DataType> class BinaryTree_AVL_Iterator_PostOrder;
template <class DataType> class BinaryTree_AVL_Iterator_LevelOrder;

// tree node class
template <class DataType> class BinaryTreeNode_AVL
{
public:
	// output
	friend ostream &operator<<(ostream &os, 
		const BinaryTreeNode_AVL<DataType> &btn) {
		os << btn.data;
		return(os);
	};

protected:
        // friend classes 
        friend class BinaryTree_AVL<DataType>;
	friend class BinaryTree_AVL_Iterator_PreOrder<DataType>;
	friend class BinaryTree_AVL_Iterator_InOrder<DataType>;
	friend class BinaryTree_AVL_Iterator_PostOrder<DataType>;
	friend class BinaryTree_AVL_Iterator_LevelOrder<DataType>;

        // constructors and destructor
        BinaryTreeNode_AVL(const DataType &);
        BinaryTreeNode_AVL(const BinaryTreeNode_AVL &);
        ~BinaryTreeNode_AVL();

	// assignment
        BinaryTreeNode_AVL &operator=(const BinaryTreeNode_AVL &);

        // internal data
	int balance;
        DataType data;
        BinaryTreeNode_AVL<DataType> *left;
        BinaryTreeNode_AVL<DataType> *right;
};

// binary search tree class
template <class DataType> class BinaryTree_AVL:
	public AbstractTree<DataType>
{
public:
	// friend classes 
	friend class BinaryTree_AVL_Iterator_PreOrder<DataType>;
	friend class BinaryTree_AVL_Iterator_InOrder<DataType>;
	friend class BinaryTree_AVL_Iterator_PostOrder<DataType>;
	friend class BinaryTree_AVL_Iterator_LevelOrder<DataType>;

        // constructors and destructor
        BinaryTree_AVL();
        BinaryTree_AVL(const BinaryTree_AVL &);
        ~BinaryTree_AVL();

        // assignment
        BinaryTree_AVL &operator=(const BinaryTree_AVL &);

        // binary tree operations
        int insert(const DataType &);
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
        ostream &dump(ostream &) const;
	void audit() const;

protected:
	// utility functions
        BinaryTreeNode_AVL<DataType> *copy() const;
        BinaryTreeNode_AVL<DataType> *
		copy(const BinaryTreeNode_AVL<DataType> *) const;
	int retrieve(const BinaryTreeNode_AVL<DataType> *, DataType &) const;
        void clear(BinaryTreeNode_AVL<DataType> *&);
	int insert(BinaryTreeNode_AVL<DataType> *&, const DataType &, int &);
	int remove(BinaryTreeNode_AVL<DataType> *&, DataType &, int &);
	int removeRightMost(BinaryTreeNode_AVL<DataType> *&, DataType &, int &);
	void rotateLeft(BinaryTreeNode_AVL<DataType> *&);
	void rotateRight(BinaryTreeNode_AVL<DataType> *&);
	void fixLeftTreeUponInsert(BinaryTreeNode_AVL<DataType> *&);
	void fixRightTreeUponInsert(BinaryTreeNode_AVL<DataType> *&);
	void fixLeftTreeUponDelete(BinaryTreeNode_AVL<DataType> *&, int &);
	void fixRightTreeUponDelete(BinaryTreeNode_AVL<DataType> *&, int &);
	int audit(const BinaryTreeNode_AVL<DataType> *) const;
        int includes(const BinaryTreeNode_AVL<DataType> *, const DataType &) const;
        int preOrder(BinaryTreeNode_AVL<DataType> *, int (*)(DataType &));
        int inOrder(BinaryTreeNode_AVL<DataType> *, int (*)(DataType &));
        int postOrder(BinaryTreeNode_AVL<DataType> *, int (*)(DataType &));
        void dump(BinaryTreeNode_AVL<DataType> *, ostream &) const;


protected:
        // internal data
        BinaryTreeNode_AVL<DataType> *root;
};

// pre-order tree iterator
template <class DataType> class BinaryTree_AVL_Iterator_PreOrder:
	public AbstractIterator<DataType>
{
public:
        // constructors and destructor
        BinaryTree_AVL_Iterator_PreOrder(
		const BinaryTree_AVL<DataType> &);
        BinaryTree_AVL_Iterator_PreOrder(
		const BinaryTree_AVL_Iterator_PreOrder<DataType> &);
        ~BinaryTree_AVL_Iterator_PreOrder();

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
        BinaryTree_AVL_Iterator_PreOrder();
        BinaryTree_AVL_Iterator_PreOrder &operator=(const 
		BinaryTree_AVL_Iterator_PreOrder &);

protected:
        // internal data
	const BinaryTree_AVL<DataType> *tree;
	Stack_List<BinaryTreeNode_AVL<DataType> * > stack;
};

// in-order tree iterator
template <class DataType> class BinaryTree_AVL_Iterator_InOrder:
	public BinaryTree_AVL_Iterator_PreOrder<DataType>
{
public:
        // constructors and destructor
        BinaryTree_AVL_Iterator_InOrder(
		const BinaryTree_AVL<DataType> &);
        BinaryTree_AVL_Iterator_InOrder(
		const BinaryTree_AVL_Iterator_InOrder<DataType> &);
        ~BinaryTree_AVL_Iterator_InOrder();

	// reset interator to start
	void reset();

	// advance iterator to next link
	int operator++(int);

private:
	// not allowed
        BinaryTree_AVL_Iterator_InOrder();
        BinaryTree_AVL_Iterator_InOrder &operator=(const 
		BinaryTree_AVL_Iterator_InOrder &);
};

// post-order tree iterator
template <class DataType> class BinaryTree_AVL_Iterator_PostOrder:
	public BinaryTree_AVL_Iterator_PreOrder<DataType>
{
public:
        // constructors and destructor
        BinaryTree_AVL_Iterator_PostOrder(
		const BinaryTree_AVL<DataType> &);
        BinaryTree_AVL_Iterator_PostOrder(
		const BinaryTree_AVL_Iterator_PostOrder<DataType> &);
        ~BinaryTree_AVL_Iterator_PostOrder();

	// reset interator to start
	void reset();

	// advance iterator to next link
	int operator++(int);

private:
	// not allowed
        BinaryTree_AVL_Iterator_PostOrder();
        BinaryTree_AVL_Iterator_PostOrder &operator=(const 
		BinaryTree_AVL_Iterator_PostOrder &);

protected:
        // internal data
	Stack_List<int> vstack;
};

// level-order tree iterator
template <class DataType> class BinaryTree_AVL_Iterator_LevelOrder:
	public AbstractIterator<DataType>
{
public:
        // constructors and destructor
        BinaryTree_AVL_Iterator_LevelOrder(
		const BinaryTree_AVL<DataType> &);
        BinaryTree_AVL_Iterator_LevelOrder(
		const BinaryTree_AVL_Iterator_LevelOrder<DataType> &);
        ~BinaryTree_AVL_Iterator_LevelOrder();

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
        BinaryTree_AVL_Iterator_LevelOrder();
        BinaryTree_AVL_Iterator_LevelOrder &operator=(const 
		BinaryTree_AVL_Iterator_LevelOrder &);

protected:
        // internal data
	const BinaryTree_AVL<DataType> *tree;
	Queue_List<BinaryTreeNode_AVL<DataType> * > queue;
};

#endif

