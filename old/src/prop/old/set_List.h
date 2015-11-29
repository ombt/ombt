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
        Set_List(const Set_List &);
        ~Set_List();

        // insert member operations
        Set_List &operator+=(const DataType &);
        Set_List operator+(const DataType &) const;
        Set_List &insert(const DataType &);

        // delete member operations
        Set_List &operator-=(DataType &);
        Set_List operator-(DataType &) const;
        Set_List &remove(DataType &);
        Set_List &clear();

        // equality and assignment set operations
        Set_List &operator=(const Set_List &);
        int operator==(const Set_List &) const;
        int operator!=(const Set_List &) const;
        int isMember(const DataType &) const;
        int isEmpty() const;

        // union set operations
        Set_List &operator|=(const Set_List &);
        Set_List operator|(const Set_List &) const;

        // intersection set operations
        Set_List &operator&=(const Set_List &);
        Set_List operator&(const Set_List &) const;

        // difference set operations
        Set_List &operator-=(const Set_List &);
        Set_List operator-(const Set_List &) const;

	// check if another set is a subset.
	int isSubsetOf(const Set_List &) const;
	int isSupersetOf(const Set_List &) const;
	int isProperSubsetOf(const Set_List &) const;
	int isProperSupersetOf(const Set_List &) const;

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
