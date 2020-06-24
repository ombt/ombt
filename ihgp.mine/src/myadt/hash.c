// functions for hash table class

// headers
#include <stdlib.h>
#include <iostream.h>
#include <assert.h>

// local headers
#include "hash.h"

// hash table constructors and destructor
HashTable::HashTable(int tsz, HashFn hfunc):
        tableSize(tsz), hasher(hfunc), table(NULLTABLE)
{
        // allocate table
        table = new List *[tableSize];
        assert(table != NULLTABLE);

        // initialize table entries to null
        for (int ib = 0; ib < tableSize; ib++)
        {
                table[ib] = NULLLIST;
        }
        assert((table != NULLTABLE) && (hasher != NULLHASHFN));
}

HashTable::HashTable(const HashTable &ht):
        tableSize(ht.tableSize), hasher(ht.hasher), table(NULLTABLE)
{
        // allocate table
        table = new List *[tableSize];
        assert(table != NULLTABLE);

        // initialize table entries to null or copy lists
        for (int ib = 0; ib < tableSize; ib++)
        {
                if (ht.table[ib] != NULLLIST)
                {
                        table[ib] = new List(*ht.table[ib]);
                        assert(table[ib] != NULLLIST);
                }
                else
                        table[ib] = NULLLIST;
        }
        assert((table != NULLTABLE) && (hasher != NULLHASHFN));
}

HashTable::~HashTable()
{
        // delete hash table lists, if any
        if (table != NULLTABLE)
        {
                for (int ib = 0; ib < tableSize; ib++)
                {
                        if (table[ib] != NULLLIST)
                        {
                                delete table[ib];
                                table[ib] = NULLLIST;
                        }
                }
        }

        // delete hash table
        delete [] table;
        table = NULLTABLE;
        tableSize = 0;
        hasher = NULLHASHFN;
}

// assignment
HashTable &
HashTable::operator=(const HashTable &ht)
{
        // check for self-assignment
        if (this == &ht) return(*this);

        // delete hash table
        for (int ib = 0; ib < tableSize; ib++)
        {
                if (table[ib] != NULLLIST)
                {
                        delete table[ib];
                        table[ib] = NULLLIST;
                }
        }
        delete [] table;
        table = NULLTABLE;
        tableSize = 0;
        hasher = NULLHASHFN;

        // copy new hash table
        hasher = ht.hasher;
        tableSize = ht.tableSize;
        table = new List *[tableSize];
        assert(table != NULLTABLE);

        // initialize table entries to null or copy lists
        for (ib = 0; ib < tableSize; ib++)
        {
                if (ht.table[ib] != NULLLIST)
                {
                        table[ib] = new List(*ht.table[ib]);
                        assert(table[ib] != NULLLIST);
                }
                else
                        table[ib] = NULLLIST;
        }
        assert((table != NULLTABLE) && (hasher != NULLHASHFN));

        // all done
        return(*this);
}

// hash table operations
int
insert(HashTable &ht, DataItem *d)
{
        // get bucket for data item
        int ib = (*ht.hasher)(d, ht.tableSize);

        // check if bucket is null
        if (ht.table[ib] == NULLLIST)
        {
                // allocate a link list
                ht.table[ib] = new List();
                assert(ht.table[ib] != NULLLIST);

                // insert entry in bucket
                if (insert(*ht.table[ib], d, ENDOFLIST) != OK)
                {
                        ERRORI("insert failed for bucket ...", ib);
                        return(NOTOK);
                }
                return(OK);
        }

        // check if entry is already in the hash table
        ListPosition lp;
        if (retrieve(*ht.table[ib], d, lp) == OK)
        {
                // delete old data item
                if (remove(*ht.table[ib], lp) != OK)
                {
                        ERRORI("remove failed for bucket ...", ib);
                        return(NOTOK);
                }
        }
        
        // insert entry in bucket
        if (insert(*ht.table[ib], d, ENDOFLIST) != OK)
        {
                ERRORI("insert failed for bucket ...", ib);
                return(NOTOK);
        }

        // all done
        return(OK);
}

int
remove(HashTable &ht, DataItem *d)
{
        // get bucket for data item
        int ib = (*ht.hasher)(d, ht.tableSize);

        // remove from hash table
        ListPosition p;
        if (ht.table[ib] == NULLLIST)
                return(NOMATCH);
        if (locate(*ht.table[ib], d, p) != OK)
                return(NOMATCH);
        if (remove(*ht.table[ib], p) != OK)
        {
                ERRORI("remove failed for bucket ...", ib);
                return(NOTOK);
        }

        // all done
        return(OK);
}

int
retrieve(const HashTable &ht, DataItem *&d)
{
        // get bucket for data item
        int ib = (*ht.hasher)(d, ht.tableSize);

        // remove from hash table
        ListPosition p;
        if (ht.table[ib] == NULLLIST)
                return(NOMATCH);
        if (locate(*ht.table[ib], d, p) != OK)
                return(NOMATCH);
        if (retrieve(*ht.table[ib], d, p) != OK)
        {
                ERRORI("retrieve failed for bucket ...", ib);
                return(NOTOK);
        }

        // all done
        return(OK);
}

int
isInTable(const HashTable &ht, DataItem *d)
{
        // get bucket for data item
        int ib = (*ht.hasher)(d, ht.tableSize);

        // find data in table
        ListPosition p;
        if ((ht.table[ib] == NULLLIST) || 
            (locate(*ht.table[ib], d, p) != OK)) 
                return(0);
        else
                return(1);
}

int
makeNull(HashTable &ht)
{
        // delete hash table lists, if any
        for (int ib = 0; ib < ht.tableSize; ib++)
        {
                if (ht.table[ib] != NULLLIST)
                {
                        delete ht.table[ib];
                        ht.table[ib] = NULLLIST;
                }
        }
        return(OK);
}

// print table
void
HashTable::dump(ostream &os) const
{
        os << "hash table = {" << endl;
        for (int ib = 0; ib < tableSize; ib++)
        {
                os << "bucket[" << ib << "] = ";
                if (table[ib] != NULLLIST)
                        table[ib]->dump(os);
                else
                        os << "{  }";
                os << endl;
        }
        os << "}" << endl;
        return;
}

ostream &
operator<<(ostream &os, const HashTable &ht)
{
        ht.dump(os);
        return(os);
}
