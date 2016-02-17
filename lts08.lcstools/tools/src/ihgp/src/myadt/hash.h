#ifndef __HASHTABLE_H
#define __HASHTABLE_H
// hash table class definitions

// headers
#include <stdlib.h>
#include <iostream.h>
#include <assert.h>

// local headers
#include "returns.h"
#include "debug.h"
#include "data.h"
#include "list.h"

// forward declarations
class HashTable;
typedef int (*HashFn)(DataItem *, int);

// definitions
#define NULLHASHFN ((HashFn) 0)
#define NULLTABLE ((List **)0)
#define NULLHASHTABLE ((HashTable *)0)

// hash table class
class HashTable {
public:
        // constructors and destructor
        HashTable(int, HashFn);
        HashTable(const HashTable &);
        virtual ~HashTable();

        // assignment
        virtual HashTable &operator=(const HashTable &);

        // hash table operations
        friend int insert(HashTable &, DataItem *);
        friend int remove(HashTable &, DataItem *);
        friend int retrieve(const HashTable &, DataItem *&);
        friend int isInTable(const HashTable &, DataItem *);
        friend int makeNull(HashTable &);

        // print table
        virtual void dump(ostream &os = cout) const;
        friend ostream &operator<<(ostream &, const HashTable &);

protected:
        // internal data
        int tableSize;
        HashFn hasher;
        List **table;
};

#endif

