#ifndef __BINARY_TREE_NR_H
#define __BINARY_TREE_NR_H
// binary search tree class definition

// headers
#include <adt/AbsTree.h>
#include <adt/AbsIterator.h>
#include <adt/Queue_List.h>
#include <adt/Stack_List.h>

namespace ombt {

// forward declarations
template <class DataType> class BinaryTreeNode_NR;
template <class DataType> class BinaryTree_NR;
template <class DataType> class BinaryTree_NR_Iterator_PreOrder;
template <class DataType> class BinaryTree_NR_Iterator_InOrder;
template <class DataType> class BinaryTree_NR_Iterator_PostOrder;
template <class DataType> class BinaryTree_NR_Iterator_LevelOrder;

// tree node class
template <class DataType> class BinaryTreeNode_NR
{
public:
#ifdef USEOSTREAM
	// output
	friend std::ostream &operator<<(std::ostream &os, 
		const BinaryTreeNode_NR<DataType> &btn) {
		os << btn.data;
		return(os);
	};
#endif

protected:
        // friend classes 
        friend class BinaryTree_NR<DataType>;
	friend class BinaryTree_NR_Iterator_PreOrder<DataType>;
	friend class BinaryTree_NR_Iterator_InOrder<DataType>;
	friend class BinaryTree_NR_Iterator_PostOrder<DataType>;
	friend class BinaryTree_NR_Iterator_LevelOrder<DataType>;

        // constructors and destructor
        BinaryTreeNode_NR(const DataType &);
        BinaryTreeNode_NR(const BinaryTreeNode_NR &);
        ~BinaryTreeNode_NR();

	// assignment
        BinaryTreeNode_NR &operator=(const BinaryTreeNode_NR &);

        // internal data
        DataType data;
        BinaryTreeNode_NR<DataType> *left;
        BinaryTreeNode_NR<DataType> *right;
};

// binary search tree class
template <class DataType> class BinaryTree_NR:
	public AbstractTree<DataType>
{
public:
	// friend classes 
	friend class BinaryTree_NR_Iterator_PreOrder<DataType>;
	friend class BinaryTree_NR_Iterator_InOrder<DataType>;
	friend class BinaryTree_NR_Iterator_PostOrder<DataType>;
	friend class BinaryTree_NR_Iterator_LevelOrder<DataType>;

        // constructors and destructor
        BinaryTree_NR();
        BinaryTree_NR(const BinaryTree_NR &);
        ~BinaryTree_NR();

        // assignment
        BinaryTree_NR &operator=(const BinaryTree_NR &);

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
        BinaryTreeNode_NR<DataType> *copy() const;

protected:
        // internal data
        BinaryTreeNode_NR<DataType> *root;
};

// pre-order tree iterator
template <class DataType> class BinaryTree_NR_Iterator_PreOrder:
	public AbstractIterator<DataType>
{
public:
        // constructors and destructor
        BinaryTree_NR_Iterator_PreOrder(
		const BinaryTree_NR<DataType> &);
        BinaryTree_NR_Iterator_PreOrder(
		const BinaryTree_NR_Iterator_PreOrder<DataType> &);
        ~BinaryTree_NR_Iterator_PreOrder();

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
        BinaryTree_NR_Iterator_PreOrder();
        BinaryTree_NR_Iterator_PreOrder &operator=(const 
		BinaryTree_NR_Iterator_PreOrder &);

protected:
        // internal data
	const BinaryTree_NR<DataType> *tree;
	Stack_List<BinaryTreeNode_NR<DataType> * > stack;
};

// in-order tree iterator
template <class DataType> class BinaryTree_NR_Iterator_InOrder:
	public BinaryTree_NR_Iterator_PreOrder<DataType>
{
public:
        // constructors and destructor
        BinaryTree_NR_Iterator_InOrder(
		const BinaryTree_NR<DataType> &);
        BinaryTree_NR_Iterator_InOrder(
		const BinaryTree_NR_Iterator_InOrder<DataType> &);
        ~BinaryTree_NR_Iterator_InOrder();

	// reset interator to start
	void reset();

	// advance iterator to next link
	int operator++(int);

private:
	// not allowed
        BinaryTree_NR_Iterator_InOrder();
        BinaryTree_NR_Iterator_InOrder &operator=(const 
		BinaryTree_NR_Iterator_InOrder &);
};

// post-order tree iterator
template <class DataType> class BinaryTree_NR_Iterator_PostOrder:
	public BinaryTree_NR_Iterator_PreOrder<DataType>
{
public:
        // constructors and destructor
        BinaryTree_NR_Iterator_PostOrder(
		const BinaryTree_NR<DataType> &);
        BinaryTree_NR_Iterator_PostOrder(
		const BinaryTree_NR_Iterator_PostOrder<DataType> &);
        ~BinaryTree_NR_Iterator_PostOrder();

	// reset interator to start
	void reset();

	// advance iterator to next link
	int operator++(int);

private:
	// not allowed
        BinaryTree_NR_Iterator_PostOrder();
        BinaryTree_NR_Iterator_PostOrder &operator=(const 
		BinaryTree_NR_Iterator_PostOrder &);

protected:
        // internal data
	Stack_List<int> vstack;
};

// level-order tree iterator
template <class DataType> class BinaryTree_NR_Iterator_LevelOrder:
	public AbstractIterator<DataType>
{
public:
        // constructors and destructor
        BinaryTree_NR_Iterator_LevelOrder(
		const BinaryTree_NR<DataType> &);
        BinaryTree_NR_Iterator_LevelOrder(
		const BinaryTree_NR_Iterator_LevelOrder<DataType> &);
        ~BinaryTree_NR_Iterator_LevelOrder();

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
        BinaryTree_NR_Iterator_LevelOrder();
        BinaryTree_NR_Iterator_LevelOrder &operator=(const 
		BinaryTree_NR_Iterator_LevelOrder &);

protected:
        // internal data
	const BinaryTree_NR<DataType> *tree;
	Queue_List<BinaryTreeNode_NR<DataType> * > queue;
};

#include <adt/BinaryTree_NR.cpp>

}
#endif
