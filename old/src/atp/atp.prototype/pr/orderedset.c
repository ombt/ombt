// member functions for a ordered set class

// required headers
#include "orderedset.h"

// ordered set constructors and destructor
template <class KeyType>
OrderedSet<KeyType>::OrderedSet():
	oset()
{
	// do nothing
}

template <class KeyType>
OrderedSet<KeyType>::OrderedSet(const OrderedSet<KeyType> &src):
	oset()
{
	// since the set contains pointers, we have to copy
	// them explicitly, and then insert them in the
	// new set.
	//
	BinaryTreeIterator_LevelOrder<Tuple<KeyType, List<KeyType> * > >
		srcIter(src.oset);
	for ( ; !srcIter.done(); srcIter++)
	{
		Tuple<KeyType, List<KeyType> * > tuple(
			srcIter().key, 
			new List<KeyType>(*(srcIter().data)));
		MustBeTrue(tuple.data != NULL);
		MustBeTrue(oset.insert(tuple) == OK);
	}
}

template <class KeyType>
OrderedSet<KeyType>::~OrderedSet()
{
	clear();
}

// assignment 
template <class KeyType>
OrderedSet<KeyType> &
OrderedSet<KeyType>::operator=(const OrderedSet<KeyType> &rhs)
{
	if (this != &rhs)
	{
		// remove current data
		clear();

		// since the set contains pointers, we have to copy
		// them explicitly, and then insert them in the
		// new set.
		//
		BinaryTreeIterator_LevelOrder<Tuple<KeyType, List<KeyType> * > >
			rhsIter(rhs.oset);
		for ( ; !rhsIter.done(); rhsIter++)
		{
			Tuple<KeyType, List<KeyType> * > tuple(
				rhsIter().key, 
				new List<KeyType>(*(rhsIter().data)));
			MustBeTrue(tuple.data != NULL);
			MustBeTrue(oset.insert(tuple) == OK);
		}
	}
	return(*this);
}

// access operations
template <class KeyType>
int
OrderedSet<KeyType>::insert(const KeyType &key)
{
	Tuple<KeyType, List<KeyType> * > tuple(key, (List<KeyType> *)NULL);
	if (oset.retrieve(tuple) == OK)
	{
		MustBeTrue(tuple.data != NULL);
		return(tuple.data->insertAtEnd(key));
	}
	else
	{
		tuple.data = new List<KeyType>();
		MustBeTrue(tuple.data != NULL);
		if (tuple.data->insertAtEnd(key) != OK)
			return(NOTOK);
		if (oset.insert(tuple) != OK)
			return(NOTOK);
		return(OK);
	}
}

template <class KeyType>
int
OrderedSet<KeyType>::retrieve(KeyType &key, int idx) const
{
	Tuple<KeyType, List<KeyType> * > tuple(key, (List<KeyType> *)NULL);
	switch (oset.retrieve(tuple))
	{
	case OK:
		MustBeTrue(tuple.data != NULL);
		return(tuple.data->retrieveNth(idx, key));

	case NOMATCH:
		return(NOMATCH);

	default:
		return(NOTOK);
	}
}

template <class KeyType>
int
OrderedSet<KeyType>::retrieve(KeyType &key, List<KeyType> &data) const
{
	Tuple<KeyType, List<KeyType> * > tuple(key, (List<KeyType> *)NULL);
	switch (oset.retrieve(tuple))
	{
	case OK:
		MustBeTrue(tuple.data != NULL);
		data = *tuple.data;
		return(OK);

	case NOMATCH:
		return(NOMATCH);

	default:
		return(NOTOK);
	}
}

template <class KeyType>
int
OrderedSet<KeyType>::update(const KeyType &key, int idx)
{
	Tuple<KeyType, List<KeyType> * > tuple(key, (List<KeyType> *)NULL);
	switch (oset.retrieve(tuple))
	{
	case OK:
		MustBeTrue(tuple.data != NULL);
		return(tuple.data->updateNth(idx, key));

	case NOMATCH:
		return(NOMATCH);

	default:
		return(NOTOK);
	}
}

template <class KeyType>
int
OrderedSet<KeyType>::update(const KeyType &key, List<KeyType> &data) 
{
	Tuple<KeyType, List<KeyType> * > tuple(key, (List<KeyType> *)NULL);
	switch (oset.retrieve(tuple))
	{
	case OK:
		MustBeTrue(tuple.data != NULL);
		*tuple.data = data;
		return(OK);

	case NOMATCH:
		return(NOMATCH);

	default:
		return(NOTOK);
	}
}

