#ifndef __OMBT_ALPHABET_H
#define __OMBT_ALPHABET_H
// alphabet and word class definitions

// headers
#include <stdlib.h>
#include <iostream>

// local headers
#include <Returns.h>
#include <Debug.h>
#include <adt/AbsIterator.h>
#include <adt/List.h>
#include <adt/BinaryTree_AVL.h>
#include <adt/Tuple.h>

namespace ombt {

// forward declarations
template <class DataType> class Alphabet;
template <class DataType> class AlphabetIterator;
template <class DataType> class Word;
template <class DataType> class WordIterator;
#if 0
template <class DataType> class TrieNode;
template <class DataType> class Trie;
template <class DataType> class TrieIterator;
#endif

#ifdef USEOSTREAM
// forward declarations of friend functions 
template <class DataType> 
std::ostream &
operator<<(std::ostream &, const Alphabet<DataType> &);

template <class DataType> 
std::ostream &
operator<<(std::ostream &, const Word<DataType> &);
#endif

// alphabet class
template <class DataType> class Alphabet {
public:
        // friend classes and functions
        friend class AlphabetIterator<DataType>;
        friend class Word<DataType>;

        // constructors and destructor
        Alphabet(const List<DataType> &);
        Alphabet(const Alphabet<DataType> &);
        ~Alphabet();

        // assignment 
        Alphabet<DataType> &operator=(const Alphabet<DataType> &);

	// predicates
	int includes(const DataType &) const;

	// comparison operators. I can't overload operators
	// since DataType is most likely a primitive and the 
	// comparison operators for primitive data types are
	// defined with the language. so we do it this way.
	//
	int eq(const DataType &, const DataType &) const;
	int neq(const DataType &, const DataType &) const;
	int lt(const DataType &, const DataType &) const;
	int ltoreq(const DataType &, const DataType &) const;
	int gt(const DataType &, const DataType &) const;
	int gtoreq(const DataType &, const DataType &) const;

#ifdef USEOSTREAM
	// output operators
        template <typename DT> friend std::ostream &operator<<(std::ostream &, const Alphabet<DT> &);
        template <typename DT> friend std::ostream &operator<<(std::ostream &, const Word<DT> &);
#endif

protected:
        // internal data
	int size;
	List<DataType> set;
	BinaryTree_AVL<Tuple<DataType, int> > orderedset;
};

// alphabet iterator
template <class DataType> class AlphabetIterator:
	public AbstractIterator<DataType>
{
public:
        // constructors and destructor
        AlphabetIterator(const AlphabetIterator<DataType> &);
        AlphabetIterator(const Alphabet<DataType> &);
        ~AlphabetIterator();

        // initialization
	void reset();

	// check if at end of list
	int done() const;

        // return data 
        DataType operator()();

	// advance iterator to next link
	int operator++(int);

private:
	// not allowed
        AlphabetIterator();
	AlphabetIterator<DataType> &operator=(const AlphabetIterator<DataType> &);

protected:
        // internal data
	const Alphabet<DataType> &alphabet;
        ListIterator<DataType> alphabetIter;
};

// word class definition
template <class DataType> class Word {
public:
	// friend classes
        friend class WordIterator<DataType>;

	// constructors and destructor
	Word(const Alphabet<DataType> &);
	Word(const Alphabet<DataType> &, DataType);
	Word(const Alphabet<DataType> &, int, const DataType * = NULL);
	Word(const Word<DataType> &);
	~Word();

	// assignment
	Word<DataType> &operator=(const Word<DataType> &);

	// access operator
	DataType &operator[](int);
	const DataType &operator[](int) const;

	// logical operators
	int operator==(const Word<DataType> &) const;
	int operator!=(const Word<DataType> &) const;
	int operator<(const Word<DataType> &) const;
	int operator<=(const Word<DataType> &) const;
	int operator>(const Word<DataType> &) const;
	int operator>=(const Word<DataType> &) const;

	// substring operator
	Word<DataType> operator()(int) const;
	Word<DataType> operator()(int, int) const;

	// concatenation operators
	Word<DataType> &operator+=(const Word<DataType> &);
	Word<DataType> operator+(const Word<DataType> &) const;

	// string length
	int wordlen() const;
	inline int getMaxSize() const {
		return(maxsize);
	}

	// casting operation
	operator const DataType *() const;
	operator DataType *();

#ifdef USEOSTREAM
	// input and output
	template <typename DT> friend std::ostream &operator<<(std::ostream &, const Word<DT> &);
#endif

private:
	// utility functions
	int wordcmp(const Word<DataType> &) const;
	void wordncpy(DataType *, const DataType *, int) const;

protected:
	const Alphabet<DataType> &alphabet;
	int size;
	int maxsize;
	DataType *buffer;
};

// word iterator
template <class DataType> class WordIterator:
	public AbstractIterator<DataType>
{
public:
        // constructors and destructor
        WordIterator(const WordIterator<DataType> &);
        WordIterator(const Word<DataType> &);
        ~WordIterator();

        // initialization
	void reset();

	// check if at end of list
	int done() const;

        // return data 
        DataType operator()();

	// advance iterator to next link
	int operator++(int);

	// get current index
	int getIndex() const;

private:
	// not allowed
        WordIterator();
	WordIterator &operator=(const WordIterator<DataType> &);

protected:
        // internal data
	const Word<DataType> &word;
	int current;
};

#include <adt/Alphabet.cpp>

#if 0
// trie node class
template <class DataType> class TrieNode {
public:
	// output
	friend ostream &operator<<(ostream &os, 
		const TrieNode<DataType> &btn) {
		os << btn.data;
		return(os);
	};

protected:
        // friend classes 
        friend class Trie<DataType>;
	friend class TrieIterator<DataType>;

        // constructors and destructor
        Trie(const DataType &);
        Trie(const TrieNode &);
        ~Trie();

	// assignment
        Trie &operator=(const Trie &);

        // internal data
        DataType data;
        TrieNode<DataType> *parent;
        TrieNode<DataType> *children;
};

// trie class
template <class DataType> class Trie 
{
public:
	// friend classes 
	friend class TrieIterator<DataType>;

        // constructors and destructor
        Trie();
        Trie(const Trie &);
        ~Trie();

        // assignment
        Trie &operator=(const Trie &);

        // binary tree operations
        int insert(const DataType &);
        int remove(DataType &);
        int retrieve(DataType &) const;
        int includes(const DataType &) const;
	int isEmpty() const;
        void clear();

        // miscellaneous
        ostream &dump(ostream &) const;

protected:
	// utility functions
        void dump(BinaryTreeNode<DataType> *, ostream &) const;

protected:
        // internal data
        TrieNode<Word<DataType> > *root;
};

// depth-first trie iterator
template <class DataType> class TrieIterator:
	public AbstractIterator<DataType>
{
public:
        // constructors and destructor
        TrieIterator(const Trie<DataType> &);
        TrieIterator(const TrieIterator<DataType> &);
        ~TrieIterator();

	// reset iterator to start
	void reset();

	// check if at end of list
	int done() const;

        // return data 
        DataType operator()();

	// advance iterator to next link
	int operator++(int);

private:
	// not allowed
        TrieIterator();
        TrieIterator &operator=(const TrieIterator<DataType> &);

protected:
        // internal data
	const BinaryTree<DataType> *tree;
	Stack_List<BinaryTreeNode<DataType> * > stack;
};

// level-order tree iterator
template <class DataType> class BinaryTreeIterator_LevelOrder:
	public AbstractIterator<DataType>
{
public:
        // constructors and destructor
        BinaryTreeIterator_LevelOrder(
		const BinaryTree<DataType> &);
        BinaryTreeIterator_LevelOrder(
		const BinaryTreeIterator_LevelOrder<DataType> &);
        ~BinaryTreeIterator_LevelOrder();

	// reset iterator to start
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
	const BinaryTree<DataType> *tree;
	Queue_List<BinaryTreeNode<DataType> * > queue;
};

#endif

}

#endif

