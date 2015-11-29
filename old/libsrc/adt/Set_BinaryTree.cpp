// member functions for binary-tree-based set class

// set constructors and destructor
template <class DataType>
Set_BinaryTree<DataType>::Set_BinaryTree():
	btree()
{
	// do nothing
}

template <class DataType>
Set_BinaryTree<DataType>::Set_BinaryTree(const Set_BinaryTree<DataType> &sl):
	btree(sl.btree)
{
	// do nothing
}

template <class DataType>
Set_BinaryTree<DataType>::~Set_BinaryTree()
{
	// do nothing
}

// insert a new member operators
template <class DataType>
Set_BinaryTree<DataType> &
Set_BinaryTree<DataType>::operator+=(const DataType &m)
{
	return(insert(m));
}

template <class DataType>
Set_BinaryTree<DataType> 
Set_BinaryTree<DataType>::operator+(const DataType &m) const
{
	return(Set_BinaryTree<DataType>(*this) += m);
}

template <class DataType>
Set_BinaryTree<DataType> &
Set_BinaryTree<DataType>::insert(const DataType &m)
{
	btree.insert(m);
	return(*this);
}

// remove a member operators
template <class DataType>
Set_BinaryTree<DataType> &
Set_BinaryTree<DataType>::operator-=(DataType &m)
{
	return(remove(m));
}

template <class DataType>
Set_BinaryTree<DataType> 
Set_BinaryTree<DataType>::operator-(DataType &m) const
{
	return(Set_BinaryTree<DataType>(*this) -= m);
}

template <class DataType>
Set_BinaryTree<DataType> &
Set_BinaryTree<DataType>::remove(DataType &m)
{
	btree.remove(m);
	return(*this);
}

template <class DataType>
Set_BinaryTree<DataType> &
Set_BinaryTree<DataType>::clear()
{
	btree.clear();
	return(*this);
}

// equality and assignment set operators
template <class DataType>
Set_BinaryTree<DataType> &
Set_BinaryTree<DataType>::operator=(const Set_BinaryTree<DataType> &sl)
{
	// check for self-assignment
	if (this != &sl)
		btree = sl.btree;
	return(*this);
}

template <class DataType>
int
Set_BinaryTree<DataType>::operator==(const Set_BinaryTree<DataType> &sl) const
{
	// check for self-comparison
	if (this == &sl)
		return(1);

	// compare individual elements
	BinaryTreeIterator_InOrder<DataType> pli1(btree);
	BinaryTreeIterator_InOrder<DataType> pli2(sl.btree);
	for ( ; ! pli1.done() && ! pli2.done(); pli1++, pli2++)
	{
		// do comparison
		if (pli1() != pli2()) 
			break;
	}
	int status;
	if (pli1.done() && pli2.done())
		status = 1;
	else
		status = 0;

	// all done
	return(status);
}

template <class DataType>
int
Set_BinaryTree<DataType>::operator!=(const Set_BinaryTree<DataType> &sl) const
{
	return(!(operator==(sl)));
}

template <class DataType>
int
Set_BinaryTree<DataType>::isMember(const DataType &m) const
{
	return(btree.includes(m));
}

template <class DataType>
int
Set_BinaryTree<DataType>::isEmpty() const
{
	return(btree.isEmpty());
}

// union set operations
template <class DataType>
Set_BinaryTree<DataType> &
Set_BinaryTree<DataType>::operator|=(const Set_BinaryTree<DataType> &sl)
{
	// insert new elements
	BinaryTreeIterator_InOrder<DataType> pli(sl.btree);
	for ( ; ! pli.done(); pli++)
	{
		btree.insert(pli());
	}
	return(*this);
}

template <class DataType>
Set_BinaryTree<DataType>
Set_BinaryTree<DataType>::operator|(const Set_BinaryTree<DataType> &sl) const
{
	return(Set_BinaryTree<DataType>(*this) |= sl);
}

// intersection set operations
template <class DataType>
Set_BinaryTree<DataType> &
Set_BinaryTree<DataType>::operator&=(const Set_BinaryTree<DataType> &sl)
{
	// generate intersection
	BinaryTree<DataType> pi;
	BinaryTreeIterator_InOrder<DataType> pli(sl.btree);
	for ( ; ! pli.done(); pli++)
	{
		if (btree.includes(pli()))
		{
			pi.insert(pli());
		}
	}
	btree = pi;
	return(*this);
}

