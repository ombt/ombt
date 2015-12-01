#ifndef __MEMORY_LEAK_H
#define __MEMORY_LEAK_H
// memory leak detection header

// headers
#include <stdlib.h>
#include <string.h>
#include <iostream>

// local headers
#include <Returns.h>
#include <Debug.h>

using namespace std;

// forward declarations
class MemoryLeak;
class MemoryLeakItem;

// forward declarations for friend functions
ostream &
operator<<(ostream &, const MemoryLeakItem &);

ostream &
operator<<(ostream &, const MemoryLeak &);

// memory leak class definitions
class MemoryLeakItem {
private:
	// friends
	friend class MemoryLeak;

	// constructors and destructor
	MemoryLeakItem(unsigned long, int);
	~MemoryLeakItem();

	// output operators
	friend ostream &operator<<(ostream &, const MemoryLeakItem &);
	friend ostream &operator<<(ostream &, const MemoryLeak &);

	// internal data
	int usecounts;
	unsigned long address;
	MemoryLeakItem *previous;
	MemoryLeakItem *next;
};

class MemoryLeak {
public:
        // constructors and destructor
	MemoryLeak(int, int);
        ~MemoryLeak();

	// hash table operations
	void clear();
	int retrieve(unsigned long &, int &) const;
	int insert(unsigned long, int);
	int update(unsigned long, int);
	int remove(unsigned long &, int &);
	int isEmpty() const;

	// access data functions
	int getSize() const {
		return(size);
	}

	// hashing function 
	static unsigned long hash(unsigned long);

	// output operator
	friend ostream &operator<<(ostream &, const MemoryLeak &);

protected:
	// data
	int size;
	int datasize;
	int maximumtotalcounts;
	int currenttotalcounts;
	int *counts;
	MemoryLeakItem **first;
	MemoryLeakItem **last;
};

#endif
