#ifndef __ARRAY_H
#define __ARRAY_H
// array class definition

// headers
#include <stdlib.h>
#include <iostream.h>

// local headers
#include "returns.h"
#include "debug.h"
#include "absIterator.h"

// forward declarations
template <class DataType> class Array;
template <class DataType> class ArrayIterator;
template <class DataType> class ArrayIterator_Reverse;

// forward declaration of friend functions
template <class DataType> 
ostream &
operator<<(ostream &, const Array<DataType> &);

// array class
template <class DataType> class Array {
public:
        // friend class
        friend class ArrayIterator<DataType>;
        friend class ArrayIterator_Reverse<DataType>;

        // constructors and destructor
        Array(int);
        Array(int, int);
        Array(const Array &);
        ~Array();

        // operators
        Array &operator=(const Array &);
        DataType &operator[](int);
        DataType &operator[](int) const;

        // miscellaneous
	void setSize(int);
	void setBounds(int, int);
	int getSize() const;
	int getLower() const;
	int getUpper() const;

	// print arrays
        friend ostream &::operator<<(ostream &, const Array<DataType> &);

private:
	// don't allow this
        Array();

protected:
        // internal data
	int size, lower, upper;
        DataType *data;
};

// array iterator
template <class DataType> class ArrayIterator:
	public AbstractIterator<DataType>
{
public:
        // constructors and destructor
        ArrayIterator(const ArrayIterator &);
        ArrayIterator(const Array<DataType> &);
        ~ArrayIterator();

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
        ArrayIterator();
	ArrayIterator &operator=(const ArrayIterator<DataType> &);

protected:
        // internal data
	const Array<DataType> &array;
	int current;
};

// array iterator
template <class DataType> class ArrayIterator_Reverse:
	public AbstractIterator<DataType>
{
public:
        // constructors and destructor
        ArrayIterator_Reverse(const ArrayIterator_Reverse &);
        ArrayIterator_Reverse(const Array<DataType> &);
        ~ArrayIterator_Reverse();

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
        ArrayIterator_Reverse();
	ArrayIterator_Reverse &operator=(const ArrayIterator_Reverse<DataType> &);

protected:
        // internal data
	const Array<DataType> &array;
	int current;
};

#endif
