// member functions for a map class

// required headers
#include "map.h"

// map constructors and destructor
template <class KeyType, class DataType>
Map<KeyType, DataType>::Map():
	map()
{
	// do nothing
}

template <class KeyType, class DataType>
Map<KeyType, DataType>::Map(const Map<KeyType, DataType> &src):
	map()
{
	// since map contains pointers, we have to copy
	// them explicitly, and then insert them in the
	// new map.
	//
	BinaryTreeIterator_LevelOrder<Tuple<KeyType, DataType *> >
		srcmapIter(src.map);
	for ( ; !srcmapIter.done(); srcmapIter++)
	{
		Tuple<KeyType, DataType *> tuple(
			srcmapIter().key, 
			new DataType(*(srcmapIter().data)));
		MustBeTrue(tuple.data != NULL);
		MustBeTrue(map.insert(tuple) == OK);
	}
}

template <class KeyType, class DataType>
Map<KeyType, DataType>::~Map()
{
	clear();
}

// assignment 
template <class KeyType, class DataType>
Map<KeyType, DataType> &
Map<KeyType, DataType>::operator=(const Map<KeyType, DataType> &rhs)
{
	if (this != &rhs)
	{
		// remove current data
		clear();

		// since map contains pointers, we have to copy
		// them explicitly, and then insert them in the
		// new map.
		//
		BinaryTreeIterator_LevelOrder<Tuple<KeyType, DataType *> >
			rhsmapIter(rhs.map);
		for ( ; !rhsmapIter.done(); rhsmapIter++)
		{
			Tuple<KeyType, DataType *> tuple(
				rhsmapIter().key, 
				new DataType(*(rhsmapIter().data)));
			MustBeTrue(tuple.data != NULL);
			MustBeTrue(map.insert(tuple) == OK);
		}
	}
	return(*this);
}

template <class KeyType, class DataType>
DataType &
Map<KeyType, DataType>::operator[](const KeyType &key)
{
	Tuple<KeyType, DataType *> tuple(key, (DataType *)NULL);
	if (map.retrieve(tuple) == OK)
	{
		MustBeTrue(tuple.data != NULL);
		return(*tuple.data);
	}
	else
	{
		tuple.data = new DataType;
		MustBeTrue(tuple.data != NULL);
		MustBeTrue(map.insert(tuple) == OK);
		return(*tuple.data);
	}
}

template <class KeyType, class DataType>
DataType &
Map<KeyType, DataType>::operator[](const KeyType &key) const
{
	Tuple<KeyType, DataType *> tuple(key, (DataType *)NULL);
	MustBeTrue(map.retrieve(tuple) == OK);
	MustBeTrue(tuple.data != NULL);
	return(*tuple.data);
}

// remove one or all entries in map
template <class KeyType, class DataType>
void
Map<KeyType, DataType>::clear()
{
	BinaryTreeIterator_InOrder<Tuple<KeyType, DataType *> > mapIter(map);
	for ( ; ! mapIter.done(); mapIter++)
	{
		if (mapIter().data != NULL)
			delete mapIter().data;
	}
	return;
}

template <class KeyType, class DataType>
int
Map<KeyType, DataType>::remove(const KeyType &key)
{
	Tuple<KeyType, DataType *> tuple(key, (DataType *)NULL);
	if (map.retrieve(tuple) != OK)
		return(NOMATCH);
	if (tuple.data != NULL)
	{
		delete tuple.data;
		tuple.data = NULL;
	}
	return(map.remove(tuple));
}

template <class KeyType, class DataType>
int
Map<KeyType, DataType>::isInMap(const KeyType &key) const
{
	Tuple<KeyType, DataType *> tuple(key, (DataType *)NULL);
	return(map.includes(tuple));
}

// print list item
template <class KeyType, class DataType>
ostream &
operator<<(ostream &os, const Map<KeyType, DataType> &d)
{
	BinaryTreeIterator_InOrder<Tuple<KeyType, DataType *> > dmapIter(d.map);
	for ( ; ! dmapIter.done(); dmapIter++)
	{
		os << "map[" << dmapIter().key << "] = ";
		if (dmapIter().data != NULL)
			os << *(DataType *)(dmapIter().data);
		else
			os << "NULL";
		os << endl;
	}
	return(os);
}

// map iterator constructors and destructor
template <class KeyType, class DataType>
MapIterator<KeyType, DataType>::MapIterator(
	const MapIterator<KeyType, DataType> &mi): 
		mapIter(mi.mapIter)
{
	// do nothing
}

template <class KeyType, class DataType>
MapIterator<KeyType, DataType>::MapIterator(
	const Map<KeyType, DataType> &m): 
		mapIter(m.map)
{
	// do nothing
}

template <class KeyType, class DataType>
MapIterator<KeyType, DataType>::~MapIterator()
{
	// do nothing
}

// reset iterator to beginning
template <class KeyType, class DataType>
void
MapIterator<KeyType, DataType>::reset()
{
	mapIter.reset();
	return;
}

// is list empty
template <class KeyType, class DataType>
int
MapIterator<KeyType, DataType>::done() const
{
	return(mapIter.done());
}

// return current data
template <class KeyType, class DataType>
DataType
MapIterator<KeyType, DataType>::operator()()
{
	return(*(mapIter().data));
}

template <class KeyType, class DataType>
DataType
MapIterator<KeyType, DataType>::data()
{
	return(*(mapIter().data));
}

template <class KeyType, class DataType>
KeyType
MapIterator<KeyType, DataType>::key()
{
	return(mapIter().key);
}

// increment to next data item in list
template <class KeyType, class DataType>
int
MapIterator<KeyType, DataType>::operator++(int)
{
	int status = mapIter++;
	return(status);
}

