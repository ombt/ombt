#ifndef __HASHTABLE_ARRAY_H
#define __HASHTABLE_ARRAY_H
// hash table class definition

// headers
#include <adt/AbsHashTable.h>
#include <adt/AbsIterator.h>
#include <adt/Array.h>

namespace ombt {

// forward declarations
template <class DataType> class HashTable_Array;
template <class DataType> class HashTable_Array_Iterator;

// status of hash table bucket
struct HashTableBucketStatus {
public:
	// status of bucket
	enum HashTableBucketStatusEnum {
		Empty, Deleted, Used
	};

	// constructors and destructor
	HashTableBucketStatus(): 
		status(Empty) {
	}
	HashTableBucketStatus(const HashTableBucketStatus &src):
		status(src.status) {
	}
	~HashTableBucketStatus() {
		status = Deleted;
	}

	// assignment 
	HashTableBucketStatus &operator=(const HashTableBucketStatus &rhs) {
		status = rhs.status;
		return(*this);
	}

	// data
	HashTableBucketStatusEnum status;
};

// hash table class
template <class DataType> class HashTable_Array:
	public AbstractHashTable<DataType> {
public:
	// friend classes
	friend class HashTable_Array_Iterator<DataType>;

        // destructor
	HashTable_Array(int, int (*)(const DataType &));
        HashTable_Array(const HashTable_Array &);
        ~HashTable_Array();

	// assignment 
	HashTable_Array &operator=(const HashTable_Array &);

	// hash table operations
	void clear();
	int insert(const DataType &);
	int remove(DataType &);
	int retrieve(DataType &) const;
	int includes(const DataType &) const;
	int isEmpty() const;

	// output data
	std::ostream &dump(std::ostream &) const;

protected:
	// data
	int size;
	int (*hash)(const DataType &);
	Array<HashTableBucketStatus> status;
	Array<DataType> array;
};

// hash table iterator
template <class DataType> class HashTable_Array_Iterator:
	public AbstractIterator<DataType>
{
public:
        // constructors and destructor
        HashTable_Array_Iterator(const HashTable_Array_Iterator &);
        HashTable_Array_Iterator(const HashTable_Array<DataType> &);
        ~HashTable_Array_Iterator();

        // initialization
	void reset();

	// check if at end of list
	int done() const;

        // return data 
        DataType operator()();

	// advance iterator to next link
	int operator++(int);

private:
	// not allowed
        HashTable_Array_Iterator();
	HashTable_Array_Iterator &operator=(
		const HashTable_Array_Iterator<DataType> &);

protected:
        // internal data
	const HashTable_Array<DataType> &table;
	int current;
};

#include <adt/HashTable_Array.cpp>

}

#endif
