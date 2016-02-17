// member functions for an array class

// required headers
#include "array.h"

// list item constructors and destructor
template <class DataType>
Array<DataType>::Array(int sz):
	size(sz), lower(0), upper(sz-1), data(new DataType [sz])
{
	MustBeTrue(sz > 0 && data != NULL);
}

template <class DataType>
Array<DataType>::Array(int l, int u):
	size(u-l+1), lower(l), upper(u), data(new DataType [u-l+1])
{
	MustBeTrue(size > 0 && data != NULL);
}

template <class DataType>
Array<DataType>::Array(const DataType &defaultValue, int sz):
	size(sz), lower(0), upper(sz-1), data(new DataType [sz])
{
	MustBeTrue(sz > 0 && data != NULL);
	for (int ia = 0; ia < size; ia++)
	{
		data[ia] = defaultValue;
	}
}

template <class DataType>
Array<DataType>::Array(const DataType &defaultValue, int l, int u):
	size(u-l+1), lower(l), upper(u), data(new DataType [u-l+1])
{
	MustBeTrue(size > 0 && data != NULL);
	for (int ia = 0; ia < size; ia++)
	{
		data[ia] = defaultValue;
	}
}

template <class DataType>
Array<DataType>::Array(const Array<DataType> &src):
	size(src.size), lower(src.lower), upper(src.upper), 
	data(new DataType [src.size])
{
	MustBeTrue(data != NULL);
	for (int ia = 0; ia < size; ia++)
	{
		data[ia] = src.data[ia];
	}
}

template <class DataType>
Array<DataType>::~Array()
{
	delete [] data;
	data = NULL;
	size = 0;
	lower = 0;
	upper = 0;
}

// assignment and equivalence
template <class DataType>
Array<DataType> &
Array<DataType>::operator=(const Array<DataType> &rhs)
{
	if (this != &rhs)
	{
		// delete old data
		delete [] data;

		// allocate new data
		size = rhs.size;
		data = new DataType [size];
		MustBeTrue(data != NULL);

		// copy over data
		lower = rhs.lower;
		upper = rhs.upper;
		for (int ia = 0; ia < size; ia++)
		{
			data[ia] = rhs.data[ia];
		}
	}
	return(*this);
}

template <class DataType>
DataType &
Array<DataType>::operator[](int ia)
{
	MustBeTrue(lower <= ia && ia <= upper);
	return(data[ia-lower]);
}

template <class DataType>
DataType &
Array<DataType>::operator[](int ia) const
{
	MustBeTrue(lower <= ia && ia <= upper);
	return(data[ia-lower]);
}

// set and get size of array
template <class DataType>
int
Array<DataType>::getSize() const
{
	return(size);
}

template <class DataType>
int
Array<DataType>::getLower() const
{
	return(lower);
}

template <class DataType>
int
Array<DataType>::getUpper() const
{
	return(upper);
}

template <class DataType>
void
Array<DataType>::setSize(int newsz)
{
	// check new size
	MustBeTrue(newsz > 0);

	// allocate new data block
	DataType *newdata = new DataType [newsz];
	MustBeTrue(newdata != NULL);

	// copy over data
	int maxia = (newsz < size) ? newsz : size;
	for (int ia = 0; ia < maxia; ia++)
	{
		newdata[ia] = data[ia];
	}

	// delete old data and save new data
	delete [] data;
	data = newdata;
	size = newsz;
	lower = 0;
	upper = size - 1;
	return;
}

template <class DataType>
void
Array<DataType>::setBounds(int l, int u)
{
	// check new size
	int newsz = u - l + 1;
	MustBeTrue(newsz > 0);

	// allocate new data block
	DataType *newdata = new DataType [newsz];
	MustBeTrue(newdata != NULL);

	// copy over data
	int maxia = (newsz < size) ? newsz : size;
	for (int ia = 0; ia < maxia; ia++)
	{
		newdata[ia] = data[ia];
	}

	// delete old data and save new data
	delete [] data;
	data = newdata;
	size = newsz;
	lower = l;
	upper = u;
	return;
}

// print list item
template <class DataType>
ostream &
operator<<(ostream &os, const Array<DataType> &d)
{
	for (int ia = 0; ia < d.size; ia++)
	{
		os << d.data[ia] << " ";
	}
	return(os);
}

// array iterator constructors and destructor
template <class DataType>
ArrayIterator<DataType>::ArrayIterator(const ArrayIterator<DataType> &ai): 
	array(ai.array), current(ai.current)
{
	// do nothing
}

template <class DataType>
ArrayIterator<DataType>::ArrayIterator(const Array<DataType> &a): 
	array(a), current(0)
{
	// do nothing
}

template <class DataType>
ArrayIterator<DataType>::~ArrayIterator()
{
	current = 0;
}

// reset iterator to beginning
template <class DataType>
void
ArrayIterator<DataType>::reset()
{
	current = 0;
	return;
}

// is list empty
template <class DataType>
int
ArrayIterator<DataType>::done() const
{
	return(current >= array.size);
}

// return current data
template <class DataType>
DataType
ArrayIterator<DataType>::operator()()
{
	return(array.data[current]);
}

// increment to next data item in list
template <class DataType>
int
ArrayIterator<DataType>::operator++(int)
{
	if (current < array.size)
	{
		current++;
		return(OK);
	}
	else
		return(NOTOK);
}

// get current index
template <class DataType>
int
ArrayIterator<DataType>::getIndex() const
{
	return(array.lower + current);
}

// array reverse iterator constructors and destructor
template <class DataType>
ArrayIterator_Reverse<DataType>::ArrayIterator_Reverse(
	const ArrayIterator_Reverse<DataType> &ai): 
	array(ai.array), current(ai.current)
{
	// do nothing
}

template <class DataType>
ArrayIterator_Reverse<DataType>::ArrayIterator_Reverse(
	const Array<DataType> &a): 
	array(a), current(a.size-1)
{
	// do nothing
}

template <class DataType>
ArrayIterator_Reverse<DataType>::~ArrayIterator_Reverse()
{
	current = array.size-1;
}

// reset iterator to beginning
template <class DataType>
void
ArrayIterator_Reverse<DataType>::reset()
{
	current = array.size-1;
	return;
}

// is list empty
template <class DataType>
int
ArrayIterator_Reverse<DataType>::done() const
{
	return(current < 0);
}

// return current data
template <class DataType>
DataType
ArrayIterator_Reverse<DataType>::operator()()
{
	return(array.data[current]);
}

// increment to next data item in list
template <class DataType>
int
ArrayIterator_Reverse<DataType>::operator++(int)
{
	if (current >= 0)
	{
		current--;
		return(OK);
	}
	else
		return(NOTOK);
}

// get current index
template <class DataType>
int
ArrayIterator_Reverse<DataType>::getIndex() const
{
	return(array.lower + current);
}