template <class DataType>
Set_BinaryTree<DataType>
Set_BinaryTree<DataType>::operator&(const Set_BinaryTree<DataType> &sl) const
{
	return(Set_BinaryTree<DataType>(*this) &= sl);
}

// difference set operations
template <class DataType>
Set_BinaryTree<DataType> &
Set_BinaryTree<DataType>::operator-=(const Set_BinaryTree<DataType> &sl)
{
	// generate difference
	BinaryTreeIterator_InOrder<DataType> pli(sl.btree);
	for ( ; ! pli.done(); pli++)
	{
		DataType tmp = pli();
		(void)btree.remove(tmp);
	}
	return(*this);
}

template <class DataType>
Set_BinaryTree<DataType>
Set_BinaryTree<DataType>::operator-(const Set_BinaryTree<DataType> &sl) const
{
	return(Set_BinaryTree<DataType>(*this) -= sl);
}

// check if this set is a subset of another set.
template <class DataType>
int
Set_BinaryTree<DataType>::isSubsetOf(const Set_BinaryTree<DataType> &set) const
{
	// check if this set is a subset of another set
	BinaryTreeIterator_InOrder<DataType> subIter(btree);
	BinaryTreeIterator_InOrder<DataType> setIter(set.btree);
	while (!subIter.done() && !setIter.done())
	{
		if (subIter() == setIter())
		{
			// both are equal, increment both iterators
			setIter++;
			subIter++;
		}
		else if (subIter() > setIter())
		{
			// increment set to match sub.
			setIter++;
		}
		else
		{
			// a member was not found
			return(0);
		}
	}
	if (subIter.done())
		return(1);
	else
		return(0);
}

template <class DataType>
int
Set_BinaryTree<DataType>::isSupersetOf(const Set_BinaryTree<DataType> &super) const
{
	return(super.isSubsetOf(*this));
}

template <class DataType>
int
Set_BinaryTree<DataType>::isProperSubsetOf(const Set_BinaryTree<DataType> &set) const
{
	// check if this set is a subset of another set
	int subno = 0;
	int setno = 0;
	BinaryTreeIterator_InOrder<DataType> subIter(btree);
	BinaryTreeIterator_InOrder<DataType> setIter(set.btree);
	while (!subIter.done() && !setIter.done())
	{
		if (subIter() == setIter())
		{
			// both are equal, increment both iterators
			setno++;
			setIter++;
			subno++;
			subIter++;
		}
		else if (subIter() > setIter())
		{
			// increment set to match sub.
			setno++;
			setIter++;
		}
		else
		{
			// a member was not found
			return(0);
		}
	}
	if (!setIter.done())
	{
		setno++;
		setIter++;
	}
	if (subIter.done() && (subno < setno))
		return(1);
	else
		return(0);
}

template <class DataType>
int
Set_BinaryTree<DataType>::isProperSupersetOf(const Set_BinaryTree<DataType> &super) const
{
	return(super.isProperSubsetOf(*this));
}

#if USEOSTREAM
// print contents of set
template <class DataType>
std::ostream &
operator<<(std::ostream &os, const Set_BinaryTree<DataType> &sl)
{
	os << sl.btree;
	return(os);
}
#endif

// set iterator constructors and destructor
template <class DataType>
Set_BinaryTree_Iterator<DataType>::Set_BinaryTree_Iterator(const Set_BinaryTree<DataType> &sl):
	iterator(sl.btree)
{
	// do nothing
}

template <class DataType>
Set_BinaryTree_Iterator<DataType>::Set_BinaryTree_Iterator(
	const Set_BinaryTree_Iterator<DataType> &sliter):
	iterator(sliter.iterator)
{
	// do nothing
}

template <class DataType>
Set_BinaryTree_Iterator<DataType>::~Set_BinaryTree_Iterator()
{
	// do nothing
}

// reset iterator to start
template <class DataType>
void
Set_BinaryTree_Iterator<DataType>::reset()
{
	iterator.reset();
	return;
}

// is iterator done ?
template <class DataType>
int
Set_BinaryTree_Iterator<DataType>::done() const
{
	return(iterator.done());
}

// return current data
template <class DataType>
DataType
Set_BinaryTree_Iterator<DataType>::operator()()
{
	return(iterator());
}

// increment to next data item in btree
template <class DataType>
int
Set_BinaryTree_Iterator<DataType>::operator++(int)
{
	return(iterator++);
}

