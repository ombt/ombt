#ifndef __TRIE_H
#define __TRIE_H
// trie class definitions

// headers
#include <stdlib.h>
#include <assert.h>
#include <iostream>
#include <list>

// local headers
#include "gstring.h"

// forward declarations
template <class CharacterType, class PayloadType> class TrieNode;
template <class CharacterType, class PayloadType> class Trie;

// forward declarations of friend functions 
template <class CharacterType, class PayloadType> 
std::ostream &
operator<<(std::ostream &, TrieNode<CharacterType, PayloadType> &);

template <class CharacterType, class PayloadType> 
std::ostream &
operator<<(std::ostream &, Trie<CharacterType, PayloadType> &);

// trie node class
template <class CharacterType, class PayloadType> class TrieNode {
public:
	// output
	friend std::ostream &operator<< <>(std::ostream &os, TrieNode<CharacterType, PayloadType> &);

protected:
        // friend classes 
        friend class Trie<CharacterType, PayloadType>;

        // constructors and destructor
        TrieNode(const CharacterType &);
        TrieNode(const TrieNode<CharacterType, PayloadType> &);
        ~TrieNode();

	// assignment
        TrieNode<CharacterType, PayloadType> &operator=(const TrieNode<CharacterType, PayloadType> &);

        // internal data
	int isaword;
        CharacterType data;
        PayloadType payload;
        TrieNode<CharacterType, PayloadType> *sibling;
        TrieNode<CharacterType, PayloadType> *children;
};

// trie class
template <class CharacterType, class PayloadType> class Trie 
{
public:
        // constructors and destructor
        Trie();
        Trie(const GString<CharacterType> &);
        Trie(const Trie<CharacterType, PayloadType> &);
        ~Trie();

        // assignment
        Trie<CharacterType, PayloadType> &operator=(const Trie<CharacterType, PayloadType> &);
        Trie<CharacterType, PayloadType> &operator=(const GString<CharacterType> &);

        // trie operations
        int insert(const GString<CharacterType> &, const PayloadType &payload);
        int remove(const GString<CharacterType> &, bool recursive = false);
        int includes(const GString<CharacterType> &) const;
        int read(const GString<CharacterType> &, PayloadType &payload) const;
        int update(const GString<CharacterType> &, const PayloadType &payload);
	int getChildren(const GString<CharacterType> &, std::list<CharacterType> &) const;
	int isEmpty() const;
        void clear();

        // miscellaneous
        std::ostream &dump(std::ostream &) const;

	// output
	friend std::ostream &operator<< <>(std::ostream &, Trie<CharacterType, PayloadType> &);

protected:
	// utility functions
        void dump(const TrieNode<CharacterType, PayloadType> *, GString<CharacterType>, std::ostream &) const;
	int insert(TrieNode<CharacterType, PayloadType> *&, const GString<CharacterType> &, int, const PayloadType &);
	int createpath(TrieNode<CharacterType, PayloadType> *&, const GString<CharacterType> &, int, const PayloadType &);
	TrieNode<CharacterType, PayloadType> *copy() const;
	TrieNode<CharacterType, PayloadType> *copy(const TrieNode<CharacterType, PayloadType> *) const;
        void clear(TrieNode<CharacterType, PayloadType> *&);
        int includes(const TrieNode<CharacterType, PayloadType> *, const GString<CharacterType> &, int) const;
        int read(const TrieNode<CharacterType, PayloadType> *, const GString<CharacterType> &, int, PayloadType &) const;
        int getChildren(const TrieNode<CharacterType, PayloadType> *, const GString<CharacterType> &, int, std::list<CharacterType> &) const;
        int update(TrieNode<CharacterType, PayloadType> *, const GString<CharacterType> &, int, const PayloadType &);
	int remove(TrieNode<CharacterType, PayloadType> *&, const GString<CharacterType> &, int, bool recursive);

protected:
        // internal data
        TrieNode<CharacterType, PayloadType> *root;
};

#include "trie.cpp"

#endif

