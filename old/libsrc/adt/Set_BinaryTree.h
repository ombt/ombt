#ifndef __SET_BINARY_TREE_H
#define __SET_BINARY_TREE_H
// tree-based set class definition

// headers
#include <adt/BinaryTree.h>

namespace ombt {

// forward declarations
template <class DataType> class Set_BinaryTree;
template <class DataType> class Set_BinaryTree_Iterator;
#if USEOSTREAM
template <class DataType>
std::ostream &
operator<<(std::ostream &, const Set_BinaryTree<DataType> &);
#endif

// binary-tree--based set class
template <class DataType> class Set_BinaryTree {
public:
	// friends
	friend class Set_BinaryTree_Iterator<DataType>;

        // destructor
        Set_BinaryTree();
        Set_BinaryTree(const Set_BinaryTree<DataType> &);
        ~Set_BinaryTree();

        // insert member operations
        Set_BinaryTree<DataType> &operator+=(const DataType &);
        Set_BinaryTree<DataType> operator+(const DataType &) const;
        Set_BinaryTree<DataType> &insert(const DataType &);

        // delete member operations
        Set_BinaryTree<DataType> &operator-=(DataType &);
        Set_BinaryTree<DataType> operator-(DataType &) const;
        Set_BinaryTree<DataType> &remove(DataType &);
        Set_BinaryTree<DataType> &clear();

        // equality and assignment set operations
        Set_BinaryTree<DataType> &operator=(const Set_BinaryTree<DataType> &);
        int operator==(const Set_BinaryTree<DataType> &) const;
        int operator!=(const Set_BinaryTree<DataType> &) const;
        int isMember(const DataType &) const;
        int isEmpty() const;

        // union set operations
        Set_BinaryTree<DataType> &operator|=(const Set_BinaryTree<DataType> &);
        Set_BinaryTree<DataType> operator|(const Set_BinaryTree<DataType> &) const;

        // intersection set operations
        Set_BinaryTree<DataType> &operator&=(const Set_BinaryTree<DataType> &);
        Set_BinaryTree<DataType> operator&(const Set_BinaryTree<DataType> &) const;

        // difference set operations
        Set_BinaryTree<DataType> &operator-=(const Set_BinaryTree<DataType> &);
        Set_BinaryTree<DataType> operator-(const Set_BinaryTree<DataType> &) const;

	// check if another set is a subset.
	int isSubsetOf(const Set_BinaryTree<DataType> &) const;
	int isSupersetOf(const Set_BinaryTree<DataType> &) const;
	int isProperSubsetOf(const Set_BinaryTree<DataType> &) const;
	int isProperSupersetOf(const Set_BinaryTree<DataType> &) const;

#if USEOSTREAM
	// output data
	template <typename DT> friend std::ostream &operator<<(std::ostream &, const Set_BinaryTree<DT> &);
#endif

protected:
	// data
	BinaryTree<DataType> btree;
};

// binary-tree-based set iterator class
template <class DataType> class Set_BinaryTree_Iterator
{
public:
        // constructors and destructor
        Set_BinaryTree_Iterator(const Set_BinaryTree<DataType> &);
        Set_BinaryTree_Iterator(const Set_BinaryTree_Iterator<DataType> &);
        ~Set_BinaryTree_Iterator();

	// reset iterator to start
	void reset();

	// check if at end of set
	int done() const;

	// return current set member
        DataType operator()();

	// advance to next set member
	int operator++(int);

private:
	// not allowed
        Set_BinaryTree_Iterator &operator=(const Set_BinaryTree_Iterator<DataType> &);

protected:
	// internal binary tree iterator
	BinaryTreeIterator_InOrder<DataType> iterator;
};

#include <adt/Set_BinaryTree.cpp>

}

#endif
