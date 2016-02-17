#ifndef __SET_LIST_H
#define __SET_LIST_H
// list-based set class definition

// headers
#include "list.h"

// forward declarations
template <class DataType> class Set_List;
template <class DataType> class Set_List_Iterator;

// list-based set class
template <class DataType> class Set_List {
public:
	// friends
	friend class Set_List_Iterator<DataType>;

        // destructor
        Set_List();
        Set_List(const Set_List<DataType> &);
        ~Set_List();

        // insert member operations
        Set_List<DataType> &operator+=(const DataType &);
        Set_List<DataType> operator+(const DataType &) const;
        Set_List<DataType> &insert(const DataType &);

        // delete member operations
        Set_List<DataType> &operator-=(DataType &);
        Set_List<DataType> operator-(DataType &) const;
        Set_List<DataType> &remove(DataType &);
        Set_List<DataType> &clear();

        // equality and assignment set operations
        Set_List<DataType> &operator=(const Set_List<DataType> &);
        int operator==(const Set_List<DataType> &) const;
        int operator!=(const Set_List<DataType> &) const;
        int isMember(const DataType &) const;
        int isEmpty() const;

        // union set operations
        Set_List<DataType> &operator|=(const Set_List<DataType> &);
        Set_List<DataType> operator|(const Set_List<DataType> &) const;

        // intersection set operations
        Set_List<DataType> &operator&=(const Set_List<DataType> &);
        Set_List<DataType> operator&(const Set_List<DataType> &) const;

        // difference set operations
        Set_List<DataType> &operator-=(const Set_List<DataType> &);
        Set_List<DataType> operator-(const Set_List<DataType> &) const;

	// check if another set is a subset.
	int isSubsetOf(const Set_List<DataType> &) const;
	int isSupersetOf(const Set_List<DataType> &) const;
	int isProperSubsetOf(const Set_List<DataType> &) const;
	int isProperSupersetOf(const Set_List<DataType> &) const;

	// output data
	friend ostream &operator<<(ostream &, const Set_List<DataType> &);

protected:
	// data
	List<DataType> list;
};

// list-based set iterator class
template <class DataType> class Set_List_Iterator
{
public:
        // constructors and destructor
        Set_List_Iterator(const Set_List<DataType> &);
        Set_List_Iterator(const Set_List_Iterator<DataType> &);
        ~Set_List_Iterator();

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
        Set_List_Iterator &operator=(const Set_List_Iterator<DataType> &);

protected:
	// internal list iterator
	ListIterator<DataType> iterator;
};

#endif