template <class KeyType>
int
OrderedSet<KeyType>::remove(const KeyType &key)
{
	Tuple<KeyType, List<KeyType> * > tuple(key, (List<KeyType> *)NULL);
	switch (oset.retrieve(tuple))
	{
	case OK:
		MustBeTrue(tuple.data != NULL);
		delete tuple.data;
		tuple.data = NULL;
		return(oset.remove(tuple));

	case NOMATCH:
		return(NOMATCH);

	default:
		return(NOTOK);
	}
}

template <class KeyType>
int
OrderedSet<KeyType>::remove(const KeyType &key, int idx)
{
	KeyType keycopy(key);
	Tuple<KeyType, List<KeyType> * > tuple(keycopy, (List<KeyType> *)NULL);
	switch (oset.retrieve(tuple))
	{
	case OK:
		MustBeTrue(tuple.data != NULL);
		return(tuple.data->removeNth(idx, keycopy));

	case NOMATCH:
		return(NOMATCH);

	default:
		return(NOTOK);
	}
}

template <class KeyType>
int
OrderedSet<KeyType>::isInSet(const KeyType &key) const
{
	Tuple<KeyType, List<KeyType> * > tuple(key, (List<KeyType> *)NULL);
	if (oset.retrieve(tuple) == OK)
		return(1);
	else
		return(0);
}

// return size of entire set or a specific equivalence set.
template <class KeyType>
int
OrderedSet<KeyType>::getSize(const KeyType &key) const
{
	Tuple<KeyType, List<KeyType> * > tuple(key, (List<KeyType> *)NULL);
	if (oset.retrieve(tuple) == OK)
	{
		MustBeTrue(tuple.data != NULL);
		return(tuple.data->getCount());
	}
	else
		return(0);
}

template <class KeyType>
int
OrderedSet<KeyType>::getSize() const
{
	long total;
	BinaryTreeIterator_InOrder<
		Tuple<KeyType, List<KeyType> * > > osetIter(oset);
	for (total=0; !osetIter.done(); osetIter++)
	{
		total += osetIter().data->getCount();
	}
	return(total);
}

template <class KeyType>
int
OrderedSet<KeyType>::numberOfEquivalentSets() const
{
	long total;
	BinaryTreeIterator_InOrder<
		Tuple<KeyType, List<KeyType> * > > osetIter(oset);
	for (total=0; !osetIter.done(); osetIter++)
	{
		total += 1;
	}
	return(total);
}

// remove all entries in the ordered set
template <class KeyType>
void
OrderedSet<KeyType>::clear()
{
	BinaryTreeIterator_InOrder<Tuple<KeyType, List<KeyType> * > > osetIter(oset);
	for ( ; ! osetIter.done(); osetIter++)
	{
		if (osetIter().data != NULL)
			delete osetIter().data;
	}
	oset.clear();
	return;
}

// print list item
template <class KeyType>
ostream &
operator<<(ostream &os, const OrderedSet<KeyType> &d)
{
	BinaryTreeIterator_InOrder<Tuple<KeyType, List<KeyType> * > > dosetIter(d.oset);
	for ( ; ! dosetIter.done(); dosetIter++)
	{
		os << "oset[" << dosetIter().key << "] = ";
		if (dosetIter().data != NULL)
			os << *dosetIter().data;
		else
			os << "NULL";
		os << endl;
	}
	return(os);
}

// oordered set iterator constructors and destructor
template <class KeyType>
OrderedSetIterator<KeyType>::OrderedSetIterator(
	const OrderedSetIterator<KeyType> &mi): 
		osetIter(mi.osetIter)
{
	// do nothing
}

template <class KeyType>
OrderedSetIterator<KeyType>::OrderedSetIterator(
	const OrderedSet<KeyType> &os): 
		osetIter(os.oset)
{
	// do nothing
}

template <class KeyType>
OrderedSetIterator<KeyType>::~OrderedSetIterator()
{
	// do nothing
}

// reset iterator to beginning
template <class KeyType>
void
OrderedSetIterator<KeyType>::reset()
{
	osetIter.reset();
	return;
}

// is list empty
template <class KeyType>
int
OrderedSetIterator<KeyType>::done() const
{
	return(osetIter.done());
}

// return current data
template <class KeyType>
List<KeyType>
OrderedSetIterator<KeyType>::operator()()
{
	return(*(osetIter().data));
}

template <class KeyType>
List<KeyType>
OrderedSetIterator<KeyType>::data()
{
	return(*(osetIter().data));
}

template <class KeyType>
KeyType
OrderedSetIterator<KeyType>::key()
{
	return(osetIter().key);
}

// increment to next data item in list
template <class KeyType>
int
OrderedSetIterator<KeyType>::operator++(int)
{
	int status = osetIter++;
	return(status);
}

