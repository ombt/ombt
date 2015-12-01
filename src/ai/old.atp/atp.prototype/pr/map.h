#ifndef __MAP_H
#define __MAP_H
// map class definition

// headers
#include <stdlib.h>
#include <iostream.h>

// local headers
#include "returns.h"
#include "debug.h"
#include "absIterator.h"
#include "tuple.h"
#include "binaryTree.h"

// forward declarations
template <class KeyType, class DataType> class Map;
template <class KeyType, class DataType> class MapIterator;

// forward declaration of friend functions
template <class KeyType, class DataType> 
ostream &
operator<<(ostream &, const Map<KeyType, DataType> &);

// map class
template <class KeyType, class DataType> class Map {
public:
	// friend classes
	friend class MapIterator<KeyType, DataType>;

        // constructors and destructor
        Map();
        Map(const Map<KeyType, DataType> &);
        ~Map();

        // operators
        Map<KeyType, DataType> &operator=(const Map<KeyType, DataType> &);
        DataType &operator[](const KeyType &);
        DataType &operator[](const KeyType &) const;

	// other functions
	int isInMap(const KeyType &) const;
	void clear();
	int remove(const KeyType &);

	// print arrays
        friend ostream &operator<<(ostream &, const Map<KeyType, DataType> &);

protected:
        // internal data
	BinaryTree<Tuple<KeyType, DataType *> > map;
};

// map iterator
template <class KeyType, class DataType> class MapIterator:
	public AbstractIterator<DataType>
{
public:
        // constructors and destructor
        MapIterator(const MapIterator<KeyType, DataType> &);
        MapIterator(const Map<KeyType, DataType> &);
        ~MapIterator();

        // initialization
	void reset();

	// check if at end of list
	int done() const;

        // return data 
        KeyType key();
        DataType data();
        DataType operator()();

	// advance iterator to next link
	int operator++(int);

	// get current index
	int getIndex() const;

private:
	// not allowed
        MapIterator();
	MapIterator<KeyType, DataType> &
		operator=(const MapIterator<KeyType, DataType> &);

protected:
        // internal data
	BinaryTreeIterator_InOrder<Tuple<KeyType, DataType * > > mapIter;
};

#endif
